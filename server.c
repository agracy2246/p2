#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include "fin.h"

/*FUNCTION PROTOTYPES*/
int validateUsername(char *user_and_pass, char user_list[][40]);
int validatePassword(char *user_and_pass, char password_list[][40], int parallelIndex);
void intHandler();

static volatile int receiver_fd;

int main(int argc, char *argv[]) {
    signal(SIGINT, intHandler);

    int message_counter = 0;
    char recbuff[64];

    char user_list[6][40];
    char password_list[6][40];

    /* Read the user names into user_list and passwords into password_list */
    getUserInfo("userList.txt" ,user_list, password_list, 6);

    /* Read the server info */

    /* Will hold the username and password retrieved by the client
        index 0 is username, index 1 is the password
    */
    char user_and_pass[2][20];

    /* Create the receiver socket and setup a connection*/
    receiver_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(receiver_fd < 0){
        puts("Error creating socket");
        exit(1);
    }
    struct sockaddr_in rx_addr;
    memset(&rx_addr, 0, sizeof rx_addr);
    rx_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &rx_addr.sin_addr);
    rx_addr.sin_port = htons(atoi(argv[1]) + 1);
    if(connect(receiver_fd, (const struct sockaddr *) &rx_addr, sizeof rx_addr) < 0){
        puts("Error connecting..");
        exit(1);
    }
    
    // Creating and binding the server socket called relay_fd
    int relay_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));
    
    if(bind(relay_fd, (const struct sockaddr *) &addr, sizeof addr) < 0)
        return -1;
    
    if(listen(relay_fd, 5) < 0)
        return -1;

    while(1){
        struct sockaddr_in cl_addr;
        socklen_t cl_addr_len = sizeof cl_addr;
        puts("Waiting for client to connect\n");
        int cl_fd = accept(relay_fd, (struct sockaddr *) &cl_addr, &cl_addr_len);
        if (cl_fd < 0)
            continue;

        puts("\nNew client connected. Waiting to read data from client...\n");
        while(1) {
            char buf[64];

            read(cl_fd, buf, sizeof buf);
            if(strcmp(buf, "x") == 0 || strcmp(buf,"q") == 0){
                goto bbreak;
            }

            /* Split message in two parts: first is the type of message, second is the message data */
            int i = 0;
            char *p = strtok (buf, "-");
            char *split_message[2];
            memset(split_message, sizeof(split_message), sizeof split_message);
            while (p != NULL)
            {
                split_message[i++] = p;
                p = strtok (NULL, "-");
            }
            printf("[Server] Client wrote: Type=%s Data=%s\n", split_message[0],split_message[1]);

            /* 
                Decide what to do with the message from the user 
            */

           /* Client sent a user name */
            if(strcmp(split_message[0], "username") == 0){
                strcpy(user_and_pass[0], split_message[1]);
                printf("[Server] Stored %s as the username\n", user_and_pass[0]);
                write(cl_fd, "ACK", 4);
                puts("[Server] Sent ACK message to the client.");
            }
            /* Client sent a password */
            if(strcmp(split_message[0], "password") == 0){
                strcpy(user_and_pass[1], split_message[1]);
                printf("[Server] Stored %s as the password\n", user_and_pass[1]);
                if(validatePassword(user_and_pass[1], password_list, validateUsername(user_and_pass[0], user_list))){
                    printf("[Server] %s validated with password \"%s\"\n", user_and_pass[0], user_and_pass[1]);
                    write(cl_fd, "good_auth", 10);
                }
                else{
                    puts("[Server] Bad password...");
                    write(cl_fd, "bad_auth",9);
                }
                

            }

            /* Client send a message to be relayed to the receiver */
            if(strcmp(split_message[0], "relay") == 0){
                write(receiver_fd, split_message[1], strlen(split_message[1])+ 1);
                message_counter++;
                if(message_counter == 2){
                    message_counter = 0;
                    read(receiver_fd, recbuff, sizeof(recbuff));
                    printf("[Server] Receiver send: %s\n", recbuff);
                    write(cl_fd, recbuff, strlen(recbuff) + 1);

                }
            }
        }
        bbreak:
        /* Server closes the client socket after its done */
        close(cl_fd);
        puts("[Server] Client disconnected\n");
    }

}
/* Searches for the name in the array, stores the index or returns -1 if not found*/
int validateUsername(char *user_and_pass, char user_list[][40]){
    for(int i = 0; i < 6; i++){
        if(strcmp(user_list[i], user_and_pass) == 0){
            printf("[Server] User \"%s\" found in the list...\n", user_and_pass);
            return i;
        }
    }
    printf("[Server] User \"%s\" not found in the list...\n", user_and_pass);
    return -1;
}
/* Checks the password given by the client against the one at the parallelIndex */
int validatePassword(char *user_and_pass, char password_list[][40], int parallelIndex){
    /* if parallelIndex < 0 then the username doesn't exist */
    if(parallelIndex < 0)
        return 0;

    if(strcmp(password_list[parallelIndex], user_and_pass) == 0){
        return 1;
    }
    else {
        return 0;
    }
    
}

/* Handles user pressing Control+C on the server */
void intHandler(){
    write(receiver_fd, "x", 2);
    close(receiver_fd);
    exit(0);
}