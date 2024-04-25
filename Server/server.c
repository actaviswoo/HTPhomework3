#include "server.h"

ssize_t receive_message(int sockfd, uint8_t *buffer, size_t buflen) {
    ssize_t len = recv(sockfd, buffer, buflen, 0);
    if (len < 0) {
        perror("Error receiving message");
        exit(EXIT_FAILURE);
    }
    return len;
}

void send_message(int sockfd, uint8_t *buffer, ssize_t len) {
    int ret = send(sockfd, buffer, len, 0);
    if (ret < 0) {
        perror("Error sending message");
        exit(EXIT_FAILURE);
    }
}

int create_server_socket(int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) < 0) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    return sockfd;
}

int accept_connection(int sockfd) {
    int connfd = accept(sockfd, NULL, NULL);
    if (connfd < 0) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    return connfd;
}

void handle_RRCConnectionRequest(int connfd) {
    uint8_t buffer[1024];
    ssize_t len = receive_message(connfd, buffer, sizeof(buffer));
    RRCConnectionRequest_t *rrc_request = NULL;
    asn_dec_rval_t dec_ret_request = asn_decode(NULL, ATS_DER, &asn_DEF_RRCConnectionRequest, (void **)&rrc_request, buffer, len);
    if (dec_ret_request.code != RC_OK) {
        fprintf(stderr, "Decoding RRC Connection Request failed\n");
        close(connfd);
        exit(EXIT_FAILURE);
    }
    printf("Received RRC Connection Request\n");
    ASN_STRUCT_FREE(asn_DEF_RRCConnectionRequest, rrc_request);
}

void send_RRCConnectionSetup(int connfd) {
    RRCConnectionSetup_t rrc_setup;
    memset(&rrc_setup, 0, sizeof(RRCConnectionSetup_t));
    rrc_setup.rrc_TransactionIdentifier = 123;
    rrc_setup.criticalExtensions.present = RRCConnectionSetup__criticalExtensions_PR_c1;
    rrc_setup.criticalExtensions.choice.c1.present = RRCConnectionSetup__criticalExtensions__c1_PR_rrcConnectionSetup_r8;
    uint8_t setup_buffer[1024];
    asn_enc_rval_t enc_ret_setup = der_encode_to_buffer(&asn_DEF_RRCConnectionSetup, &rrc_setup, setup_buffer, sizeof(setup_buffer));
    if (enc_ret_setup.encoded == -1) {
        fprintf(stderr, "Encoding RRC Connection Setup failed\n");
        close(connfd);
        exit(EXIT_FAILURE);
    }
    send_message(connfd, setup_buffer, enc_ret_setup.encoded);
    printf("Sent RRC Connection Setup\n");
}

void handle_RRCConnectionSetupComplete(int connfd) {
    uint8_t complete_buffer[1024];
    ssize_t len_complete = receive_message(connfd, complete_buffer, sizeof(complete_buffer));
    RRCConnectionSetupComplete_t *rrc_setup_complete = NULL;
    asn_dec_rval_t dec_ret_complete = asn_decode(NULL, ATS_DER, &asn_DEF_RRCConnectionSetupComplete, (void **)&rrc_setup_complete, complete_buffer, len_complete);
    if (dec_ret_complete.code != RC_OK) {
        fprintf(stderr, "Decoding RRC Connection Setup Complete failed\n");
        close(connfd);
        exit(EXIT_FAILURE);
    }
    printf("Received RRC Connection Setup Complete\n");
    ASN_STRUCT_FREE(asn_DEF_RRCConnectionSetupComplete, rrc_setup_complete);
}