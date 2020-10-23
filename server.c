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

    /*
        These three arrays are parallel, i.e. curr_names[2] corresponds to
        passwords[2], as well as values[2] and so on.
    */
    

    // Creating and binding the server socket called fd
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));
    
    if(bind(fd, (const struct sockaddr *) &addr, sizeof addr) < 0)
        return -1;
    
    if(listen(fd, 5) < 0)
        return -1;

    while(1){
        struct sockaddr_in cl_addr;
        socklen_t cl_addr_len = sizeof cl_addr;

        int cl_fd = accept(fd, (struct sockaddr *) &cl_addr, &cl_addr_len);
        if (cl_fd < 0)
            continue;

        puts("\nNew client connected. Waiting to read data from client...\n");
        while(1) {
            int count;
            char buf[64];
            while ((count = read(cl_fd, buf, sizeof buf)) > 0) {
                if(strcmp(buf, "x") == 0){
                    goto bbreak;
                }
                printf("Client wrote: %s\n\n", buf);
                
            }

        }
        bbreak:
        /* Server closes the client socket after its done */
        close(cl_fd);
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
