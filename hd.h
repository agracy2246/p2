int hammingDistance(char *array[]){
    /* Check to see if strings are of equal length */
    if(strlen(array[0]) != strlen(array[1])){
        return -1;
    }
    int distance = 0;    
    
    int i = 0;
    while (array[0][i] != '\0'){
        if (array[0][i] != array[1][i])
            distance++;
        i++;
    }
    return distance;
}