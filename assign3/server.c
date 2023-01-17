#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
    struct sockaddr_in my_addr, cli_addr; //sockaddr_in describe an internet socket address
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
//     printf("%d", newsockfd);
    while (1)
     {
        bzero(buffer, 255);
        n= read(newsockfd, buffer, 255);
        printf("Client: %s\n", buffer);
        n= write(newsockfd, buffer, strlen(buffer));
        bzero(buffer, 255);
        int i= strncmp("Close", buffer, 5);
        if (i==0)
        break;
     }
    close(newsockfd);
    close(socketfd);
    return 0;
    

}
