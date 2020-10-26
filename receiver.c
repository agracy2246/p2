#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "hd.h"

int main(int argc, char *argv[]) {

    fd_set readfds;
    int client_socket[2];
    //initialise all client_socket[] to 0 so not checked
    for (int i = 0; i < 2; i++) 
    {
        client_socket[i] = 0;
    }

    // Creating and binding the server socket called fd
    int rx_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));
    
    if(bind(rx_fd, (const struct sockaddr *) &addr, sizeof addr) < 0)
        return -1;
    
    if(listen(rx_fd, 5) < 0)
        return -1;

    while(1){
        struct sockaddr_in cl_addr;
        socklen_t cl_addr_len = sizeof cl_addr;
        puts("[Receiver] Waiting for relay to connect\n");
        int relay_fd = accept(rx_fd, (struct sockaddr *) &cl_addr, &cl_addr_len);
        if (relay_fd < 0)
            continue;

        puts("\n[Receiver] Relay connected. Waiting to read data from relay...\n");
        int multiple = 0;
        char previous[64];

        while(1) {
            char recbuff[64];
            char sendbuff[64];
            read(relay_fd, recbuff, sizeof recbuff);
            int distance = 0;

            multiple++;
            printf("[Receiver] Relay wrote: %s\n", recbuff);
            if(multiple % 2 != 0){
                strcpy(previous, recbuff);
            }
            if(multiple != 0 && multiple % 2 == 0){
                puts("Computing hamming distance..");
                char *hammingArray[2] = {
                    previous,
                    recbuff
                };

                if( (distance = hammingDistance(hammingArray)) >= 0){
                    printf("[Receiver] Hamming distance is: %d\n", distance);

                }
                else{
                    puts("[Receiver] Strings are not of equal length...\n");
                }
                sprintf(sendbuff, "%d", distance);

                //Write to the relay
                write(relay_fd, sendbuff, strlen(sendbuff) + 1);
            }

            /* Server indicates a close */
            if(strcmp(recbuff, "x") == 0 || strcmp(recbuff, "q") == 0){
                goto bbreak;
            }


        }
        bbreak:
        /* Receiver closes the relay socket after its done */
        close(relay_fd);
        puts("Server disconnected\n");
    }

}