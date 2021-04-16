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
#include <fcntl.h>
#include <errno.h>
#include "socket2.hpp"
using namespace std;

#define BUFSIZE 128

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
    string _method;
    string _uri_way;
    string _uri_par;
    string _version;
    string _body;

public:
    HttpRequest(string request) 
    {
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
                _uri_way = uri.substr(1, pos2);
                uri.erase(0, pos2+1);
                _uri_par = uri;
            }
            else 
            {   
                _uri_way = uri.substr(0, uri.size()-1);
                _uri_par = " ";
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
};

std::ostream &operator<<(std::ostream &out, HttpRequest &r)
{
    out << "method: \t" << r._method << "\nuri_way: \t" << r._uri_way << "\nuri_par: \t" << r._uri_par << "\nversion: \t" << r._version << "\n\n";
    return out;
}

class HttpResponse {
    string _answer;
    string _headers;
    string _code;
    string _body;

public:
    HttpResponse(HttpRequest & request, ConnectedSocket & cs) :  _headers(""), _code(""), _body("")
    {   
        //cout << "|" << request._uri_way.c_str() << "|";
        int fd = open(request._uri_way.c_str(), O_RDONLY);
        if (fd == -1) 
        {   
            if (errno == EACCES)
                _code = "403 NotAccessable";
            else 
                _code = "404 NotFound";
        }
        else _code = "200 Okay";

        if ((request._method != "HEAD") && (request._method != "GET"))
        {
            _code = "501 NotImplemented";
            HttpHeader H_Allow("Allow", "GET, HEAD");
            _headers += H_Allow.Return() + "\n\n";
        }
        _answer = "HTTP/1.1 " + _code + "\n\n";
        _answer += "Code: " + _code + "\n";

        HttpHeader H_Server("Server", "Model HTTP Uberariy");
        _headers += H_Server.Return() + "\n";

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
        _headers += H_ContentLength.Return() + "\n";
        _answer += _headers + "\n";

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

//http://127.0.0.1:1234
//http://127.0.0.1:1234/cgi-bin/testcgi?name=igor&surname=golovin&mail=igolovin

#endif