#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

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
        puts("Waiting for relay to connect\n");
        int relay_fd = accept(rx_fd, (struct sockaddr *) &cl_addr, &cl_addr_len);
        if (relay_fd < 0)
            continue;

        puts("\nRelay connected. Waiting to read data from relay...\n");
        while(1) {
            char buf[64];
            read(relay_fd, buf, sizeof buf);
            
            printf("Relay wrote: %s\n", buf);


        }
        bbreak:
        /* Server closes the client socket after its done */
        close(relay_fd);
        puts("Client disconnected\n");
    }

}