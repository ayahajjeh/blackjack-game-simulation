/* 
 * network.h - header file for CS50 'network' module
               handles network interaction between servers and clients,
               provides all network functions for server (dealer) and clients (players).
 *
 * Daniel Lampert
 * CS 50, Fall 2022
 */


#ifndef __NETWORK_H
#define __NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <stdbool.h>

// server struct that holds all relevant data for server
typedef struct server {
   const int PORT;
   struct sockaddr_in address;
   int server_fd, new_socket;
} server_t;

// client struct that holds all relevant data for client
typedef struct client {
   const int PORT;
   int sock;
   int client_fd;
   struct sockaddr_in serv_addr;
} client_t;


/**************** Server Functions ****************/

/**************** startServer ****************/
/* Set up server socket to listen for client connections
 * If connection made, returns server struct, or NULL if error.
 * 
 * Note: Caller MUST call closeServer to free memory.
 */
server_t* startServer(const int PORT);

/**************** readClientMessage ****************/
/* Reads for incoming message from client.
 * 
 * Note: Caller MUST free the message.
 */
char* readClientMessage(server_t* server);

/**************** sendToClient ****************/
/* Sends a message to client
 * Returns false if invalid message, otherwise return true
 */
bool sendToClient(server_t* server, char* message);

/**************** closeServer ****************/
/* Closes connected socket and listening socket,
 * frees server struct.
 */
void closeServer(server_t* server);


/**************** Client Functions ****************/

/**************** startServer ****************/
/* Set up client socket to connect to server at server_addr and PORT.
 * If connection made, returns client struct, or NULL if error.
 * 
 * Note: Caller MUST call closeClient to free memory.
 */
client_t* startClient(const char *server_addr, const int PORT);

/**************** readServerMessage ****************/
/* Reads for incoming message from server.
 * 
 * Note: Caller MUST free the message.
 */
char* readServerMessage(client_t* client);

/**************** sendToServer ****************/
/* Sends a message to server
 * Returns false if invalid message, otherwise return true
 */
bool sendToServer(client_t* client, char* message);

/**************** closeClient ****************/
/* Closes connected client socket
 * frees client struct.
 */
void closeClient(client_t* client);


#endif //__NETWORK_H