#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void read_data(void* arg){
    int newsockfd= * (int*)arg;
    char buffer[255];
    while (1)
    {
        bzero(buffer, 255);
        int n= recv(newsockfd, buffer, 255, 0);
        if (n==0){
            exit(0);}
        printf("Client: %s\n", buffer);
        int i= strncmp("Bye", buffer, 3);
        if (i==0)
        exit(0);
        
    }
}

void write_data(int newsockfd){
    char buffer[255];
    while (1)
    {
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        int n= send(newsockfd, buffer, strlen(buffer), 0);
    }
}


int main(int argc, char *argv[]){
    struct sockaddr_in my_addr, cli_addr; //sockaddr_in describe an internet socket address
    pthread_t thread1, thread2;
    //clilen will contain the size (in bytes) of the strut pointed by addr;
    //on return it will contain the actual size of the peer address.
    
    socklen_t clilen;  
    int portno, n;
    portno= atoi(argv[1]);
    char buffer[255];
    bzero((char *)&my_addr, sizeof(my_addr)); //set n bytes of my_addr to 0
    
    int socketfd= socket(PF_INET, SOCK_STREAM, 0);

    //setting up socket addr
    my_addr.sin_family= AF_INET;
    my_addr.sin_port= htons(portno);
    my_addr.sin_addr.s_addr= INADDR_ANY;

    /* 
    bind() assigns the address specified by addr to the socket referred to
    by the file descriptor sockfd. 
    “assigning a name to a socket”.
    */

    if (bind(socketfd, (struct sockaddr *) &my_addr, sizeof(my_addr))<0){
        printf("%s", "Binding Failed");
    }


    listen(socketfd, 5);
    clilen= sizeof(cli_addr);

    int newsockfd= accept(socketfd,(struct sockaddr *) &cli_addr , &clilen);
    
    pthread_create(&thread1, NULL, read_data, &newsockfd );
    write_data(newsockfd);

    close(newsockfd);
    close(socketfd);
    return 0;
    

}
