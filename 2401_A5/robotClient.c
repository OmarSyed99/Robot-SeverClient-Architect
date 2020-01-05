#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
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
  // ... ADD SOME VARIABLE HERE ... //
  int clientSocket;
  struct sockaddr_in clientAddress;
  int bytesRcv, status;
  char buffer[80];
  char reg[80] = "1";
  char bResponse[5];
  char robotNum[4];
  char xValue[14];
  char yValue[14];
  char dir[4];


  // Set up the random seed
  srand(time(NULL));

  // Register with the server
  clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(clientSocket < 0 ){
    printf("ERROR: socket\n");
    exit(-1);
  }

  memset(&clientAddress, 0, sizeof(clientAddress));
  clientAddress.sin_family = AF_INET;
  clientAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
  clientAddress.sin_port = htons((unsigned short)SERVER_PORT);

  // Connect to server
  status = connect(clientSocket, (struct sockaddr *)&clientAddress, sizeof(clientAddress));
  if(status < 0){
    printf("ERROR: connect\n");
    exit(-1);
  }

  printf("ROBOT CONNECTED\n");


  printf("ROBOTCLIENT sending messeage: %s\n", reg);

  send(clientSocket, reg, 80, 0);
  bytesRcv = recv(clientSocket, buffer, 80, 0);
  buffer[bytesRcv] = '\0';
  printf("ROBOTCLIENT: RECIEVED %s\n", buffer);


  if(buffer[0] == '4'){

    char *token =  strtok(buffer, "^");


    strcpy(bResponse, token);
    token = strtok(NULL, "^");

    strcpy(robotNum, token);
    token = strtok(NULL, "^");

    strcpy(xValue, token);
    token = strtok(NULL, "^");

    strcpy(yValue, token);
    token = strtok(NULL, "^");

    strcpy(dir, token);
    token = strtok(NULL, "^");

      printf("CLOSING CLIENT SOCKET\n");
      close(clientSocket);
      //exit(-1);

    }else if(buffer[0] == '5'){
      printf("21: \n");

    }



  // Go into an infinite loop exhibiting the robot behavior
  while (1) {
    char move[5] = "3";
    char request[80];
    int xdir = atoi(dir) * (PI/180);
    int xVal = atoi(xValue) + (ROBOT_SPEED * cos(xdir));
    int yVal = atoi(yValue) + (ROBOT_SPEED * sin(xdir));


    //printf("atoi num: %d %d\n", xVal, yVal);
    // Connect to the server
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocket < 0 ){
      printf("ERROR: socket\n");
      exit(-1);
    }

    memset(&clientAddress, 0, sizeof(clientAddress));
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
    clientAddress.sin_port = htons((unsigned short)SERVER_PORT);

    status = connect(clientSocket, (struct sockaddr *)&clientAddress, sizeof(clientAddress));
    if(status < 0){
      printf("ERROR: connect\n");
      exit(-1);
    }

    printf("TESTER CONNECTED\n");
    char nxVal[10], nyVal[10];
    gcvt((float) xVal, 10, nxVal);
    gcvt((float) yVal, 10, nyVal);

    strcpy(request, "3");
    strcat(request, "^");
    strcat(request, robotNum);
    strcat(request, "^");
    strcat(request, nxVal);
    strcat(request, "^");
    strcat(request, nyVal);
    strcat(request, "^");
    strcat(request, dir);
    strcat(request, "^");

    printf("CLIENT: SENDING REQUEST %f\n", ROBOT_SPEED * (-cos(atoi(dir))) ) ;
    send(clientSocket, request, sizeof(request), 0);

    bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0);
    buffer[bytesRcv] = '\0';
    if(buffer[0] == '6'){
      int direction = atoi(dir);
      int random = rand() % 2 ;
      int LorR;
      if (random == 0){
        LorR = 90;
      }else if(random == 1){
        LorR = -90;
      }
      direction = (direction + LorR) % 360;
      gcvt((float) direction, 4, dir);

    }else if(buffer[0] == '7'){
      int direction = atoi(dir);
      int random = rand() % 2 ;
      int LorR;
      if (random == 0){
        LorR = 90;
      }else if(random == 1){
        LorR = -90;
      }
      direction = (direction + LorR) % 360;
      gcvt((float) direction, 4, dir);


    }
    printf("ROBOTCLIENT: RECIEVED MOVE_TO RESPONSE %s\n", buffer);
    close(clientSocket);

    gcvt((float) xVal, 10, xValue);
    gcvt((float) yVal, 10, yValue);


    // Compute a forward location and check if it is ok

    // Send MOVE_TO request to server

    // Get response from server.

    // If response is "OK" then move forward

    // Otherwise, we could not move forward, so make a turn.
    // If we were turning from the last time we collided, keep
    // turning in the same direction, otherwise choose a random
    // direction to start turning.

    // Uncomment line below to slow things down a bit
     usleep(10000);
  }

}
