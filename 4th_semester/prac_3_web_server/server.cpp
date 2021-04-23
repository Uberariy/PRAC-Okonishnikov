#include <cmath>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket2.hpp"
#include "http.hpp"
using namespace std;

#define DEFAULTPORT 1234
int port;

class Server {
    ServerSocket _Serv;
    int _accept_err;
    int _queue;
    int _exit;
    int _next;
public:
    Server (int port, int queue) : _Serv(htonl(INADDR_ANY), port), _queue(queue), _accept_err(0), _exit(0), _next(0)
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
        for(;;)
        {
            string line = cs._Read(0);
            //cout << "|" << line << "|";
            if (line == "Disconnect") 
            {
                cerr << "Server: Client Disconected!\n";
                _next = 1;
            }
            else if (line == "Close")
            {
                cerr << "Server: Server Closed!\n";\
                _exit = 1;
            }
            else if (!line.empty())
            {
                HttpRequest prekl(line);    
                cout << prekl;
                HttpResponse lol(prekl, cs);
                _next = 1; 
            }
            line.clear();
            if(_exit || _next) { cs._Shutdown(); break; }
        }
    }

    void Run ()
    {
        for(;;)
        {
            SocketAddress Client;
            int clientsd = _Serv._Accept(Client);
            if (clientsd < 0) {
                if (_accept_err == 1)
                {
                    cerr << "Server: Accept Error is Fatal! (happened twice)\n";
                    exit(3);
                }
                else    // if zero
                {
                    cerr << "Server: Accept Error is Tolerated! (happened once)\n";
                    _accept_err++;
                    continue;
                }
            }
            else _accept_err = 0;
            cout << "Server: Client " << clientsd << " connected!\n";
            ProcessConnection(clientsd, Client);
            if (_exit) break;
        }
    }

};

int HttpRequest::_numb = 0;
int main(int argc, char **argv)
{
    if(argc == 2) port = atoi(argv[1]);
    else port = DEFAULTPORT;

    Server server(port, 5);
    server.Run();

    return(0);
}
