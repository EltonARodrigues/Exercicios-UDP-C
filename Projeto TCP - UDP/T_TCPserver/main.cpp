/*

 Servidor TCP

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

void strupr(char str[])
{
    int indx = 0;
    for (; indx < MAX_SIZE; indx++)
    {
        if (str[indx] <= 122 && str[indx] >= 97)
            str[indx] = (char)(((int)str[indx]) - 32);
    }
}

int main(int argc, char **argv)
{
    char Buffer[MAX_SIZE];
    int retval;
#ifdef WINDOWS
    int fromlen;
#else
    socklen_t fromlen;
#endif

    struct sockaddr_in local, from;
    SOCKET listen_socket, msgsock;

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

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(IPADDRESS);
    local.sin_port = htons(DEFAULT_PORT);

    listen_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (listen_socket < 0)
    {
        cout << "Server: socket() failed with error: " << listen_socket << endl;
        exit(-1);
    }
    else
        cout << "Server: socket() is OK." << endl;

    if (bind(listen_socket, (struct sockaddr*) &local, sizeof (local)) < 0)
    {
        cout << "Server: bind() failed with error: " << endl;
        exit(-2);
    }

    cout << "Server: bind() is OK.";

    if (listen(listen_socket, 5) < 0)
    {
        cout << "Server: listen() failed with error: " << endl;
        exit(-3);
    }
    else
        cout << "Server: listen() is OK" << endl;

    cout << "Server: listening and waiting connection on port " << DEFAULT_PORT << endl;

    while (1)
    {
        memset(Buffer,0x0,MAX_SIZE);

        fromlen = sizeof (from);

        msgsock = accept(listen_socket,
                         (struct sockaddr*) &from,
                         &fromlen);
        if (msgsock < 0)
        {
            cout << "Server: accept() error: " << msgsock << endl;
            exit(-4);
        }
        else
            cout << "Server: accept() is OK" << endl;

        cout << "Server: accepted connection from " << inet_ntoa(from.sin_addr);
        cout << " Port: " << htons(from.sin_port) << endl;

        retval = recv(msgsock, Buffer, sizeof (Buffer), 0);

        if (retval < 0)
        {
            cout << "Server: No bytes received" << endl;
            closesocket(msgsock);
            continue;
        }
        else
            printf("Server: recv() is OK.\n");

        if (retval == 0)
        {
            printf("Server: Client closed connection.\n");
            closesocket(msgsock);
            continue;
        }


        cout << "Server: Received " << retval << " bytes" << endl;
        cout << "Data: " << Buffer << endl;

        //codigo para inverter string
        char auxiliar[50];
        int a,b;

        b = strlen(Buffer)-1;
        //strlen calcula a quantidade de caracteres que tem a string
        for(a=0;Buffer[a]!='\0';a++){        //Repete enquanto nao chegar ao final da string
        auxiliar[b]=Buffer[a];
        b--;
        }
        auxiliar[a]='\0';      //Se nao colocar essa parte, o programa pode mostrar LIXO
        strcpy(Buffer,auxiliar);    //Copia para a variável string o conteúdo da variável auxiliar
        printf("\nResposta: %s\n",Buffer);
        // fim do codigo de inversao de string

        printf("Server: Enviando nova string para o proxy...\n");
        strupr(Buffer);

        retval = send(msgsock, Buffer, sizeof (Buffer), 0);



        if (retval < 0)
        {
            cout << "Server: send() failed: error: " << endl;
        }
        else
            cout << "Server: send() is OK." << endl;

        closesocket(msgsock);
    }

    closesocket(listen_socket);

    exit(0);
}
