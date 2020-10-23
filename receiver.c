#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define NUMBER_OF_STRINGS 6

/*FUNCTION PROTOTYPES*/
int validateCurrency(char buf[], const char *curr_names[], int *parallelIndex);
int validatePassword(char buf[], const char *passwords[], int *parallelIndex);

int main(int argc, char *argv[]) {

    

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
