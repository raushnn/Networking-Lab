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

int sz;
int acc_cli[1000];

void read_data(void* arg){
    int newsockfd= * (int*)arg;
    char buffer[255];
    while (1)
    {
        bzero(buffer, 255);
        int n= recv(newsockfd, buffer, 255, 0);
        
        if (n==0){
            exit(0);}
        int len= strlen(buffer);
        printf("%c\n", buffer[0]);
        printf("Client %d: %s\n",newsockfd, buffer);
        int i= strncmp("Bye", buffer, 3);
        int j= strncmp("lis", buffer, 3);
        if (i==0){
            close(newsockfd);
            break;
        }
        else if (j==0)
        {   
            send(newsockfd, "list of active users\n", strlen("list of active users\n"), 0);
            char u[10]= "User_";
            for(int user=0; user<sz; user++){
                if (acc_cli[user]==newsockfd || acc_cli[user]==0)
                continue;
                bzero(u, 10);
                char u[10]= "\nUser_";
                sprintf(u+6, "%d", acc_cli[user]);
                send(newsockfd, u, strlen(u), 0);
                
            }
            continue;
        }
        
        
        char str[2];
        str[0]=buffer[5];
        str[1]= '\0';
        int fd= atoi(str);
        char num[2];
        sprintf(num, "%d", newsockfd);
        buffer[5]= num[0];
        int l= send(fd, buffer, strlen(buffer), 0);
        printf("%d",l);
        
        // exit(0);
        
    }
}


int main(int argc, char *argv[]){
    struct sockaddr_in my_addr, cli_addr; //sockaddr_in describe an internet socket address
    pthread_t th[1000];
    //clilen will contain the size (in bytes) of the strut pointed by addr;
    //on return it will contain the actual size of the peer address.
    sz=1;
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
    ???assigning a name to a socket???.
    */

    if (bind(socketfd, (struct sockaddr *) &my_addr, sizeof(my_addr))<0){
        printf("%s", "Binding Failed");
    }

    
    listen(socketfd, 5);
    int i=0;
    // pthread_create(&th[0], NULL, write_data, acc_cli);
    while (1){
        clilen= sizeof(cli_addr);
        int newsockfd= accept(socketfd,(struct sockaddr *) &cli_addr , &clilen);
        if (newsockfd>-1 && i<900){
                acc_cli[i]= newsockfd;
                sz++;
                pthread_create(&th[i], NULL, read_data, &acc_cli[i]);
                i++;
        }

    }

    
    close(socketfd);
    return 0;
    

}
