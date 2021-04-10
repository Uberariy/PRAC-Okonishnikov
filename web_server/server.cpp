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

class Server {
    ServerSocket _Serv;
    int _accept_err;
    int _queue;
public:
    Server (int port, int queue) : _Serv(INADDR_LOOPBACK, port), _queue(queue), _accept_err(0)
    {
        try { _Serv._Bind();    _Serv._Listen(_queue); } 
        catch(Error E) {
            cerr << "Server: " << E.GetErr() << " error!\n";
            exit(2);
        };
    }

    void ProcessConnection(int clientsd, SocketAddress & Client)
    {
        ConnectedSocket cs(clientsd);
        string request = cs._Read(0);
    }

    void Run ()
    {
        for(;;)
        {
            SocketAddress Client;
            int clientsd;
            if (clientsd = _Serv._Accept(Client)) {
                cerr << "Server: Accept Error!\n";
                _accept_err++;
                if (_accept_err == 1)
                {
                    cerr << "Server: Accept Error is Fatal!\n";
                    exit(3);
                }
                else
                {
                    cerr << "Server: Accept Error Tolerated!\n";
                }
            };
            _accept_err = 0;
            cout << "Server: Client " << clientsd << " connected!\n";
            ProcessConnection(clientsd, Client);
        }
    }

};


int main(int argc, char **argv)
{
    if(argc == 2) port = atoi(argv[1]);
    else port = DEFAULTPORT;

    Server server(port, 5);
    server.Run();

    return(0);
}
