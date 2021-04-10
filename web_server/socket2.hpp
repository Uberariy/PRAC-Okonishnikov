#ifndef socket
#define socket

#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

class Error {
    string _error_type;
public:
    Error(string errtype) : _error_type(errtype) {}
    string GetErr() { return(_error_type); }
};

struct SocketAddress {
    int _sd;
    struct sockaddr_in _sock_addr;

    SocketAddress ()
    {
        if ((_sd = socket (AF_INET, SOCK_STREAM, 0)) < 0) 
            throw Error("socket");
    }
    SocketAddress (int IP_addr, short port) 
    {
        if ((_sd = socket (AF_INET, SOCK_STREAM, 0)) < 0) 
            throw Error("socket");
        memset(&_sock_addr, 0, sizeof(_sock_addr));
        _sock_addr.sin_family = AF_INET;
        _sock_addr.sin_port = port;
        _sock_addr.sin_addr.s_addr = IP_addr; 
        // INADDR_LOOPBACK (127.0.0.1) or INADDR_ANY (0.0.0.0) or INADDR_BROADCAST (255.255.255.255)   
    }
    SocketAddress (const char * IP_addr, short port)
    {
        if ((_sd = socket (AF_INET, SOCK_STREAM, 0)) < 0) 
            throw Error("socket");
        memset(&_sock_addr, 0, sizeof(_sock_addr));
        _sock_addr.sin_family = AF_INET;
        _sock_addr.sin_port = port;
        _sock_addr.sin_addr.s_addr = inet_addr(IP_addr); 
        // INADDR_LOOPBACK (127.0.0.1) or INADDR_ANY (0.0.0.0) or INADDR_BROADCAST (255.255.255.255)      
    }
    int GetAddrlen() { return(sizeof(_sock_addr)) ;}
    int GetSd() { return(_sd); }
    ~SocketAddress() { close(_sd); }
};

class Socket {
protected:
    SocketAddress _myaddr;
public:
    Socket() : _myaddr() {}
    Socket(int IP_addr, short port) : _myaddr(IP_addr, port) {}  // Добавить конструктор с const char *
    Socket(const char * IP_addr, short port) : _myaddr(IP_addr, port) {}
};

class ServerSocket : public Socket {
public:
    ServerSocket(int IP_addr, short port) : Socket(IP_addr, port)
    {cout << _myaddr._sd;}
    int _Listen(int queue)
    {
        int res = listen(_myaddr._sd, queue);
        if (res < 0)
            throw Error("Listen");
        return(res);
    }
    int _Bind()
    {   cout << _myaddr._sd;
        int res = bind(_myaddr._sd, (struct sockaddr *) &_myaddr._sock_addr, _myaddr.GetAddrlen());
        if (res < 0)
            throw Error("Bind");
        return(res);
    }
    int _Accept(SocketAddress& ClientAddr)   // This func. is used by a server.
    {
        socklen_t pointlen = ClientAddr.GetAddrlen();
        int res = accept(_myaddr._sd, (struct sockaddr *) &ClientAddr._sock_addr, &pointlen);
        return(res);
    }
 
};

class ConnectedSocket : public Socket {
public:
    ConnectedSocket() : Socket() {}
    ConnectedSocket(int IP_addr, short port) : Socket(IP_addr, port)
    {}
    ConnectedSocket(int sd) : Socket() { _myaddr._sd = sd; }
    string _Read(int flags)
    {
        int size = 0;
        recv(_myaddr._sd, &size, sizeof(int), 0);
        vector<char> tmp;
        tmp.resize((int) (size / sizeof(std::vector<char>)));
        recv(_myaddr._sd, &tmp[0], size, flags);
        string str(tmp.begin(), tmp.end());
        return(str);
    }
    void _Write(SocketAddress& ServAddr, string str, int flags)
    {
        std::vector<char> tmp(str.begin(), str.end());
        int size = tmp.size() * sizeof(tmp);
        send(ServAddr._sd, &size, sizeof(int), 0);
        send(ServAddr._sd, tmp.data(), tmp.size(), 0);
    }
};

class ClientSocket : public ConnectedSocket {
public:
    ClientSocket() : ConnectedSocket() {}
    ClientSocket(int IP_addr, short port) : ConnectedSocket(IP_addr, port)
    {}
    int _Connect(SocketAddress& ServAddr)  // This func. is used by a client.
    {
        int res = connect(ServAddr._sd, (struct sockaddr *) &ServAddr._sock_addr, ServAddr.GetAddrlen());
        if (res < 0)
            throw Error("Connect");
        return(res);       
    }    
};

class HttpHeader {
    string _name;
    string _value;
public:
    HttpHeader (const string &n, const string &v) : _name(n), _value(v) {}
    explicit HttpHeader(const string& line) {}
};

#endif

// \r\n - разделители строк в хедерах