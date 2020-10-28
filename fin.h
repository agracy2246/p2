#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

void getUserInfo(char *filePath,char user_list[][40], char password_list[][40], int sizeOfArrays){
    FILE *file_ptr;
    char buf[64];
    file_ptr = fopen("userList.txt", "r");

    if(!file_ptr){
        puts("Could not open the file...");
        exit(1);
    }

    int j = 0;
    while(fgets(buf, sizeof(buf), file_ptr) != NULL){

        int i = 0;
        
        char *split_array[2];
        char *p = strtok (buf, " ");

        while(p != NULL){
            split_array[i++] = p;
            p = strtok(NULL, " ");

        }
        
        strcpy(user_list[j], split_array[0]);
        strcpy(password_list[j], split_array[1]);

        /* Replace newline with null terminator */
        size_t ln = strlen(password_list[j]) - 1;
        if (password_list[j][ln] == '\n')
            password_list[j][ln] = '\0';

        j++;
    }

}

struct ServerInfo{
    char receiver_name[32];
    char addr[16];
    int port;
};

void getReceiverInfo(char *filePath, struct ServerInfo server_info[]){
    FILE *file_ptr;
    char buf[64];
    file_ptr = fopen("receiverList.txt", "r");

    if(!file_ptr){
        puts("Could not open the file...");
        exit(1);
    }

    int j = 0;
    while(fgets(buf, sizeof(buf), file_ptr) != NULL){

        int i = 0;
        
        char *split_array[3];
        char *p = strtok (buf, " ");

        while(p != NULL){
            split_array[i++] = p;
            p = strtok(NULL, " ");

        }
        
        strcpy(server_info[j].receiver_name, split_array[0]);
        strcpy(server_info[j].addr, split_array[1]);
        server_info[j].port = atoi(split_array[2]);

        j++;
    }

}
