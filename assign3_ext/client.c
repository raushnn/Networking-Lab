#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>

typedef enum ElementType {
    CHAR,
    SHORT,
    INT,
    FLOAT,
} ElementType;
typedef struct PDU{
    short messageType;
    short messageLen;
    unsigned char messageval[101];
}MSG;


int main(int argc, char *argv[]){
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    MSG p1;

    server= gethostbyname(argv[1]);
    portno= atoi(argv[2]);
    

    sockfd= socket(AF_INET, SOCK_STREAM, 0);
    
    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family= AF_INET;

    //coping the content of server into serv_addr byte by byte
    bcopy((char *) server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length );
    serv_addr.sin_port= htons(portno);

    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    while(1){
        
        printf("choose anything\n0 char\n1 short\n2 int\n3 float\n");
        short n;
        scanf("%hd", &n);
        printf("Number of elements to be sorted\n");
        int size, data_size;
        scanf("%d", &size);
        memset(p1.messageval, 0, sizeof(p1.messageval));
        p1.messageType=0;
        short temp=0;
        p1.messageval[0]= (unsigned char)temp;
        p1.messageval[2]= (unsigned char)n;
        if (n==0){
            if(size>94){
                printf("Size too large\n");
                continue;
            }
            data_size=1;
            printf("Enter elements\n");
            
            for(int i=4; i<(data_size*size)+4; i++){
                char temp;
                scanf("%c", &temp);
                p1.messageval[i]= (unsigned char)temp;
                i+=1;
            }
        }else if (n==1)
        {
            if (size>45){
                printf("Size too large\n");
                continue;
            }
            data_size=2;
            printf("Enter elements\n");
            
            for(int i=4; i<data_size*size; i++){
                short temp;
                scanf("%hd", &temp);
                p1.messageval[i]= (unsigned char)temp;
                i+=2;
            }
        }else if (n==2){
            if (size>23){
                printf("Size too large\n");
                continue;
            }
            data_size=4;
            printf("Enter elements\n");
            
            for(int i=4; i<(data_size*size)+4; i++){
                int temp;
                scanf("%d", &temp);
                p1.messageval[i]= (unsigned char)temp;
                i+=4;
            }
        }else if (n==3)
        {
            if (size>23){
                printf("Size too large\n");
                continue;
            }
            data_size=4;
            printf("Enter elements\n");
            
            for(int i=4; i<(data_size*size)+4; i++){
                float temp;
                scanf("%f", &temp);
                p1.messageval[i]= (unsigned char)temp;
                i+=4;
            }
        }
        p1.messageLen= sizeof(p1);
        int z= send(sockfd, (void *)&p1, sizeof(MSG), 0);
        printf("%d\n", z);
        
        
    }
    
    
    close(sockfd);
    return 0;
}