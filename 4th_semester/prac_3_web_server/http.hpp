#ifndef http
#define http

#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <ctime>
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
#include "socket2.hpp"
using namespace std;

#define BUFSIZE         8192
#define REQUEST         "REQUEST="
#define SERVER_ADDR     "SERVER_ADDR=127.0.0.1"
#define CONTENT_TYPE    "CONTENT_TYPE=text/plain"
#define SERVER_PROTOCOL "SERVER_PROTOCOL=HTTP/1.1"
#define SCRIPT_NAME     "SCRIPT_NAME="
#define SERVER_PORT     "SERVER_PORT=1234"
#define QUERY_STRING    "QUERY_STRING="

class HttpHeader {
    string _name;
    string _value;
public:
    HttpHeader (const string &n, const string &v) : _name(n), _value(v) 
    {}
    explicit HttpHeader(const string& line) 
    {
        string str = line;
        int pos = str.find(":");
        _name = str.substr(0, pos);
        str.erase(0, pos+1);    
        _value = str; 
    }
    string Return()
    {
        string str = _name;
        str += ": ";
        str += _value;
        return(str);
    }
    friend class ConnectedSocket;
};

class HttpRequest {
    string _fullrequest;
    string _method;
    string _uri_way;
    string _uri_par;
    string _version;
    string _body;
    int _cgi;

public:
    static int _numb;
    HttpRequest(string request) 
    {
        _numb++;
        string _fullrequest = request;
        string str = request;
        int pos = str.find(" ");
        _method = str.substr(0, pos);
        // if not GET or HEAD => Diagnos
        str.erase(0, pos+1);
        pos = str.find(" ");
            string uri = str.substr(1, pos);
            int pos2 = uri.find("?");
            if (pos2 != -1)
            {
                _uri_way = uri.substr(0, pos2);
                uri.erase(0, pos2+1);
                _uri_par = uri;
                _cgi = 1;
            }
            else 
            {   
                _uri_way = uri.substr(0, uri.size()-1);
                _uri_par = " ";
                _cgi = 0;
            }
        str.erase(0, pos+1);
        pos = str.find("/");
        str.erase(0, pos+1);
        pos = str.find("\n");
        _version = str.substr(0, pos);
        // if not 1.0 or 1.1 => Diagnos
    }

    friend std::ostream &operator<<(std::ostream &out, HttpRequest &r); 
    friend class HttpResponse;
    friend char** NewEnv(HttpRequest & request);
};

std::ostream &operator<<(std::ostream &out, HttpRequest &r)
{
    out << "[" << r._numb << "]\nmethod: \t" << r._method << "\nuri_way: \t" << r._uri_way << "\nuri_par: \t" 
        << r._uri_par << "\nversion: \t" << r._version << "\niscgi?: \t" << r._cgi << "\n\n";
    return out;
}

char** NewEnv(HttpRequest & request)
{
    char ** env = new char*[8];
    env[0] = new char [request._fullrequest.size()];
    env[1] = new char[22]; //SERVER_ADDR
    env[2] = new char[17]; //SERVER_PORT
    env[3] = new char[24]; //CONTENT_TYPE
    env[4] = new char[25]; //SERVER_PROTOCOL
    env[5] = new char[13 + request._uri_way.size()]; //SCRIPT_NAME
    env[6] = new char[14 + request._uri_par.size()]; //QUERY_STRING
    env[7] = NULL;

    env[0] = (char *) request._fullrequest.c_str();
    env[1] = (char *) SERVER_ADDR;
    env[2] = (char *) SERVER_PORT;
    env[3] = (char *) CONTENT_TYPE;
    env[4] = (char *) SERVER_PROTOCOL;
    strcpy(env[5], SCRIPT_NAME);            
    strcat(env[5], request._uri_way.c_str());
    strcpy(env[6], QUERY_STRING);           
    strcat(env[6], request._uri_par.c_str());

    return env;
}

class HttpResponse {
    string _answer;
    string _headers;
    string _code;
    string _body;

public:
    HttpResponse(HttpRequest & request, ConnectedSocket & cs) :  _headers(""), _code(""), _body("")
    {
        int fd;
        if (request._cgi)   // CGI
        {
            pid_t pid;
            if ((pid = fork()) > 0)
            {
                int stat;
                wait(&stat);
                if (!(WIFEXITED(stat) && WEXITSTATUS(stat)))
                {
                    fd = open("tmp.txt", O_RDONLY);
                    _code = "200 Okay";
                }
            }
            else if (pid == 0)
            {
                fd = open("tmp.txt", O_WRONLY|O_TRUNC|O_CREAT, 0777);
                if (fd < 0) { perror("tmp.txt"); exit(1); }
                dup2(fd, 1);
                close(fd);
                char * argv[] = { (char*)request._uri_way.c_str(), NULL };
                char ** env = NewEnv(request);
                execvpe(request._uri_way.c_str(), argv, env);
                perror("execve");
                exit(2);
            }
        }
        else // NOT CGI
        {
            //cout << "|" << request._uri_way.c_str() << "|";
            fd = open(request._uri_way.c_str(), O_RDONLY);
            if (fd == -1) 
            {   
                if (errno == EACCES)
                    _code = "403 NotAccessable";
                else 
                {
                    fd = open("index/404.html", O_RDONLY);
                    _code = "404 NotFound";
                }
            }
            else _code = "200 Okay";
        }

        if ((request._method != "HEAD") && (request._method != "GET"))
        {
            _code = "501 NotImplemented";
            HttpHeader H_Allow("Allow", "GET, HEAD");
            _headers += H_Allow.Return() + "\n\n";
        }
        _answer = "HTTP/1.1 " + _code + "\r\n";

        HttpHeader H_Server("Server", "Model HTTP Uberariy");
        _headers += H_Server.Return() + "\r\n";

        time_t timer = time(nullptr);
        HttpHeader H_Date("Date", (string)asctime(localtime(&timer)));
        _headers += H_Date.Return();

        char c;         string tmp;
        int len = 0;
        if (fd >= 0)
        {
            while(read(fd, &c, 1)) len++;
            lseek(fd, 0, 0);
        }
        HttpHeader H_ContentLength("Content-Length", to_string(len));
        _headers += H_ContentLength.Return() + "\r\n";
        _answer += _headers + "\r\n";

        cout << _answer;
        cs._Write(_answer, 0);

        if ((request._method == "GET") && (fd >= 0))
        {
            len = 0;
            char buf[BUFSIZE];
            while ((len = read(fd, buf, BUFSIZE)) > 0)
            {
                cs._Write(buf, len, 0);
            }
        }
        close(fd);
    }
};

//GET / HTTP/1.1
//GET /cgi-bin/testcgi?name=igor&surname=golovin&mail=igolovin HTTP/1.1

//http://127.0.0.1:1234/index/200.html
//http://127.0.0.1:1234/index/404.html
//http://127.0.0.1:1234/index/cgi-bin/cgi?name=igor&surname=golovin&mail=igolovin

#endif