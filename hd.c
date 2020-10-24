#include <stdio.h>
#include <string.h>
#include "hd.h"

int hammingDistance(char *array[]);

int main(){
    
    char *array[2];
    array[0] = "racecar";
    array[1] = "racccar";
    

    printf("%d\n", hammingDistance(array));
    
}

