#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int main(int argc, char *argv[]){
    srand(time(NULL));
    //character reference for generating the key
    char text[27] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    //key variable with extra space
    char key [atoi(argv[1]) + 1];
    memset(key,0,strlen(key));
    int i = 0;
    int random;
    for (i; i < atoi(argv[1]); i++){
        // get random number between 0 and 26
        random = rand() % 27; 
        //store that value with a reference to the text variable for a particular character
        key[i] = text[random];  
    }
    //finish the key to fill space for proper size
    key[atoi(argv[1])] = '$';
    key[atoi(argv[1]) + 1] = '\0';
    printf("%s", key);
}   