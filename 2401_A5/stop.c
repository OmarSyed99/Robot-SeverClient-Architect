#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "simulator.h"


// Set up a client socket and connect to the server.
// Return -1 if there was an error.
int connectToServer(int *sock,  struct sockaddr_in *address) {
  // ... WRITE YOUR CODE HERE ... //
}


int main() {
  // Variables
  int clientSocket;
  struct sockaddr_in clientAddr;
  int bytesRcv, status;
  char buffer[80];
  char input[80] = "2";


  // Register with the server
  // Create socket
  clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(clientSocket < 0 ){
    printf("ERROR: socket\n");
    exit(-1);
  }

  // Setup address
  memset(&clientAddr, 0, sizeof(clientAddr));
  clientAddr.sin_family = AF_INET;
  clientAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
  clientAddr.sin_port = htons((unsigned short)SERVER_PORT);

  // Connect to server
  status = connect(clientSocket, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
  if(status < 0){
    printf("ERROR: connect\n");
    exit(-1);
  }

  printf("CONNECTED\n");


  // Send command string to server
  // ... WRITE SOME CODE HERE ... //

  //while (1) {
    // Get a command from the user

    // Send command string to server
    strcpy(buffer, input);
    printf("CLIENT: Sending \"%s\" to server.\n", buffer);
    send(clientSocket, buffer, strlen(buffer), 0);
  //}

}
