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
        if (_sd = socket (AF_INET, SOCK_STREAM, 0) < 0) 
            throw Error("socket");
    }
    SocketAddress (uint32_t IP_addr, short port) 
    {
        if (_sd = socket (AF_INET, SOCK_STREAM, 0) < 0) 
            throw Error("socket");
        _sock_addr.sin_family = AF_INET;
        _sock_addr.sin_port = port;
        _sock_addr.sin_addr.s_addr = htonl(IP_addr); 
        // INADDR_LOOPBACK (127.0.0.1) or INADDR_ANY (0.0.0.0) or INADDR_BROADCAST (255.255.255.255)      
    }
    SocketAddress (const char * IP_addr, short port)
    {
        if (_sd = socket (AF_INET, SOCK_STREAM, 0) < 0) 
            throw Error("socket");
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
    int _sd;
    struct sockaddr_in _sock_addr;
public:
    Socket()
    {
        if (_sd = socket (AF_INET, SOCK_STREAM, 0) < 0) 
            throw Error("socket");
    }
    Socket(uint32_t IP_addr, short port)
    {
        if (_sd = socket (AF_INET, SOCK_STREAM, 0) < 0) 
            throw Error("socket");
        memset(&_sock_addr, 0, sizeof(_sock_addr));
        _sock_addr.sin_family = AF_INET;
        _sock_addr.sin_port = port;
        _sock_addr.sin_addr.s_addr = htonl(IP_addr); 
        // INADDR_LOOPBACK (127.0.0.1) or INADDR_ANY (0.0.0.0) or INADDR_BROADCAST (255.255.255.255)      
    }
    Socket(const char * IP_addr, short port)
    {
        if (_sd = socket (AF_INET, SOCK_STREAM, 0) < 0) 
            throw Error("socket");
        memset(&_sock_addr, 0, sizeof(_sock_addr));
        _sock_addr.sin_family = AF_INET;
        _sock_addr.sin_port = port;
        _sock_addr.sin_addr.s_addr = inet_addr(IP_addr); 
        // INADDR_LOOPBACK (127.0.0.1) or INADDR_ANY (0.0.0.0) or INADDR_BROADCAST (255.255.255.255)      
    }
    int GetAddrlen() { return(sizeof(_sock_addr)) ;}
    int GetSd() { return(_sd); }
    ~Socket() { close(_sd); }
};

class ServerSocket : public Socket {
public:
    ServerSocket(uint32_t IP_addr, short port) : Socket(IP_addr, port)
    {cout << port;  cout << _sock_addr.sin_port;}
    int _Listen(int queue)
    {
        int res = listen(_sd, queue);
        if (res < 0)
            throw Error("Listen");
        return(res);
    }
    int _Bind()
    {   cout << _sock_addr.sin_port;
        int res = bind(_sd, (struct sockaddr *) &_sock_addr, GetAddrlen());
        if (res < 0)
            throw Error("Bind");
        return(res);
    }
    int _Accept(SocketAddress& ClientAddr)   // This func. is used by a server.
    {
        socklen_t pointlen = ClientAddr.GetAddrlen();
        int res = accept(_sd, (struct sockaddr *) &ClientAddr._sock_addr, &pointlen);
        return(res);
    }
 
};

class ConnectedSocket : public Socket {
public:
    ConnectedSocket() : Socket() {}
    ConnectedSocket(uint32_t IP_addr, short port) : Socket(IP_addr, port)
    {}
    ConnectedSocket(int sd) : Socket() { _sd = sd; }
    string _Read(int flags)
    {
        int size = 0;
        recv(_sd, &size, sizeof(int), 0);
        vector<char> tmp;
        tmp.resize((int) (size / sizeof(std::vector<char>)));
        recv(_sd, &tmp[0], size, flags);
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
    ClientSocket(uint32_t IP_addr, short port) : ConnectedSocket(IP_addr, port)
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