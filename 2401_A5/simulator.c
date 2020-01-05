#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "simulator.h"

#include "display.c"


// The environment that contains all the robots
Environment    environment;
int numRobots = 0;



// Initialize the server by creating the server socket,
// setting up the server address, binding the server socket
// and setting up the server to listen for the MAX_ROBOTS
// number of robot clients
void initializeServer(int *serverSocket,
                      struct sockaddr_in  *serverAddress) {
  // ... WRITE SOME CODE HERE ... //
}


// Determine whether the registered robot with id i is able
// to move to the location specified by (newX, newY). Return
// OK if it is able.  Return NOT_OK_COLLIDE if moving to that
// location would cause the robot to collide with another robot.
// Return NOT_OK_BOUNDARY if moving to that location would cause
// the robot to go beyond the environmental boundary.
char canMoveTo(/* PUT PARAMS HERE */) {


  // ... WRITE YOUR CODE HERE ... //


  // delay to slow things down
  usleep(10000/(1 + environment.numRobots));
  return OK;
}



// Handle client requests coming in through the server socket.
// This code should run indefinitiely.  It should wait for a
// client to send a request, process it, and then close the
// client connection and wait for another client.  The requests
// that may be handlesd are REGISTER and MOVE_TO.   Upon
// receiving a REGISTER request, the server should choose a
// random location and direction within the environment (which
// MUST be within the boundaries and must NOT be overlapping
// another robot).  Upon receiving a MOVE_TO request, the
// code should send a single byte to the client which is either
// OK or NOT_OK_BOUNDARY (if the robot would collide with the
// boundary at the requested location) or NOT_OK_COLLIDE (if the
// robot would collide with another robot at the requested
// location).  Parameter *e is a pointer to the environment.
void *handleIncomingRequests(void *e) {

  // Variables
  int                 serverSocket, clientSocket;
  struct sockaddr_in  serverAddress, clientAddr;
  int                 status, addrSize, bytesRcv;
  char                buffer[20];
  char              response[80];


  // Initialize the server

  // Server socket
  serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serverSocket < 0) {
    printf("*** SERVER ERROR: Could not open socket.\n");
    exit(-1);
  }

  // Server address
  memset(&serverAddress, 0, sizeof(serverAddress)); // zeros the struct
  serverAddress.sin_family        = AF_INET;
  serverAddress.sin_addr.s_addr   = htonl(INADDR_ANY);
  serverAddress.sin_port          = htons((unsigned short) SERVER_PORT);

  // Bind
  status = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
  if (status < 0) {
    printf("*** SERVER ERROR: Could not bind socket.\n");
    exit(-1);
  }

  // Line-up to handle up to 10 clients in line
  status = listen(serverSocket, 10);
  if (status < 0) {
    printf("*** SERVER ERROR: Could not listen on socket.\n");
    exit(-1);
  }

  // Wait for clients now
  while (environment.shutDown == 0) {
    addrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &addrSize);
    if (clientSocket < 0) {
      printf("ERROR:ACCEPTING CLIENT\n");
      exit(-1);
    }
    printf("SERVER: CLIENT CONNECTED\n");

    while (1) {
      bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0);
      printf("SEVER: RECIEVED CLIENT MESSAGE %s\n", buffer);
      buffer[bytesRcv] = 0;
      if( strcmp(buffer, "1") == 0 ){
        if(environment.numRobots < MAX_ROBOTS){
          int check = 0;
          int counter =0;

          environment.robots[environment.numRobots].x = (float) ((rand() % ((ENV_SIZE - ROBOT_RADIUS) - ROBOT_RADIUS + 1)) + ROBOT_RADIUS);
          environment.robots[environment.numRobots].y = (float) ((rand() % ((ENV_SIZE - ROBOT_RADIUS) - ROBOT_RADIUS + 1)) + ROBOT_RADIUS);
          environment.robots[environment.numRobots].direction = (float) ((rand() % (360 + 1)) + 0);


          for(int i=0; i<environment.numRobots; i++){
            if( i != environment.numRobots){
              int colDist = ROBOT_RADIUS + ROBOT_RADIUS;
              int actDist = sqrt( pow( (environment.robots[i].x - environment.robots[environment.numRobots].x), 2) + pow((environment.robots[i].y - environment.robots[environment.numRobots].y), 2) );
              if( actDist < colDist){
                check = 1;
              }
            }
          }
          if (check ==0){
            environment.robots[environment.numRobots].x = (float) ((rand() % ((ENV_SIZE - ROBOT_RADIUS) - ROBOT_RADIUS + 1)) + ROBOT_RADIUS);
            environment.robots[environment.numRobots].y = (float) ((rand() % ((ENV_SIZE - ROBOT_RADIUS) - ROBOT_RADIUS + 1)) + ROBOT_RADIUS);
          }

          char nString[10], xString[10], yString[10], dString[10];
          gcvt((float) environment.numRobots, 3, nString);
          gcvt(environment.robots[environment.numRobots].x, 8, xString);
          gcvt(environment.robots[environment.numRobots].y, 8, yString);
          gcvt(environment.robots[environment.numRobots].direction, 8, dString);

          strcpy(response, "4");
          strcat(response, "^");
          strcat(response, nString);
          strcat(response, "^");
          strcat(response, xString);
          strcat(response, "^");
          strcat(response, yString);
          strcat(response, "^");
          strcat(response, dString);
          strcat(response, "^");

          printf("SEVER: SENDING MESSAGE %s\n", response);
          send(clientSocket, response, strlen(response), 0);
          environment.numRobots++;

          break;

        }else{

          strcpy(response, "5");
          //printf("SEVER: SENDING MESSAGE %s\n", response);
          send(clientSocket, response, strlen(response), 0);
          //break;
        }

      }else if( buffer[0] == '3'){
        char *token =  strtok(buffer, "^");
        char bResponse[4];
        strcpy(bResponse, token);
        token = strtok(NULL, "^");
        char robotNum[4];
        strcpy(robotNum, token);
        token = strtok(NULL, "^");
        char xValue[14];
        strcpy(xValue, token);
        token = strtok(NULL, "^");
        char yValue[14];
        strcpy(yValue, token);
        token = strtok(NULL, "^");
        char dir[14];
        strcpy(dir, token);
        token = strtok(NULL, "^");

        if( (atoi(xValue) <= 0 || atoi(xValue) >= ENV_SIZE) || (atoi(yValue) <= 0 || atoi(yValue) >= ENV_SIZE) ){
          char nokBound[4] = "6";
          printf("SEVER: BOUNDARY COLLISION\n");
          send(clientSocket, nokBound, sizeof(nokBound), 0);
          //environment.robots[atoi(robotNum)].direction = environment.robots[atoi(robotNum)].direction - 90 ;
        }else {
          //printf("buffer met 3: %s %s\n", xValue, yValue);
          char notkCollide[4] = "7";
          char ok[4] = "4";
          int check = 0;


          for(int i=0; i<environment.numRobots; i++){
            if( i != atoi(robotNum)){
              int colDist = ROBOT_RADIUS + ROBOT_RADIUS;
              int actDist = sqrt( pow( (environment.robots[i].x - atoi(xValue)), 2) + pow((environment.robots[i].y - atoi(yValue)), 2) );
              if( actDist < colDist){
                check = 1;
              }
            }
          }

          if(check ==1){
            printf("SEVER: ROBOT COLLISION\n");
            send(clientSocket, notkCollide, sizeof(notkCollide), 0);


          }else{
            environment.robots[atoi(robotNum)].x = atoi(xValue);
            environment.robots[atoi(robotNum)].y = atoi(yValue);
            environment.robots[atoi(robotNum)].direction = atoi(dir);
            printf("SEVER: OK TO MOVE\n");
            send(clientSocket, ok, sizeof(ok), 0);

          }




        }



        break;
      }







      if (strcmp(buffer,"2") == 0){
        break;
      }
      //break;

    }



    printf("SERVER: Closing client connection.\n");

    close(clientSocket); // Close this client's socket

    // If the client said to stop, then I'll stop myself
    if (strcmp(buffer,"2") == 0){
      environment.shutDown = 1;
      break;
    }

  }
  close(serverSocket);

}



int main() {
  // So far, the environment is NOT shut down
  environment.shutDown = 0;



  // Set up the random seed
  srand(time(NULL));






  // Spawn threads to handle incoming requests and
  // update the display
  pthread_t    t1, t2;

  pthread_create(&t1, NULL, handleIncomingRequests, &environment);
  pthread_create(&t2, NULL,                 redraw, &environment);

  // Wait for the threads to complete
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  printf("X:%f\n", environment.robots[0].x );
  printf("Y:%f\n", environment.robots[0].y );
  printf("DIR:%d\n", environment.robots[0].direction );

  printf("ENVIRONMENT: Shutting down.\n");

  exit(-1);

}
