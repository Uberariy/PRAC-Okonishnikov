
#include <iostream>
#include <cmath>
#include <string>
#include <cstring>
using namespace std;

class Student {
    char _name[64];
    long _number;
    int _date_year;
    int _date_month;
    int _date_day;

public:
    friend class Group;
    friend std::ostream& operator<< (std::ostream &out, const Student &st);

 //   Student(): 
   // _number(0), _date_year(0), _date_month(0), _date_day(0){strcpy(_name, "UNDEFINED");}

    Student(const char * name="UNDEFINED", long number=0, int day=0, int month=0, int year=0): 
    _number(number), _date_year(year), _date_month(month), _date_day(day)
    {   strcpy(_name, name);   }

    friend void StSwap(Student &a, Student &b);

};

void StSwap(Student &a, Student &b)
{
    Student c=a; a=b; b=c;
}

std::ostream& operator<< (std::ostream &out, const Student &st)
{
    out << st._name << '\t' << st._date_day << '.' << st._date_month << '.' << st._date_year << "\t+" << st._number << '\n';
    return(out);
}





class Group {
    Student * _students;
    int _course; // 1-6
    int _flow; // 1-3
    int _size;

public:
    friend std::ostream& operator<< (std::ostream &out, const Group &gr);

    Group(int c=2, int f=3, int s=0):
    _course(c), _flow(f), _size(s) {_students = new Student [s];}

    Group(Student * sts, int c=2, int f=3, int s=5):
    _course(c), _flow(f), _size(s)
    {   
        int i = 0;
        _students = new Student [s];
        for(i=0; i<_size; i++)
            _students[i] = sts[i];
    }

    void AddSt(Student s)
    {
        int i;
        for (i=0; i<_size; i++)
            if (strcmp(s._name, _students[i]._name) == 0) return;
        _students = (Student *)realloc(_students, sizeof(Student)*(_size+2));
        _students[_size]=s;
        _size+=1;           //cout << g._size;
    }

    void RemoveSt(const char * name)
    {
        int i;
        for (i=0; i<_size; i++)
            if (strcmp(name, _students[i]._name) == 0)
            {
                for (; i<_size-1; i++) _students[i]=_students[i+1];
                Student tmp;    _students[--_size] = tmp;   return;
            }
        cout << "\t Student for removal " << name << " not found! \n";         
    }

    void ChangeSt(Student s)
    {
        int i;
        for (i=0; i<_size; i++)
            if (strcmp(s._name, _students[i]._name) == 0)
            {
                _students[i]._number = s._number;     _students[i]._date_day = s._date_day;     _students[i]._date_month = s._date_month;     _students[i]._date_year = s._date_year;
                cout << "\t Student " << s._name << " found and changed! \n";   return;
            }
        cout << "\t Student " << s._name << " not found! \n";  return;
    } 

    void NameSort()
    {
        int i,j;
        for (i=0; i<_size; i++)
            for (j=0; j<i; j++)
                if (strcmp(_students[i]._name, _students[j]._name) < 0) StSwap(_students[i], _students[j]);
        cout << "NameSort done. \n";
    }

    void DateSort()
    {
        int i,j;
        for (i=0; i<_size; i++)
            for (j=0; j<i; j++)
                if (_students[i]._date_year < _students[j]._date_year) StSwap(_students[i], _students[j]);  
                else if ((_students[i]._date_year == _students[j]._date_year) and (_students[i]._date_month < _students[j]._date_month)) StSwap(_students[i], _students[j]);
                     else if ((_students[i]._date_month == _students[j]._date_month) and (_students[i]._date_day < _students[j]._date_day)) StSwap(_students[i], _students[j]); 
        cout << "DateSort done. \n";
    }

    void NumberSort()
    {
        int i,j;
        for (i=0; i<_size; i++)
            for (j=0; j<i; j++)
                if (_students[i]._number < _students[j]._number) StSwap(_students[i], _students[j]);
        cout << "NumberSort done. \n";
    }    

    Student NameSeek(const char * name) const
    {
        int i;
        for (i=0; i<_size; i++)
            if (strcmp(name, _students[i]._name) == 0)
            { cout << "\t Student found! \n" << _students[i]; return(_students[i]); }
        cout << "\t Student " << name << " not found! \n";
        Student st;    return(st);
    }

    Student DateSeek(int day, int month, int year) const
    {
        int i;
        for (i=0; i<_size; i++)
            if (day == _students[i]._date_day)
                if (month == _students[i]._date_month)
                    if (year == _students[i]._date_year)
                        { cout << "\t Student found! \n" << _students[i]; return(_students[i]); }
        cout << "\t Student with such date not found! \n";
        Student st;    return(st);
    }

    Student NumberSeek(long number) const
    {
        int i;
        for (i=0; i<_size; i++)
            if (number == _students[i]._number)
                { cout << "\t Student found! \n" << _students[i]; return(_students[i]); }
        cout << "\t Student with such number not found! \n";
        Student st;    return(st);
    }

    ~Group() {delete [] _students;}

};

std::ostream& operator<< (std::ostream &out, const Group &gr)
{   
    int i;
    for(i=0; i<gr._size; i++)
        out << gr._students[i];
    return(out);
}

void menu()
{
    cout << "\x1b[31mФункционал команд: \x1b[0m\n 0. Вызов этого меню: menu \n 1. Просмотр группы: see \n 2. Добавить участника add ";
    cout << "\n 3. Редактировать участника: change \n 4. Удалить участника: remove \n 5-6. Сортировка по имени/телефону/дате рождения: sortn, sortp, sortb \n 7-9. Сортировка по имени/телефону/дате рождения: findn, findp, findb \n 10. Завершение работы программы: end\n";
}





int main()
{
    int i;
    Student st0("БАЛАНОВСКИЙ", 89266000000, 11, 5, 2001),
            st1("ГОНЧАРЕНКО", 89102000000, 4, 6, 2000),
            st2("ОКОНИШНИКОВ", 89853000000, 2, 4, 2002),
            st3("ГЛАДЫШЕВ", 89152000000, 10, 10, 2001);
    //cout << st0 << st1 << st2 << st3;
    Group gr214(2,3,0); gr214.AddSt(st0); gr214.AddSt(st1);  gr214.AddSt(st2);  gr214.AddSt(st3);

    cout << "\n\t \x1b[31mДобро пожаловать в редактор Группы 214! \n     Первоначально в группе 4 участника, с заполненными полями имени, номера и даты рождения соответственно. \x1b[0m\n"; menu();
    for(;;)
    {
        char str[256];  long phone;  int year, day, month;
        cout << "\x1b[31mВведите команду: \x1b[0m";
        std::cin >> str;

        if (!strcmp("see", str)) cout << gr214;
        else if (!strcmp("menu", str)) menu();
        else if (!strcmp("sortn", str)) gr214.NameSort();
        else if (!strcmp("sortb", str)) gr214.DateSort();
        else if (!strcmp("sortp", str)) gr214.NumberSort();
        else if (!strcmp("findn", str)) {cout << " -> Введите имя: "; std::cin >> str; gr214.NameSeek(str);}
        else if (!strcmp("findb", str)) {cout << " -> Введите дату рождения: Год: "; std::cin >> year; cout << " -> Месяц: "; std::cin >> month; cout << " -> День: "; std::cin >> day; gr214.DateSeek( day, month, year);}
        else if (!strcmp("findp", str)) {cout << " -> Введите номер телефона: "; std::cin >> phone; gr214.NumberSeek(phone);}
        else if (!strcmp("add", str)) {cout << " -> Введите имя: "; std::cin >> str; cout << " -> Введите номер телефона: "; std::cin >> phone; cout << " -> Введите дату рождения: Год: "; std::cin >> year; cout << " -> Месяц: "; std::cin >> month; cout << " -> День: "; std::cin >> day; Student st(str, phone, day, month, year); gr214.AddSt(st);}
        else if (!strcmp("change", str)) {cout << " -> Введите имя: "; std::cin >> str; cout << " -> Введите номер телефона: "; std::cin >> phone; cout << " -> Введите дату рождения: Год: "; std::cin >> year; cout << " -> Месяц: "; std::cin >> month; cout << " -> День: "; std::cin >> day; Student st(str, phone, day, month, year); gr214.ChangeSt(st);}
        else if (!strcmp("remove", str)) {cout << " -> Введите имя: "; std::cin >> str; gr214.RemoveSt(str);}
        else if (!strcmp("end", str)) { cout << "Ending!! \n"; break;}
        else cout << "No such cmd \n";
    }
    return(0);
}