#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket2.hpp"
using namespace std;

#define DEFAULTPORT 1234
int port;

class Client {
    ClientSocket _Client;
    SocketAddress _Serv;
    int _exit;
public:
    Client(int port) : _Serv(htonl(INADDR_LOOPBACK), port), _Client(), _exit(0) {}
    
    void Run()
    {
        try { _Client._Connect(_Serv); }
        catch(Error E) {
            cerr << "Client: " << E.GetErr() << " error! (Server does not exist, or address is wrong)\n";
            exit(2);
        };
        sleep(1);
        cout << "Client: Connected to server" << INADDR_LOOPBACK << ":" << port << "!\n"; 
        string line;
        for(;;)
        {
            std::getline(std::cin, line);   //cout << "|" << line << "|";
            _Client._Write(_Serv, line, 0);
            _Client._Write(_Serv, "\r\n", 0);
            if (line == "Disconnect") 
            {
                cout << "Client: Client Disconected\n";
                _exit = 1;
            }
            if (line == "Close")
            {
                cout << "Client: Closing Server, Client Disconected\n";
                _exit = 1;
            }
            if (_exit) break;
        }
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