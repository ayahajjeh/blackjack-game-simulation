/*
 * clientTest.c - client unit test module that tests client functionality
                  tests normal gameplay and then attempts to send erroneous messsages
                  Note: serverTest.c must be running first to connect
 * 
 * usage: ./clientTest <server_address> <port>
 * 
 * get ip address from command line with:
 * ip addr
 *  or
 * wget -O - -q https://checkip.amazonaws.com
 *
 * Daniel Lampert, Fall 2022
 * 
 */

#include "network.h"

/* *****************************************
* The main function.
*/
int main(const int argc, char* argv[]) {

   // ensure proper # of arguments
   if (argc != 3) {
      fprintf(stderr, "usage: ./clientTest <server_address> <port>\n");
      exit(1);
   }

   // grab second arg
   char* server_address = argv[1];

   // if third arg is not an int, exit 2
   int PORT = 0;
   if(sscanf(argv[2], "%d", &PORT) != 1) {
      fprintf(stderr, "usage: ./clientTest <server_address> <port>\n");
      exit(2);
   }

   client_t* client = startClient(server_address, PORT);

   // if client is null, exit 4
   if (client == NULL) {
      fprintf(stderr, "unable to connect client\n");
      exit(4);
   }

   printf("\nSimulating game run through w/ valid messages.\n");

   // send "JOIN <playername>" message
   if (!sendToServer(client, "JOIN player1")) {
      fprintf(stderr, "error sending messsage to server\n");
   }

   // read for "BEGIN" message
   char* serverMessage = readServerMessage(client);
   if (serverMessage != NULL) {
      printf("Message from server: %s\n", serverMessage);
      free(serverMessage);
   }

   // read for "CARD <rank> of <suit>" message
   serverMessage = readServerMessage(client);
   if (serverMessage != NULL) {
      printf("Message from server: %s\n", serverMessage);
      free(serverMessage);
   }

   // read for another "CARD <rank> of <suit>" message
   serverMessage = readServerMessage(client);
   if (serverMessage != NULL) {
      printf("Message from server: %s\n", serverMessage);
      free(serverMessage);
   }

   // read for "DEALER <rank> of <suit>" message
   serverMessage = readServerMessage(client);
   if (serverMessage != NULL) {
      printf("Message from server: %s\n", serverMessage);
      free(serverMessage);
   }

   // read for "DECISION" message
   serverMessage = readServerMessage(client);
   if (serverMessage != NULL) {
      printf("Message from server: %s\n", serverMessage);
      free(serverMessage);
   }

   // send "HIT" message
   if (!sendToServer(client, "HIT")) {
      fprintf(stderr, "error sending messsage to server\n");
   }

   // read for "RESULT <WIN|LOOSE|PUSH>" message
   serverMessage = readServerMessage(client);
   if (serverMessage != NULL) {
      printf("Message from server: %s\n", serverMessage);
      free(serverMessage);
   }

   // read for "QUIT" message
   serverMessage = readServerMessage(client);
   if (serverMessage != NULL) {
      printf("Message from server: %s\n", serverMessage);
      free(serverMessage);
   }

   // test a number of invalid messages for client
   printf("\nTesting 14 invalid messages for client.\n");
   char* invalidClientMessages[14] = {"BEGIN", "CARD", "CARD Jack of Hearts", "DEALER", "DEALER Seven of Clubs",
                                    "DECISION", "RESULT", "RESULT WIN", "QUIT", "JOIN", "HITSTAND", "HIT client",
                                    "FAKECOMMAND", "FAKE COMMAND"};
   for (int i = 0; i < 14; i++) {
      if (!sendToServer(client, invalidClientMessages[i])) {
         fprintf(stderr, "error sending messsage %d to server: %s\n", (i+1), invalidClientMessages[i]);
      }
   }
   
   // shut down client
   closeClient(client);
}