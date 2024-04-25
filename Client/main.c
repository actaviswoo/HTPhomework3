#include "client.h"

int main() {
    int sockfd = create_client_socket("127.0.0.1", 12345);

    send_RRCConnectionRequest(sockfd);
    receive_RRCConnectionSetup(sockfd);
    send_RRCConnectionSetupComplete(sockfd);

    close(sockfd);
    return 0;
}