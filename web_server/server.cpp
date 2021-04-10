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
    int _exit;
public:
    Server (int port, int queue) : _Serv(htonl(INADDR_ANY), port), _queue(queue), _accept_err(0), _exit(0)
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
        string line = cs._Read(0);
        if (line == "Disconnect") 
        {
            cerr << "Server: Client Disconected!\n";
        }
        if (line == "Close")
        {
            cerr << "Server: Server Closed!\n";\
            _exit = 1;
        }
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
                    cerr << "Server: Accept Error is Tolerated!\n";
                }
            };
            _accept_err = 0;
            cout << "Server: Client " << clientsd << " connected!\n";
            ProcessConnection(clientsd, Client);
            if (_exit) break;
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
