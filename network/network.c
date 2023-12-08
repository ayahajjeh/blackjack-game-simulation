/*
 * index.c - implementation of 'network' module
 *           Adapted from https://www.cs.dartmouth.edu/~tjp/cs50/project/client.c
 *           and https://www.cs.dartmouth.edu/~tjp/cs50/project/server.c 
 *
 * Daniel Lampert, Fall 2022
 * 
 */

#include <ctype.h>
#include <time.h>
#include "network.h"

// server function prototypes
server_t* startServer(const int PORT);
char* readClientMessage(server_t* server);
bool sendToClient(server_t* server, char* message);
void closeServer(server_t* server);

// client function prototypes
client_t* startClient(const char *server_addr, const int PORT);
char* readServerMessage(client_t* client);
bool sendToServer(client_t* client, char* message);
void closeClient(client_t* client);

// local function prototypes
static bool isValidMessage(char* message, char* type);
int nanosleep(const struct timespec *req, struct timespec *rem);


/**************** Server Functions ****************/

/**************** startServer ****************/
/* Set up server socket to listen for client connections
 * If connection made, returns server struct, or NULL if error.
 * 
 * Note: Caller MUST call closeServer to free memory.
 */
server_t* startServer(const int PORT) {

   server_t* server = malloc(sizeof(server_t));

   if (server == NULL) {
      return NULL;
   }

   int opt = 1;
   int addrlen = sizeof(server->address);

   // Creating socket file descriptor
   if ((server->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
      perror("socket failed");
      exit(EXIT_FAILURE);
   }

   // Attaching socket to the PORT
    if (setsockopt(server->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(PORT);

    // Binding socket to the PORT
    if (bind(server->server_fd, (struct sockaddr*)&(server->address), sizeof(server->address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server->server_fd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    fflush(stdout); //ensure line above prints before waiting for connection
    if ((server->new_socket = accept(server->server_fd, (struct sockaddr*)&(server->address), (socklen_t*)&addrlen)) < 0) {
        printf("\n");
        perror("accept");
        exit(EXIT_FAILURE);
    }

    return server;
}

/**************** readClientMessage ****************/
/* Reads for incoming message from client.
 * 
 * Note: Caller MUST free the message.
 */
char* readClientMessage(server_t* server) {
   char buffer[1024] = { 0 };
   int valread = read(server->new_socket, buffer, 1024);
   if (valread < 0) {
      perror("reading message");
      exit(EXIT_FAILURE);
   }
   char* message = malloc(sizeof(buffer)+1);
   strcpy(message, buffer);
   return message;
}

/**************** sendToClient ****************/
/* Sends a message to client
 * Returns false if invalid message, otherwise return true
 */
bool sendToClient(server_t* server, char* message) {
   if (isValidMessage(message, "server")) {
      send(server->new_socket, message, strlen(message), 0);
      struct timespec remaining;
      struct timespec request = {0, 50*1000000};
      if (nanosleep(&request, &remaining) != 0) {
         fprintf(stderr, "unable to nanosleep\n");
      }
      return true;
   }
   return false;
}

/**************** closeServer ****************/
/* Closes connected socket and listening socket,
 * frees server struct.
 */
void closeServer(server_t* server) {
   // closing the connected socket
   close(server->new_socket);
   // closing the listening socket
   shutdown(server->server_fd, SHUT_RDWR);

   free(server);
}

/**************** Client Functions ****************/

/**************** startServer ****************/
/* Set up client socket to connect to server at server_addr and PORT.
 * If connection made, returns client struct, or NULL if error.
 * 
 * Note: Caller MUST call closeClient to free memory.
 */
client_t* startClient(const char *server_addr, const int PORT) {

   if (server_addr == NULL) {
      return NULL;
   }

   client_t* client = malloc(sizeof(client_t));
   client->sock = 0;

   if ((client->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("\n Socket creation error \n");
      free(client);
      return NULL;
   }

   client->serv_addr.sin_family = AF_INET;
   client->serv_addr.sin_port = htons(PORT);

   // Convert IPv4 and IPv6 addresses from text to binary form
   if (inet_pton(AF_INET, server_addr, &(client->serv_addr).sin_addr) <= 0) {
      perror("\nInvalid address/ Address not supported \n");
      free(client);
      return NULL;
   }

   if ((client->client_fd = connect(client->sock, (struct sockaddr*)&(client->serv_addr), sizeof(client->serv_addr))) < 0) {
      perror("\nConnection Failed \n");
      free(client);
      return NULL;
   }
   return client;
}

/**************** readServerMessage ****************/
/* Reads for incoming message from server.
 * 
 * Note: Caller MUST free the message.
 */
char* readServerMessage(client_t* client) {
   char buffer[1024] = { 0 };
   read(client->sock, buffer, 1024);
   char* message = malloc(sizeof(buffer)+1);
   strcpy(message, buffer);
   return message;
}

/**************** sendToServer ****************/
/* Sends a message to server
 * Returns false if invalid message, otherwise return true
 */
bool sendToServer(client_t* client, char* message) {
   if (isValidMessage(message, "client")) {
      send(client->sock, message, strlen(message), 0);
      struct timespec remaining;
      struct timespec request = {0, 50*1000000};
      if (nanosleep(&request, &remaining) != 0) {
         fprintf(stderr, "unable to nanosleep\n");
      }
      return true;
   }
   return false;
}

/**************** closeClient ****************/
/* Closes connected client socket
 * frees client struct.
 */
void closeClient(client_t* client) {
   close(client->client_fd);
   free(client);
}

/**************** isValidMessage ****************/
/* function that checks for valid messages from client/server.
 * checks for each type of message for "server" and "client"
 * returns true if found matching valid message structure, otherwise false
 */
static bool isValidMessage(char* message, char* type) {
   if (message == NULL || type == NULL) {
      return false;
   }

   bool result = false;
   
   char* serverMessages[3] = {"BEGIN", "DECISION", "CLOSE"};
   char* clientMessages[2] = {"HIT", "STAND"};
   char* cardSuits[4] = {"Hearts", "Clubs", "Diamonds", "Spades"};
   char* cardRanks[13] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine",
                           "Ten", "Jack", "Queen", "King"};

   char* firstWord = malloc(strlen(message)+1);
   if (firstWord == NULL) {
      return false;
   }
   strcpy(firstWord, message);
   // once it reaches a space, insert a null terminator to only evaluate first word of message
   for (char* ptr = firstWord; *ptr != '\0'; ptr++) {
      if (isspace(*ptr)) {
         *ptr = '\0';
         break;
      }
   }

   // server has special commands "CARD" and "DEALER" that pass more information
   if (strcmp(type, "server") == 0) {
      for (int i = 0; i < (sizeof(serverMessages)/sizeof(char*)); i++) {
         if (strcmp(message, serverMessages[i]) == 0) {
            result = true;
         }
         // if "CARD" or "DEALER", check that it passes valid card
         else if (strcmp(firstWord, "CARD") == 0 || strcmp(firstWord, "DEALER") == 0) {
            char* rank = malloc(strlen("Three")+1);
            char* suit = malloc(strlen("Diamonds")+1);
            char* buffer = malloc(strlen(message)+1);
            if (rank == NULL || suit == NULL || buffer == NULL) {
               return false;
            }
            // grab suit and rank if an extended command
            if (sscanf(message, "%s %s of %s", buffer, rank, suit) == 3) {
               bool validSuit = false;
               for (int i = 0; i < sizeof(cardSuits)/sizeof(char*); i++) {
                  if (strcmp(suit, cardSuits[i]) == 0) {
                     validSuit = true;
                  }
               }
               if (validSuit) {
                  for (int i = 0; i < sizeof(cardRanks)/sizeof(char*); i++) {
                     if (strcmp(rank, cardRanks[i]) == 0) {
                        result = true;
                     }
                  }
               }
            }
            free(buffer);
            free(suit);
            free(rank);
         }
         // if first word is "RESULT", check for second word of "WIN", "LOOSE", or "PUSH"
         else if (strcmp(firstWord, "RESULT") == 0) {
            char* nextWord = malloc(strlen("LOOSE")+1);
            if (nextWord == NULL) {
               return false;
            }
            if (sscanf(message, "RESULT %s", nextWord) == 1) {
               if (strcmp(nextWord, "WIN") == 0 || strcmp(nextWord, "LOOSE") == 0 || strcmp(nextWord, "PUSH") == 0) {
                  result = true;
               }
            }
            free(nextWord);
         }
      }

   // iterate through potential client instructions
   } else if (strcmp(type, "client") == 0) {
      for (int i = 0; i < sizeof(clientMessages)/sizeof(char*); i++) {
         if (strcmp(message, clientMessages[i]) == 0) {
            result = true;
         }
      }
      // check for a playername given after "JOIN"
      if (strcmp(firstWord, "JOIN") == 0) {
         char* playerName = malloc(strlen(message)-strlen(firstWord)+1);
         if (playerName == NULL) {
            return false;
         }
         if (sscanf(message, "JOIN %s", playerName) == 1) {
            result = true;
         }
         free(playerName);
      }
   }
   free(firstWord);
   return result;
}

