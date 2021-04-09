#ifndef socket
#define socket

#include <cmath>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

class Error {
    string _error_type;
public:
    Error(string errtype) : _error_type(errtype) {}
};

class Socket {
protected:
    int _sd;
    struct sockaddr_in _sock_addr;
public:
    Socket(uint32_t IP_addr, short port)
    {
        if (_sd = socket (AF_INET, SOCK_STREAM, 0) < 0) 
            throw Error("socket");
        _sock_addr.sin_family = AF_INET;
        _sock_addr.sin_port = port;
        _sock_addr.sin_addr.s_addr = htonl(IP_addr);             
        // INADDR_LOOPBACK (127.0.0.1) or INADDR_ANY (0.0.0.0) or INADDR_BROADCAST (255.255.255.255)
    }
    int GetAddrlen() { return(sizeof(_sock_addr));}
    int GetSd() { return(_sd); }
};

class ServerSocket : public Socket {
public:
    ServerSocket(uint32_t IP_addr, short port) : Socket(IP_addr, port)
    {}
    int _Listen(int queue)
    {
        int res = listen(GetSd(), queue);
        if (res < 0)
            throw Error("listen");
        return(res);
    }
    int _Bind()
    {
        int res = bind(_sd, (struct sockaddr *) &_sock_addr, GetAddrlen());
        if (res < 0)
            throw Error("bind");
        return(res);
    }
    int _Connect()  // This func. is used by a client.
    {
        int res = connect(_sd, (struct sockaddr *) &_sock_addr, GetAddrlen());
        if (res < 0)
            throw Error("connect");
        return(res);       
    }
};

class ConnectedSocket : public Socket {
public:
    ConnectedSocket(uint32_t IP_addr, short port) : Socket(IP_addr, port)
    {}
    int _Accept(int serv)   // This func. is used by a server.
    {
        socklen_t pointlen = GetAddrlen();
        return(accept(serv, (struct sockaddr *) &_sock_addr, &pointlen));
    }
};

class ClientSocket : public ConnectedSocket {
public:
    ClientSocket(uint32_t IP_addr, short port) : ConnectedSocket(IP_addr, port)
    {}
    // Add read and write;
};

#endif