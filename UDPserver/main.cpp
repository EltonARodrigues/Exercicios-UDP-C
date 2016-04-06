#define LINUX

#ifdef WINDOWS
#include <windows.h>
#include <winsock.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef LINUX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#endif

#define LOCAL_SERVER_PORT   61234
#define LOCALHOST           "127.0.0.1"
#define MAX_MSG             1024

int main(int argc, char *argv[])
{

    int sd; // descritor de socket (socket descriptor)
    int rc; // retorno de funзгo (return code)
    int n;  // quantidade de bytes recebidos/enviados
    //int slen; // tamanho da struct sockaddr_in
    socklen_t slen;
    struct sockaddr_in client;   // informacoes do cliente
    struct sockaddr_in server;   // informacoes do servidor
    char msg[MAX_MSG]; // buffer de informacoes

#ifdef WINDOWS
    WSADATA wsaData;

    int wsret = WSAStartup(MAKEWORD(2, 1), &wsaData);

    if (wsret != 0)
    {
        fprintf(stderr,"\nErro: WSAStartup error (%d)\n",wsret);
        exit(-1);
    }
#endif

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("Socket(): ");
        exit(-2);
    }

    printf("\n-->Socket criado com sucesso");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(LOCAL_SERVER_PORT);

    rc = bind(sd, (struct sockaddr *) &server,
              sizeof (server));
    if (rc < 0)
    {
        perror("bind(): ");
        exit(-3);
    }

    printf("\n-->Bind realizado com sucesso");

    printf("\n-->Aguardando dados pela porta UDP %u\n",
           LOCAL_SERVER_PORT);

    slen = sizeof(client);

    while (1)
    {
        memset(msg,0x0,MAX_MSG);

        n = recvfrom(sd, msg, MAX_MSG,0,(struct sockaddr *) &client, &slen);

        if (n < 0)
        {
            printf("\n-->Dados nao recebidos");
            continue;
        }
    printf("Recebido: %s",msg);


        printf("\n-->Dados recebidos de %s Porta UDP %u\n--> %s \n",
               inet_ntoa(client.sin_addr),
               ntohs(client.sin_port), msg);


    for (int indx=0; indx<100 && msg[indx-1] != 0xA; indx++){ msg[indx] = toupper(msg[indx]);}

    n = sendto(sd, msg, MAX_MSG, 0,(struct sockaddr *) &client,sizeof(server));
    printf("-->Enviando Resposta....");

    }


#ifdef WINDOWS
    WSACleanup();
    closesocket(sd);
#endif // WINDOWS
#ifdef LINUX
    close(sd);
#endif
}
