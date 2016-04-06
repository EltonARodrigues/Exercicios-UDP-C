#define LINUX

#ifdef WINDOWS
#include <windows.h>
#include <winsock.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#ifdef LINUX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#define SERVER_ADRESS      "127.0.0.1"
#define LOCAL_SERVER_PORT   61234
#define MAX_MSG             1024

int main(int argc, char ** argv)
{

    int sd;
    int indx = 0;
    int n;
    socklen_t slen;
    struct sockaddr_in server;
    char msg[MAX_MSG];

#ifdef WINDOWS
    WSADATA wsaData;
    int wsret = WSAStartup(MAKEWORD(2, 1), &wsaData);

    if (wsret != 0)
    {
        perror("WSAStartup(): ");
        exit (-1);
    }
#endif

    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket()");
        exit(-2);
    }


    server.sin_family = AF_INET;
    server.sin_port = htons(LOCAL_SERVER_PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_ADRESS);

    printf("\n--> ");
    for (; indx<100 && msg[indx-1] != 0xA; indx++)
    {
        msg[indx] = fgetc(stdin);
        putc(msg[indx],stdout);
    }

    msg[indx] = '\0';


    n = sendto(sd, msg, MAX_MSG, 0,
               (struct sockaddr *) &server,
               sizeof (server));

    if (n < 0)
    {
        perror("sendto()");
        exit(2);
    }

    slen = sizeof(server);

    while (1)
    {
        memset(msg,0x0,MAX_MSG);

        n = recvfrom(sd, msg, MAX_MSG,0,(struct sockaddr *) &server, &slen);
        //n = recvfrom(sd, msg,(int) sizeof(MAX_MSG), 0, (sockaddr*) &client, &slen);
        if (n < 0)
        {
            printf("\n-->Dados nao recebidos");
            continue;
        }
    //printf("Resposta do Servidor: %s",msg);

        printf("\n-->Dados recebidos de %s Porta UDP %u\n--> Resposta: %s \n",
               inet_ntoa(server.sin_addr),
               ntohs(server.sin_port), msg);
    break;
    }


#ifdef WINDOWS
    closesocket(sd);
    WSACleanup();
#endif
#ifdef LINUX
    close(sd);
#endif // LINUX

    exit(0);
}
