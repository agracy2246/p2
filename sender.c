#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>

static volatile int relay_fd;
#define USERNAME_MESSAGE_HEADER = "username-"
#define PASSWORD_MESSAGE_HEADER = "password-"
/* Handles user pressing Control+C */
void intHandler(){
    write(relay_fd, "x", 2);
    close(relay_fd);
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, intHandler);
    int messages_sent = 0;
    if(argc < 3 || argc > 3){
        puts("USAGE: ./sender.out <IPADDR> <PORTNO>");
        exit(1);
    }

    /* Create the client socket and setup a connection*/
    relay_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(relay_fd < 0){
        puts("Error creating socket");
        exit(1);
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &addr.sin_addr);
    addr.sin_port = htons(atoi(argv[2]));
    if(connect(relay_fd, (const struct sockaddr *) &addr, sizeof addr) < 0){
        puts("Error connecting..");
        exit(1);
    }

    char inputbuff[64];
    char sendbuff[64];
    char recbuff[64];

    
    
    do{
        /* On successful connection, prompt for a user name */
        puts("Welcome, please enter a username:");

        /* Get user name from STDIN */
        fgets(inputbuff, sizeof(inputbuff), stdin);
        size_t ln = strlen(inputbuff) - 1;
        if (inputbuff[ln] == '\n')
            inputbuff[ln] = '\0';

        strcpy(sendbuff, "username-");
        strcat(sendbuff, inputbuff);
        /* Write "username-<USER INPUT>" */
        write(relay_fd, sendbuff, strlen(sendbuff) + 1);

        read(relay_fd, recbuff, sizeof(recbuff));
        if(strcmp(recbuff, "ACK") == 0){
            puts("Please enter a password:");
            fgets(inputbuff, sizeof(inputbuff), stdin);
            size_t ln = strlen(inputbuff) - 1;
            if (inputbuff[ln] == '\n')
                inputbuff[ln] = '\0';
            strcpy(sendbuff, "password-");
            strcat(sendbuff, inputbuff);

            /* Write "password-<USER INPUT>" */
            write(relay_fd, sendbuff, strlen(sendbuff) +1);
            
            /* Check what the server replied */
            read(relay_fd, recbuff, sizeof(recbuff));
            /* If username and password match, proceed to final loop (sending strings to receiver) */
            if(strcmp(recbuff,"good_auth") == 0){
                puts("User authenticated, entering ham loop");
                goto hamming_loop;

            }else{
                puts("User could not be authenticated. Try Again.");
            }

        }
        else{
            puts("The server could not acknowledge your request. Try again.");
            continue;
        }

    }while(1);

    hamming_loop:
    
    while(1){
        /* Get user name from STDIN */
        printf("Enter string #%d\n", messages_sent + 1);
        fgets(inputbuff, sizeof(inputbuff), stdin);
        size_t ln = strlen(inputbuff) - 1;
        if (inputbuff[ln] == '\n')
            inputbuff[ln] = '\0';

        strcpy(sendbuff, "relay-");
        strcat(sendbuff, inputbuff);
        /* Write "relay-<USER INPUT>" */
        write(relay_fd, sendbuff, strlen(sendbuff) + 1);
        messages_sent++;
        if(messages_sent == 2){
            read(relay_fd, recbuff, sizeof(recbuff));
            
            if(strcmp(recbuff, "-1") == 0){
                puts("[Error] You must provide strings of equal length...");
            }
            else {
                printf("Hamming distance is: %s\n", recbuff);
            }
            messages_sent = 0;
        }
    
        
    }
    puts("closing socket");
    close(relay_fd);
}