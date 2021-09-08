#include <cmath>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

int main()
{
        time_t timer = time(nullptr);
        cout << (string)asctime(localtime(&timer));
}