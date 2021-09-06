#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
using namespace std;

#define RED             "\x1b[31m"
#define BOLDRED         "\033[1m\033[31m"
#define MAGENTA         "\033[35m"
#define BOLDMAGENTA     "\033[1m\033[35m"
#define BLUE            "\033[34m"
#define BOLDBLUE        "\033[1m\033[34m"
#define CYAN            "\033[36m" 
#define BOLDCYAN        "\033[1m\033[36m"
#define WHITE           "\x1b[38m"
#define PURPLE          "\x1b[38;2;253;159;197m"
#define COLORENDS       "\x1b[0m"
#define BACKGROUND_RED  "\x1b[48;2;110;20;12m"
#define BOLDWHITE       "\033[1m\033[37m"

const char * TW[] = { "", "Boolean", "break", "continue", "do", "else", "false", "for", "function", "getenv", 
                "if", "in", "NaN", "Number", "null", "Object", "return", "String", "true", "typeof",
                "undefined", "var", "while", "write", NULL };

const char * TD[] = { "", "@", ";", ",", ":", ".", "(", ")", "[", "]", "{", "}", 
                "=", "==", "===", "<", ">", 
                "+", "+=", "++", "-", "-=", "--", 
                "*", "*=", "*/", "/", "/=", "/*", "//", 
                "%", "%=", "<=", "!", "!=", "!==", ">=", 
                "|", "||", "&", "&&", NULL };

enum state { INIT, IDENT, NUMB, MULTCOM, HELPCOM, SINGCOM, SLSH, ALE, FINALE, PLUS, MINUS, AMP, PIPE, QUOTE };

enum LexType {
    // TW starts
    L_NULL,                                                                                                
    L_BOOL, L_BREAK, L_CONTINUE, L_DO, L_ELSE, L_FALSE, L_FOR, L_FUNCTION, L_GETENV, L_IF, L_IN,            
    L_NAN, L_NUMBER, L_NULLPTR, L_OBJECT, L_RETURN, L_STRING, L_TRUE, L_TYPEOF, 	                      
    L_UNDEFINED, L_VAR, L_WHILE, L_WRITE,
    // TD starts
    L_FIN,                                                                                                   
    L_SEMICOLON, L_COMMA, L_COLON, L_DOT, L_LPAREN, L_RPAREN, L_LQPAREN, L_RQPAREN, L_BEGIN, L_END,             
    L_EQ, L_DEQ, L_TEQ, L_LSS, L_GTR, L_PLUS, L_PLUS_EQ, L_DPLUS, L_MINUS, L_MINUS_EQ, L_DMINUS,                
    L_TIMES, L_TIMES_EQ, L_TIMES_SLASH, L_SLASH, L_SLASH_EQ, L_SLASH_TIMES, L_DSLASH, L_PERCENT, L_PERCENT_EQ,  
    L_LEQ, L_NOT, L_NEQ, L_NDEQ, L_GEQ, L_OR, L_DPIPE, L_AND, L_DAMP, 
    L_ID, L_NUMB, L_STR_CONST                                                                                              
};

class Ident {
    string name_;       // Contains Ident' name
    LexType itype_;
    int ivalue_;        // Contains Ident' content
public:

    Ident (string n) : name_(n) {}
    bool operator==(const string& s) const { return name_ == s; }

    LexType GetType () { return itype_; }
    int GetValue () { return ivalue_; }
    string GetName () { return name_; }

    void SetType (LexType t) { itype_ = t; }
    void SetValue (int v) { ivalue_ = v; }

    friend ostream& operator << ( ostream &out, Ident l )
    {
        out << '(' << l.itype_ << ',' << l.ivalue_ << ");";
        return out;
    }
};

vector<Ident> TID;
int addtoTID(const string& str)
{
    vector<Ident>::iterator i;
    i = find(TID.begin(), TID.end(), str);
    if (i != TID.end()) return(i - TID.begin());
    else
    {
        TID.push_back(Ident(str));
        return(TID.size() - 1);
    }
}

class Lex {
    LexType ltype_;
    int lvalue_;
    string lstr_;      // Used if LexType is String
public:

    static int count_;
    Lex (LexType t = L_NULL, int v = 0, string str = "") : ltype_(t), lvalue_(v), lstr_(str) {}

    LexType GetType () { return ltype_; }
    int GetValue () { return lvalue_; }

    void SetType (LexType t) { ltype_ = t; }
    void SetValue (int v) { lvalue_ = v; }

    friend ostream& operator<< (ostream &out, Lex l);
};

int Lex::count_ = 0;

ostream& operator<< (ostream &out, Lex l)
{
    l.count_++;
    string type, adding;
    if (l.ltype_ == L_FIN)
    {    type = "@";                            adding = "TD:"; }
    else if (l.ltype_ <= L_WRITE)               
    {    type = (string)TW[l.ltype_];           adding = "TW:"; }
    else if ((l.ltype_ <= L_DAMP) && (l.ltype_ >= L_FIN))
    {    type = (string)TD[l.ltype_ - L_FIN];   adding = "TD:"; }
    else if (l.ltype_ == L_NUMB)
    {    type = "Number";                       adding = ""; }

    if (l.ltype_ == L_ID)
    {
        type = TID[l.lvalue_].GetName();
        out << BOLDRED << l.count_ << BOLDBLUE << ' ' << type << BOLDWHITE << " << " << BOLDMAGENTA << "TID:" << l.lvalue_ << COLORENDS << "\n";
    }
    else if ((l.ltype_ == L_STR_CONST) || (l.ltype_ == L_STRING))
    {
        type = "String";
        out << BOLDRED << l.count_ << BOLDBLUE << ' ' << type << BOLDWHITE << " >> " << BOLDMAGENTA << adding << l.lstr_ << COLORENDS << "\n";
    }
    else 
        out << RED << l.count_ << BLUE << ' ' << type << COLORENDS << " == " << MAGENTA << adding << l.lvalue_ << COLORENDS << "\n";
    return out;
}

class Scanner {
    int fd;
    bool flag_read_type = 0;
    bool flag_additional = 1;
    char c;
    char gc() 
    { 
        if (flag_read_type) read(fd, &c, 1); 
        else c = getchar(); 
        return(c); 
    }
    int seek(const string s, const char** table)
    {
        int i = 0;
        while (table[i] != NULL)
        {
            if (s == table[i])
                return(i);
            i++;
        }
        return(0);
    }
public:

    Scanner() 
    {}
    Scanner(const char * argv1) 
    {
        if ((fd = open(argv1, O_RDONLY)) < 0) 
        {
            perror("cant open js file");
            terminate();
        }
        else flag_read_type = 1;
    }

    Lex GetLex()
    {
        int dig, j;
        state CS = INIT;
        string str;

        do
        {
            if (flag_additional) gc(); else flag_additional = 1;
            switch(CS)
            {
                case INIT:
                    if ( c ==' ' || c =='\n' || c=='\r' || c =='\t' ) ;
                    else if (isalpha(c))
                    {
                        str.push_back(c);
                        CS = IDENT;
                    }
                    else if (isdigit(c))
                    {
                        dig = c - '0';
                        CS = NUMB;
                    }
                    else if (c == '+')
                    {
                        str.push_back(c);
                        CS = PLUS;
                    }                   
                    else if (c == '-')
                    {
                        str.push_back(c);
                        CS = MINUS;
                    }
                    else if (c == '/')
                    {
                        str.push_back(c);
                        CS = SLSH;
                    }
			        else if (c == '#')
			        {
                        CS = SINGCOM;
			        }
                    else if (c == '!' || c == '=')
                    {
                        str.push_back(c);
                        CS = ALE;
                    }
                    else if (c == '*' || c == '<' || c == '>' || c == '%')
                    {
                        str.push_back(c);
                        CS = FINALE;
                    }
                    else if (c == '&')
                    {
                        str.push_back(c);
                        CS = AMP;
                    }
                    else if (c == '|')
                    {
                        str.push_back(c);
                        CS = PIPE;
                    }
                    else if (c == '"')
                    {
                        CS = QUOTE;
                    }    
                    else if (c == '@')
                    {
                        return(Lex((LexType)L_FIN, j));
                    }
                    else 
                    {
                        str.push_back(c);
                        if (j = seek(str, TD))
                            return(Lex((LexType)(j + (int)L_FIN), j));
                        else throw(c);
                    }
                    break;  
                case IDENT:
                    if (isalpha(c) || isdigit(c))
                    {
                        str.push_back(c);
                    }          
                    else
                    {
                        flag_additional = 0;
                        if (j = seek(str, TW))
                        {
                            return Lex((LexType)j, j);
                        }
                        else 
                        {
                            j = addtoTID(str);
                            return Lex(L_ID, j);
                        }
                    }
                    break;
                case NUMB:
                    if (isdigit(c))
                    {
                        dig = 10 * dig + (c - '0');
                    }
                    else if (isalpha(c))
                        throw(c);
                    else
                    {
                        flag_additional = 0;
                        return Lex(L_NUMB, dig);
                    }
                    break;
                case PLUS:
                    if (c == '=' || c == '+')
                    {
                        str.push_back(c);
                        j = seek(str, TD);
                        return Lex((LexType)(j + (int)L_FIN), j);
                    }
                    else
                    {
                        flag_additional = 0;
                        j = seek(str, TD);
                        return Lex((LexType)(j + (int)L_FIN), j);
                    }
                    break;     
                case MINUS:
                    if (c == '=' || c == '-')
                    {
                        str.push_back(c);
                        j = seek(str, TD);
                        return Lex((LexType)(j + (int)L_FIN), j);
                    }
                    else
                    {
                        flag_additional = 0;
                        j = seek(str, TD);
                        return Lex((LexType)(j + (int)L_FIN), j);
                    }
                    break;      
                case AMP:
                    if (c == '&')
                    {
                        str.push_back(c);
                        j = seek(str, TD);
                        return Lex((LexType)(j + (int)L_FIN), j);
                    }
                    else
                    {
                        flag_additional = 0;
                        j = seek(str, TD);
                        return Lex((LexType)(j + (int)L_FIN), j);
                    }
                    break;    
                case PIPE:
                    if (c == '|')
                    {
                        str.push_back(c);
                        j = seek(str, TD);
                        return Lex((LexType)(j + (int)L_FIN), j);
                    }
                    else
                    {
                        flag_additional = 0;
                        j = seek(str, TD);
                        return Lex((LexType)(j + (int)L_FIN), j);
                    }
                    break;  
                case QUOTE:
                    if (c == '"') // End of readline
                    {
                        string quoted_str = "";
                        quoted_str += str;
                        return Lex(L_STR_CONST, 0, quoted_str);
                    }  
                    else if (c == '@')
                        throw(c);
                    str.push_back(c);
                    break;    
                case SLSH:
                    if (c == '=')
                    {
                        str.push_back(c);
                        j = seek(str, TD);
                        return Lex((LexType)(j + (int)L_FIN), j);
                    }
                    else if (c == '/')
                    {
                        str.pop_back();
                        CS = SINGCOM;
                    }
                    else if (c == '*')
                    {
                        str.pop_back();
                        CS = MULTCOM;
                    }
                    else
                    {
                        flag_additional = 0;
                        j = seek(str, TD);
                        return Lex((LexType)(j + (int)L_FIN), j);
                    }
                    break;
                case SINGCOM:
                    if (c == '\n')
                        CS = INIT;
                    else if (c == '@')
                        throw(c);
                    break;
                case MULTCOM:
                    if (c == '*')
                        CS = HELPCOM;
                    else if (c == '@')
                        throw(c);
                    break;
                case HELPCOM:
                    if (c == '/')
                        CS = INIT;
                    else if (c == '@')
                        throw(c);
                    else
                        CS = MULTCOM;
                    break;
                case ALE:
                    if (c == '=')
                    {
                        str.push_back(c);
                        CS = FINALE;
                    }  
                    else
                    {
                        flag_additional = 0;
                        j = seek(str, TD);
                        return Lex((LexType)(j + (int)L_FIN), j);
                    }
                    break;
                case FINALE: 
                    if (c == '=')
                    {
                        str.push_back(c);
                        j = seek(str, TD);
                        return Lex((LexType)(j + (int)L_FIN), j);            
                    }   
                    else
                    {
                        flag_additional = 0;
                        j = seek(str, TD);
                        return Lex((LexType)(j + (int)L_FIN), j);                       
                    }    
                    break;    
            }
        } while(1);
    }
};



int main(int argc, char** argv)
{
    cout << BOLDRED << "=======[ Лексический анализ начинается: ]========" << COLORENDS << "\n";
    Scanner A;
    if (argc >= 3) 
    {
        perror("argc is too big");
        terminate();
    }
    else if (argc == 2) 
        A = Scanner(argv[1]);
    else 
        A = Scanner();

    Lex a;
    for(;;)
    {
        try { a = A.GetLex(); }
        catch (char c)
        { cout << "ERROR: " << c << "\n"; terminate(); }
        cout << a;
        if (a.GetType() == L_FIN) break;
    }
    cout << BOLDRED << "=====[ Лексический анализ заканчивается... ]====\n" << COLORENDS << "\n";

    return 0;
}