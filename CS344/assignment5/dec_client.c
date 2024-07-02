
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//global variables used in the program
char ciphertext[300];
char ciphertextkey [800];
char key[300];
// Error function used for reporting issues
void error(const char *msg, int val) { 
    fprintf(stderr, msg) ;
    exit(val); 
} 
//making the key for the ciphertext to sent to be decrypted 
void make_key(){
    //create variable string
    strcpy(ciphertextkey, "");
    //first add the ciphertext
    strcat(ciphertextkey, ciphertext);
    // separate the variables
    strcat(ciphertextkey, "-");
    //add the key
    strcat(ciphertextkey, key);
    //NULL symbol to finish the key
    strcat(ciphertextkey, "\0");
}
//verifying that all the chacracters are valid and right size
void check_key(){
    //text reference variable
    char text[27] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    //make sure thee key is the right size
    if (strlen(key) < strlen(ciphertext)){
        error("ERROR keysize too small\n", 1);
    }
    int i = 0;
    //loop to validate characters
    for (i; i< strlen(ciphertext) - 1; i++){ 
      //goes through the whole reference list(text variable) to make sure each and every character is valid
      if(strchr(text, ciphertext[i]) == NULL){
         error("ERROR invalid ciphertext character \n", 1);
      }
      //same with the key
      if(strchr(text, key[i]) == NULL){
        error("ERROR invalid key character \n", 1);
      }
    }
}
int main(int argc, char *argv[]){

    int socketFD, charsWritten, charsRead;
    struct sockaddr_in serverAddress;
    char buffer[800];
    // Check usage & args
    if (argc < 4) { 
        fprintf(stderr,"USAGE: %s ciphertext key port\n", argv[0]); 
        exit(0); 
    } 
    
    // Set up the server address struct
    // Clear out the address struct
    memset((char*)&serverAddress, '\0', sizeof(serverAddress)); 
    // The address should be network capable
    serverAddress.sin_family = AF_INET;
     // Store the port number - argv[3]
    serverAddress.sin_port = htons(atoi(argv[3]));
    // Get the DNS entry for localhost
    struct hostent* serverHostInfo = gethostbyname("localhost"); 
    if (serverHostInfo == NULL) { 
        error("CLIENT: ERROR, no such host\n", 2);    
    }
    // Copy the first IP address from the DNS entry to sin_addr.s_addr
    memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address
    
    //open the file with the ciphertext and read
    FILE * cipherf = fopen(argv[1], "r");  
    //get the contents for the ciphertext
    fgets(ciphertext, 300, (FILE*)cipherf); 
    // Remove the trailing \n that fgets adds
    ciphertext[strcspn(ciphertext, "\n")] = '\0'; 
    //close cipher file
    fclose(cipherf);

    //open the file with the key and read
    FILE * keyf = fopen(argv[2], "r");
    //get the contents for the ciphertext
    fgets(key, 300, (FILE*)keyf);
    // Remove the trailing \n that fgets adds
    key[strcspn(key, "\n")] = '\0';
    //close key file
    fclose(keyf);
    
    // Create a socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0); 
    if (socketFD < 0){
        error("CLIENT: ERROR opening socket, attempted port number ", 2);
    }
    
    // Connect to server
    if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
        error("CLIENT: ERROR connecting", 2);
    }
    
    
    //make sure that they key and plaintext are valid
    check_key();
    
    //calling to make the key
    make_key();
    

    //send a message to the dec_server verifying it is the dec_client
    send(socketFD, "dec", strlen("dec"), 0); 
    //recieve message of verification that it is dec_server without null character
    charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0);
    //error handling for message recieved
    if (strcmp("FAIL", buffer) == 0){ 
        error("CLIENT: dec_client not connected to enc_server \n", 1);
    }
    // Send message to dec_server to decrypt 
    charsWritten = send(socketFD, ciphertextkey, strlen(ciphertextkey), 0);
    if (charsWritten < 0){ 
        error("CLIENT: ERROR writing to socket", 2);
    }
    //make sure data is written 
    if (charsWritten < strlen(buffer)){
        printf("CLIENT: WARNING: Not all data written to socket!\n", 2);
    }
    
    // Clear out the buffer array
    memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse

    //read socket message from server of success
    charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); 
    if (charsRead < 0){
        error("CLIENT: ERROR reading from socket", 2);
    }
    printf("%s\n", buffer);
    // Close the socket
    close(socketFD); 
    return 0;
}