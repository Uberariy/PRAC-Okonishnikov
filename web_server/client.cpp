#include <cmath>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket2.hpp"
using namespace std;

#define DEFAULTPORT 3666
int port;

class Client {
    ClientSocket _Client;
    SocketAddress _Serv;
public:
    Client(int port) : _Serv(INADDR_LOOPBACK, port), _Client() {}
    
    void Run()
    {
        _Client._Connect
    }
};



int main(int argc, char **argv)
{
    if(argc == 2) port = atoi(argv[1]);
    else port = DEFAULTPORT;

    Client client(port);
    client.Run();

    return(0);
}