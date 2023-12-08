/*
 * serverTest.c - server unit test module that tests server functionality
                  tests normal gameplay and then attempts to send erroneous messsages
                  Note: must run clientTest.c to connect to server
 * 
 * usage: ./serverTest <port>
 *
 * Daniel Lampert, Fall 2022
 * 
 */

#include "network.h"

/* *****************************************
* The main function.
*/
int main(const int argc, char* argv[]) {

   // if improper # of args, exit 1
   if (argc != 2) {
      fprintf(stderr, "usage: ./serverTest <port>\n");
      exit(1);
   }

   // if second arg is not an int, exit 2
   int PORT = 0;
   if(sscanf(argv[1], "%d", &PORT) != 1) {
      fprintf(stderr, "usage: ./serverTest <port>\n");
      exit(2);
   }

   server_t* server = startServer(PORT);
   // if server does not start, exit code 3
   if (server == NULL) {
      fprintf(stderr, "unable to establish server\n");
      exit(3);
   }

   printf("Simulating game run through w/ valid messages.\n");

   // read for "JOIN <playername>" message
   char* clientMessage = readClientMessage(server);
   if (clientMessage != NULL) {
      printf("Message from client: %s\n", clientMessage);
      free(clientMessage);
   }

   // send "BEGIN" message
   if (!sendToClient(server, "BEGIN")) {
      fprintf(stderr, "error sending messsage to client\n");
   }
   // send "CARD" message
   if (!sendToClient(server, "CARD Jack of Hearts\0")) {
      fprintf(stderr, "error sending messsage to client\n");
   }
   // send another "CARD" message
   if (!sendToClient(server, "CARD Seven of Diamonds\0")) {
      fprintf(stderr, "error sending messsage to client\n");
   }
   // send "DEALER" message
   if (!sendToClient(server, "DEALER Three of Clubs")) {
      fprintf(stderr, "error sending messsage to client\n");
   }
   // send "DECISION" message
   if (!sendToClient(server, "DECISION")) {
      fprintf(stderr, "error sending messsage to client\n");
   }

   // read for "HIT/STAND" message
   clientMessage = readClientMessage(server);
   if (clientMessage != NULL) {
      printf("Message from client: %s\n", clientMessage);
      free(clientMessage);
   }

   // send "RESULT WIN" message
   if (!sendToClient(server, "RESULT WIN")) {
      fprintf(stderr, "error sending messsage to client\n");
   }

   // send "QUIT" message
   if (!sendToClient(server, "RESULT WIN")) {
      fprintf(stderr, "error sending messsage to client\n");
   }

   // test a number of invalid messages for server
   printf("\nTesting 16 invalid messages for dealer.\n");
   char* invalidDealerMessages[16] = {"JOIN", "JOIN Player1", "HIT", "STAND", "BEGIN 1", "CARD", "CARD Jack of Hears",
                                     "CARD Jock of Hearts", "CARD Jack ooof Hearts", "DEALER", "DECISION 123", 
                                     "RESULT!@#$", "RESULT POSH", "QUIeT", "FAKECOMMAND", "FAKE COMMAND"};
   for (int i = 0; i < 16; i++) {
      if (!sendToClient(server, invalidDealerMessages[i])) {
         fprintf(stderr, "error sending messsage %d to client: %s\n", (i+1), invalidDealerMessages[i]);
      }
   }

   // shut down server   
   closeServer(server);
}