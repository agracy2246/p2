#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/*FUNCTION PROTOTYPES*/
int validateCurrency(char buf[], const char *curr_names[], int *parallelIndex);
int validatePassword(char buf[], const char *passwords[], int *parallelIndex);

int main(int argc, char *argv[]) {

    /* Will hold the username and password retrieved by the client
        index 0 is username, index 1 is the password
    */
    char *user_and_pass[2];
    

    // Creating and binding the server socket called fd
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
            if(strcmp(buf, "x") == 0){
                goto bbreak;
            }

            /* Split message in two parts: first is the type of message, second is the message data */
            int i = 0;
            char *p = strtok (buf, "-");
            char *split_message[2];

            while (p != NULL)
            {
                split_message[i++] = p;
                p = strtok (NULL, "-");
            }
            printf("Client wrote: Type =%s Data=%s\n\n", split_message[0],split_message[1]);

            /* 
                Decide what to do with the message from the user 
            */

           /* Client sent a user name */
            if(strcmp(split_message[0], "username") == 0){
                user_and_pass[0] = split_message[1];
                printf("Stored %s as the username\n", user_and_pass[0]);
                write(cl_fd, "ACK", 4);
                puts("Sent ACK message to the client.");
            }
            /* Client sent a password */
            if(strcmp(split_message[0], "password") == 0){
                user_and_pass[1] = split_message[1];
                printf("Stored %s as the password\n", user_and_pass[1]);
                write(cl_fd, "good_auth", 10);
            }

        }
        bbreak:
        /* Server closes the client socket after its done */
        close(cl_fd);
        puts("Client disconnected\n");
    }

}
/* Searches for the currency in the array, stores the index */
int validateCurrency(char buf[], const char* curr_names[], int *parallelIndex){
    for(int i = 0; i < 6; i++){
        if(strcmp(curr_names[i], buf) == 0){
            *parallelIndex = i;
            return 1;
        }
    }
    return 0;
}
/* Checks the password given by the client against the one at the parallelIndex */
int validatePassword(char buf[], const char *passwords[], int *parallelIndex){
    if(strcmp(passwords[*parallelIndex], buf) == 0){
        return 1;
    }
    return 0;
}
