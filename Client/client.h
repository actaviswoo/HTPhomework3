#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "RRCConnectionRequest.h"
#include "RRCConnectionSetup.h"
#include "RRCConnectionSetupComplete.h"

int create_client_socket(const char *server_ip, int server_port);
void send_message(int sockfd, uint8_t *buffer, ssize_t len);
ssize_t receive_message(int sockfd, uint8_t *buffer, size_t buflen);
void send_RRCConnectionRequest(int sockfd);
void receive_RRCConnectionSetup(int sockfd);
void send_RRCConnectionSetupComplete(int sockfd);

#endif // CLIENT_H