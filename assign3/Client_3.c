#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>


void* read_data(void* arg){
    char buffer[255];
    int sockfd= *((int*)arg);
    
    while (1)
    {
        bzero(buffer, 255);
        int n= recv(sockfd, buffer, 255, 0);
        if (n==0){
            exit(0);}
        printf("Server: %s\n", buffer);
        int i= strncmp("Bye", buffer, 3);
        if (i==0)
        exit(0);
        
    }
}

void write_data(int sockfd){
    char buffer[255];
    while (1)
    {
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        int n= send(sockfd, buffer, strlen(buffer), 0);
    }
}


int main(int argc, char *argv[]){

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    pthread_t thread1, thread2;
    char buffer[256];

    server= gethostbyname(argv[1]);
    portno= atoi(argv[2]);
    

    sockfd= socket(AF_INET, SOCK_STREAM, 0);
    
    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family= AF_INET;

    //coping the content of server into serv_addr byte by byte
    bcopy((char *) server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length );
    serv_addr.sin_port= htons(portno);

    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    //Creating the process of thread
    pthread_create(&thread1, NULL, read_data,&sockfd );

    write_data(sockfd);
    close(sockfd);
    return 0;
}