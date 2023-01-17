#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int sockfd, portno, n;
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct hostent *server;
  
    server= gethostbyname(argv[1]);
    portno= atoi(argv[2]);
  
    sockfd= socket(AF_INET, SOCK_STREAM, 0);
    // printf("%d", sockfd);
    
    bzero((char *)&serv_addr, sizeof(serv_addr)); //set n bytes of serv_addr to 0
    serv_addr.sin_family= AF_INET;
    bcopy((char *) server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length );
    serv_addr.sin_port= htons(portno);
  
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

     while(1){
        // printf("Hello");
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n= write(sockfd, buffer, strlen(buffer));
        bzero(buffer, 255);
        n= read(sockfd, buffer, 255);
        printf("Server: %s\n", buffer);
        int i= strncmp("Close", buffer, 5);
        if (i==0)
        break;
    }
    close(sockfd);
    return 0;
}
