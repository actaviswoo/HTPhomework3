#include "server.h"

int main() {
    int sockfd = create_server_socket(12345);
    int connfd = accept_connection(sockfd);

    handle_RRCConnectionRequest(connfd);
    send_RRCConnectionSetup(connfd);
    handle_RRCConnectionSetupComplete(connfd);

    close(connfd);
    close(sockfd);
    return 0;
}