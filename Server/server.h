#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <RRCConnectionRequest.h>
#include <RRCConnectionSetup.h>
#include <RRCConnectionSetupComplete.h>

ssize_t receive_message(int sockfd, uint8_t *buffer, size_t buflen);
void send_message(int sockfd, uint8_t *buffer, ssize_t len);
int create_server_socket(int port);
int accept_connection(int sockfd);
void handle_RRCConnectionRequest(int connfd);
void send_RRCConnectionSetup(int connfd);
void handle_RRCConnectionSetupComplete(int connfd);

#endif // SERVER_H