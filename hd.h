int hammingDistance(char *array[]){
    int distance = 0;    
    
    int i = 0;
    while (array[0][i] != '\0'){
        if (array[0][i] != array[1][i])
            distance++;
        i++;
    }
    return distance;
}