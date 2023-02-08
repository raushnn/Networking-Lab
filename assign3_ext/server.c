#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>



typedef struct PDU{
    short messageType;
    short messageLen;
    unsigned char messageval[101];
}MSG;

int compare_ints(const void *a, const void *b) {
  int x = *((int*)a);
  int y = *((int*)b);
  return (x > y) - (x < y);
}

int compare_shorts(const void *a, const void *b) {
  short x = *((short*)a);
  short y = *((short*)b);
  return (x > y) - (x < y);
}

int compare_floats(const void *a, const void *b) {
  float x = *((float*)a);
  float y = *((float*)b);
  return (x > y) - (x < y);
}

int compare_chars(const void *a, const void *b) {
  char x = *((char*)a);
  char y = *((char*)b);
  return (x > y) - (x < y);
}

void swap(void *a, void *b, size_t size) {
  char *pa = a, *pb = b;
  for (size_t i = 0; i < size; ++i) {
    char tmp = *pa;
    *pa++ = *pb;
    *pb++ = tmp;
  }
}

void bubble_sort(void *array, size_t count, size_t size, int (*compare)(const void*, const void*)) {
  for (size_t i = 0; i < count - 1; ++i) {
    for (size_t j = 0; j < count - 1 - i; ++j) {
      char *a = (char*)array + j * size;
      char *b = (char*)array + (j + 1) * size;
      if (compare(a, b) > 0) {
        swap(a, b, size);
      }
    }
  }
}

int main(int argc, char *argv[]){
    struct sockaddr_in my_addr, cli_addr; //sockaddr_in describe an internet socket address
    //clilen will contain the size (in bytes) of the strut pointed by addr;
    //on return it will contain the actual size of the peer address.
    MSG p1, p2;
    socklen_t clilen;  
    int portno, n ;
    portno= atoi(argv[1]);
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
    int accepted_client[10], acc_no=0;
    fd_set readsockfds, readaccfds;
    FD_ZERO(&readaccfds);
    FD_SET(socketfd, &readaccfds);
    int max_sd= socketfd;

    while (1){

        readsockfds= readaccfds;
        int result = select(max_sd+1, &readsockfds, NULL, NULL, NULL);

        if (FD_ISSET(socketfd, &readsockfds)){
            socklen_t clilen= sizeof(cli_addr);
            accepted_client[acc_no]= accept(socketfd, (struct sockaddr *) &cli_addr , &clilen);
            FD_SET(accepted_client[acc_no], &readaccfds);
            if (accepted_client[acc_no]>max_sd){
                max_sd= accepted_client[acc_no];
            }
            acc_no++;
        }

        for (int i=0; i<acc_no; i++){
            if (FD_ISSET(accepted_client[i], &readaccfds)){
                printf("abcd");
                int recived= recv(accepted_client[i], &p1, sizeof(p1), 0);
                p2=*(MSG*)&p1;
                if (recived==0){
                    printf("abbcd");
                    close(accepted_client[i]);
                    FD_CLR(accepted_client[i], &readaccfds);

                }
                printf("%d", recived);
                // printf(PDU.messageVal);
                for(int i=4; i<100; i++){
                    short temp1;
                    memcpy(&temp1, &p2.messageval[i], sizeof(short));
                    printf(" %hd", temp1);
                    i+=2;
                }

            }
        }
    }

    close(socketfd);
    return 0;

}
