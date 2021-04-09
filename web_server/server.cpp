#include <cmath>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.hpp"
using namespace std;

#define DEFAULTPORT 3666
int port;

class Server {
    ServerSocket _Serv;
    int _queue;
public:
    Server (int port, int queue) : _Serv(INADDR_LOOPBACK, port), _queue(queue)
    {
        try { _Serv._Bind();    _Serv._Listen(_queue); } 
        catch(Error E) {
            cerr << "Server: " << E.GetErr() << " error!\n";
            exit(2);
        };
    }

    void Run ()
    {
        while(1)
        {
            ConnectedSocket Client;
            int clientsd;
            if (clientsd = Client._Accept(_Serv.GetSd())) {
                cerr << "Server: Accept Error!\n";
                exit(3);
            };
            cout << "Server: Client " << clientsd << " connected!\n";
        }
    }

};


int main(int argc, char **argv)
{
    if(argc == 2) port = atoi(argv[1]);
    else port = DEFAULTPORT;

    Server server(port, 16);
    server.Run();

    return(0);
}
