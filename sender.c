#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
static volatile int fd;

/* Handles user pressing Control+C */
void intHandler(){
    write(fd, "x", 2);
    close(fd);
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, intHandler);
    if(argc < 3 || argc > 3){
        puts("USAGE: ./sender.out <IPADDR> <PORTNO>");
        exit(1);
    }

    /* Create the client socket and setup a connection*/
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        puts("Error creating socket");
        exit(1);
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &addr.sin_addr);
    addr.sin_port = htons(atoi(argv[2]));
    if(connect(fd, (const struct sockaddr *) &addr, sizeof addr) < 0){
        puts("Error connecting..");
        exit(1);
    }

    char username_buf[64];
    char recbuff[64];

    /* On successful connection, prompt for a user name */
    puts("Welcome, please enter a username:");

    /* Get user name from STDIN */
    fgets(username_buf, sizeof(username_buf), stdin);
    size_t ln = strlen(username_buf) - 1;
    if (username_buf[ln] == '\n')
        username_buf[ln] = '\0';
    
    do{
        write(fd, username_buf, strlen(username_buf) + 1);
        //read(fd, recbuff, 1);
        fgets(username_buf, sizeof(username_buf), stdin);
        size_t ln = strlen(username_buf) - 1;
        if (username_buf[ln] == '\n')
            username_buf[ln] = '\0';

    }while(1);
    puts("closing socket");
    close(fd);
}