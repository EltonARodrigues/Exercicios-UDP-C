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
    struct sockaddr_in proxy;
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
        perror("Client: socket()");
        exit(-2);
    }


    proxy.sin_family = AF_INET;
    proxy.sin_port = htons(LOCAL_SERVER_PORT);
    proxy.sin_addr.s_addr = inet_addr(SERVER_ADRESS);

    printf("\nClient: ");
    for (; indx<100 && msg[indx-1] != 0xA; indx++)
    {
        msg[indx] = fgetc(stdin);
        putc(msg[indx],stdout);
    }

    msg[indx] = '\0';


    n = sendto(sd, msg, MAX_MSG, 0,
               (struct sockaddr *) &proxy,
               sizeof (proxy));

    if (n < 0)
    {
        perror("Client: sendto()");
        exit(2);
    }

    slen = sizeof(proxy);

    while (1)
    {
        memset(msg,0x0,MAX_MSG);

        n = recvfrom(sd, msg, MAX_MSG,0,(struct sockaddr *) &proxy, &slen);

        if (n < 0)
        {
            printf("\nClient: Dados nao recebidos");
            continue;
        }

        printf("\nClient: Dados recebidos de %s Porta UDP %u\nResposta: %s \n",inet_ntoa(proxy.sin_addr),ntohs(proxy.sin_port), msg);

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
