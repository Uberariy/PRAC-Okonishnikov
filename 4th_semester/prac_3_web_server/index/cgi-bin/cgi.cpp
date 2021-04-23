#include <cmath>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main() {
    //std::cout << "Content-type: text/html\n\n";
    cout << "<html><body>\n"       << "[CGI Program Interface]\r\n";

    cout << "QUERY_STRING: "       << getenv("QUERY_STRING") << "\r\n"; 
    cout << "SERVER_ADDR: "        << getenv("SERVER_ADDR") << "\r\n";
    cout << "SERVER_PORT: "        << getenv("SERVER_PORT") << "\r\n";
    cout << "SCRIPT_NAME: "        << getenv("SCRIPT_NAME") << "\r\n";   
    cout << "SERVER_PROTOCOL: "    << getenv("SERVER_PROTOCOL") << "\r\n";  
    
    cout << "</body></html>\n";
    return(0);
}