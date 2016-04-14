/*
 Proxy UDP/TCP
 Elton de Andrade Rodrigues RA: 00079714
 */
#define LINUX

#ifdef WINDOWS
#include <windows.h>
#include <winsock.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <iostream>



#include <stdio.h>
#include <stdlib.h>

#ifdef LINUX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket(n) close(n)
#define SOCKET socklen_t
#endif

// UDPCLient Comunicacao
#define SERVER_ADRESS            "127.0.0.1"
#define LOCAL_SERVER_PORT_PROXY   61234
#define MAX_MSG                   1024
// TCPServer comunicaço
#define DEFAULT_PORT 64321
#define IPADDRESS "127.0.0.1"

using namespace std;

int main(int argc, char ** argv)
{
    //UDP var
    int sd; // descritor de socket (socket descriptor)
    int rc; // retorno de funзгo (return code)
    int n;  // quantidade de bytes recebidos/enviados

    //TCP var
    SOCKET connect_socket;
    int retval;

    //Universal var
    //int slen; // tamanho da struct sockaddr_in
    socklen_t  slen;
    socklen_t fromlen;
    struct sockaddr_in client;  // informacoes do cliente udp
    struct sockaddr_in proxy;   // informacoes do proxy udp/tcp
    struct sockaddr_in server;  // informacao do server tcp
    char msg[MAX_MSG]; // buffer de informacoes



    // cominicacao client
    proxy.sin_family = AF_INET;
    proxy.sin_addr.s_addr = htonl(INADDR_ANY);
    proxy.sin_port = htons(LOCAL_SERVER_PORT_PROXY);

    // Comunicacao server
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IPADDRESS);
    server.sin_port = htons(DEFAULT_PORT);

    connect_socket = socket(AF_INET, SOCK_STREAM, 0);


    //socket para UDP

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("Socket(): ");
        exit(-2);
    }

    cout << "\nProxy: Socket com o client criado com sucesso" <<endl;


    //conecta servidor com proxy
    if (connect_socket < 0)
    {
        cout << "Proxy: socket() failed with error: " << connect_socket << endl;
        exit(-1);
    }
    else
        cout << "Proxy: socket() is OK." << endl;

    if (connect(connect_socket, (struct sockaddr*) &server, sizeof (server)) < 0)
    {
        cout << "Proxy: connect() failed\n" << endl;
        exit(-2);
    }
    else
        cout << "Proxy: connect() is OK." << endl;
    // ----------------- fim coneccao --------------------------------------


    rc = bind(sd, (struct sockaddr *) &proxy,
              sizeof (proxy));
    if (rc < 0)
    {
        perror("bind(): ");
        exit(-3);
    }

    cout << "\nProxy: Bind realizado com sucesso"<< endl;

    cout << "\nProxy: Aguardando dados do cliente pela porta UDP "<< LOCAL_SERVER_PORT_PROXY << endl;

    slen = sizeof(client);

    while (1)
    {
        memset(msg,0x0,MAX_MSG);

        n = recvfrom(sd, msg, MAX_MSG,0,(struct sockaddr *) &client, &slen);

        if (n < 0)
        {
            printf("\nProxy: Dados nao recebidos");
            continue;
        }

        else{
        cout << "Proxy: String Recebida do Client........\n" << endl;
        //printf("Recebido: %s",msg);
        break;
        }

    }

    // -----------------------mensagem tcp----------------------------------

    retval = send(connect_socket, msg, sizeof (msg), 0);
    if (retval <= 0)
    {
        cout << "Proxy: send() failed: error: " << endl;
        closesocket(connect_socket);
        exit(-3);
    }
    else
        cout << "Proxy: send() is OK." << endl;

    retval = recv(connect_socket, msg, sizeof (msg), 0);
    if (retval <= 0)
    {
        cout << "Proxy: recv() failed" << endl;
        exit(-4);
    }
    else
        cout << "Proxy: recv() is OK." << endl;

    //cout << "Dado Recebido: " << msg << endl;


    printf("\nProxy: Dados recebidos de %s Porta FTP %u",
               inet_ntoa(server.sin_addr),
               ntohs(server.sin_port));

    n = sendto(sd, msg, MAX_MSG, 0,(struct sockaddr *) &client,sizeof(proxy));
    printf("\nProxy: Resposta Enviada....\n\n\n");




    closesocket(connect_socket);



#ifdef LINUX
    sleep(5);
#endif

    exit(0);

#ifdef WINDOWS
    closesocket(sd);
    WSACleanup();
#endif
#ifdef LINUX
    close(sd);
#endif // LINUX

    exit(0);
}
