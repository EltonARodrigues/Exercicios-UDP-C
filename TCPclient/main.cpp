/*
 Client UDP
 Elton de Andrade Rodrigues RA: 00079714
 */
#define LINUX

#ifdef WINDOWS
#include <winsock.h>
#include <windows.h>
#endif

#ifdef LINUX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket(n) close(n)
#define SOCKET socklen_t
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#define DEFAULT_PORT 64321
#define IPADDRESS "127.0.0.1"
#define MAX_SIZE 1024

using namespace std;

/*
 *
 */
int main(int argc, char** argv)
{
    char Buffer[MAX_SIZE];
    int retval;
#ifdef WINDOWS
    int fromlen;
#else
    socklen_t fromlen;
#endif

    struct sockaddr_in proxy, client;
    SOCKET connect_socket;

#ifdef WINDOWS
    WSADATA wsaData;

    if ((retval = WSAStartup(0x202, &wsaData)) != 0)
    {
        cout << "Server: WSAStartup() failed with error: " << retval << endl;
        WSACleanup();
        return -1;
    }
    else
        cout << "Server: WSAStartup() is OK." << endl;
#endif

    proxy.sin_family = AF_INET;
    proxy.sin_addr.s_addr = inet_addr(IPADDRESS);
    proxy.sin_port = htons(DEFAULT_PORT);

    connect_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (connect_socket < 0)
    {
        cout << "Client: socket() failed with error: " << connect_socket << endl;
        exit(-1);
    }
    else
        cout << "Client: socket() is OK." << endl;

    if (connect(connect_socket, (struct sockaddr*) &proxy, sizeof (proxy)) < 0)
    {
        cout << "Client: connect() failed\n" << endl;
        exit(-2);
    }
    else
        cout << "Client: connect() is OK." << endl;


    cin.getline(Buffer, MAX_SIZE);
    retval = send(connect_socket, Buffer, sizeof (Buffer), 0);
    if (retval <= 0)
    {
        cout << "Client: send() failed: error: " << endl;
        closesocket(connect_socket);
        exit(-3);
    }
    else
        cout << "Client: send() is OK." << endl;

    retval = recv(connect_socket, Buffer, sizeof (Buffer), 0);
    if (retval <= 0)
    {
        cout << "Client: recv() failed" << endl;
        exit(-4);
    }
    else
        cout << "Client: recv() is OK." << endl;

    cout << "Data received: " << Buffer << endl;
    closesocket(connect_socket);
#ifdef LINUX
    sleep(5);
#endif

    exit(0);
}
