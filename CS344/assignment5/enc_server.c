
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//global variables used in the program
char buffer[800];
char ciphertext[300];
char plaintext[300];
// Error function used for reporting issues
void error(const char *msg) { 
    perror(msg); 
    exit(2); 
} 
//encrypting plaintext into ciphertext
void encrypt(){
  //read message first part (till the - separator)
  char *token = strtok(buffer, "-"); 
  //assign the first token to the plaintext 
  strcpy(plaintext, token); 
  //key variable
  char key[300];
  //read message second part (till the end)
  token = strtok(NULL, "\0");
  //assign the second token to the key
  strcpy(key, token);
  
  //valid character reference
  char text[27] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  //index values for loop
  int i = 0;
  int j = 0;
  int size = strlen(plaintext);
  //integer variables for ciphertext, plaintext, and key
  int k;
  int plain;
  int cipher;
  //for loops where encryption takes place
  //first index goes through the key and plaintext and converts each letter at a time
  for(j = 0; j < size; j++){  
    //second index goes through all the characters in the valid character reference
    //breaks when the character is found with the char variable
    for (i = 0; i < 27; i++){
    	if (key[j] == text[i]){
        //assign integer value for convertion 
        k = i;
        break;
   	  }
    }
    for (i = 0; i < 27; i++){
      if (plaintext[j] == text[i]){
        //assign integer value for convertion
   		  plain = i;  
        break;          
   	  }  
    }
    //converting to integer value for ciphertext one by one conversion of characters
    cipher = (plain + k) % 27;
    //loops to assign characters for the integer value for each individual one
    for (i = 0; i < 27; i++){
    	if (cipher == i){
    	  ciphertext[j] = text[i];
         //breaks once character is assigned
        break;
   	  }
    }
  }
}

int main(int argc, char *argv[]){
    int connectionSocket, charsRead;
    struct sockaddr_in serverAddress, clientAddress;
    //Getting the size of the client address to connect
    socklen_t sizeOfClientInfo = sizeof(clientAddress); 
    // Check usage & args
    if (argc < 2) { 
        fprintf(stderr,"USAGE: %s port\n", argv[0]); 
        exit(1); 
    } 

    // Set up the address struct
    // Clear out the address struct
    memset((char *)&serverAddress, '\0', sizeof(serverAddress)); 
    // The address should be network capable
    serverAddress.sin_family = AF_INET; 
    // Store the port number argv[1]
    serverAddress.sin_port = htons(atoi(argv[1]));
    // Allow a client at any address to connect to this server 
    serverAddress.sin_addr.s_addr = INADDR_ANY; 

    // Create the socket that will listen for connections
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0); 
    //error with opening the socket
    if (listenSocket < 0)
        error("ERROR opening socket");
        
    // Associate the socket to the port    
    if (bind(listenSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) 
        error("ERROR on binding");
        // Start listening for connetions. Allow up to 5 connections to queue up
        listen(listenSocket, 5);     
        
    // Accept a connection, blocking if one is not available until one connects
    //Loops as long as the parent process exists
    while (1){
        // Accept the connection request which creates a connection socket
        connectionSocket = accept(listenSocket, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); 
        // fork a child
        int child = fork();
        // works only if child process exists 
        if (child == 0){ 
            //error connecting
            if (connectionSocket < 0){ 
                error("ERROR on accept");
            }
            //connected  to the server
            printf("SERVER: Connected to client running at host %d port %d\n", ntohs(clientAddress.sin_addr.s_addr), ntohs(clientAddress.sin_port));
            // Get the message from the client and display it
            memset(buffer, '\0', 800);

            // Read the client's message from the socket
            charsRead = recv(connectionSocket, buffer, 300, 0); 
            //making sure there is something in the message
            if (charsRead < 0){ 
                error("ERROR reading from socket");
            }
            //Make sure it is the enc_client through its unique "enc" signal
            if (strcmp(buffer,"enc") != 0){
                //failed connection protocol 
                charsRead = send(connectionSocket, "FAIL", 5, 0); 
                error("ERROR connection refused");
            }

            // Send a Success message back to the enc_client
            charsRead = send(connectionSocket, "PASS", 5, 0);
            
            //make sure something was sent 
            if (charsRead < 0){ 
                error("ERROR writing to socket");
            }
            
            //Read the enc_client message with the plaintext
            charsRead = recv(connectionSocket, buffer, 600, 0); 

            //Call to encrypt the freshly recieved plaintext
            encrypt();


            // Send the ciphertext to the enc_client
            charsRead = send(connectionSocket, ciphertext, strlen(ciphertext), 0); 
            
            if (charsRead < 0){ 
                error("ERROR writing to socket");
            }
            // Close the connection socket for this client
            close(connectionSocket); 
        }
        
    }
    // Close the listening socket
    close(listenSocket); 
    return 0;
}
