#include "client.h"

int create_client_socket(const char *server_ip, int server_port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &servaddr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}


void send_message(int sockfd, uint8_t *buffer, ssize_t len) {
    int ret = send(sockfd, buffer, len, 0);
    if (ret < 0) {
        perror("Error sending message");
        exit(EXIT_FAILURE);
    }
}

ssize_t receive_message(int sockfd, uint8_t *buffer, size_t buflen) {
    ssize_t len = recv(sockfd, buffer, buflen, 0);
    if (len < 0) {
        perror("Error receiving message");
        exit(EXIT_FAILURE);
    }
    return len;
}

void send_RRCConnectionRequest(int sockfd) {
    RRCConnectionRequest_t rrc_request;
    memset(&rrc_request, 0, sizeof(RRCConnectionRequest_t));
    rrc_request.criticalExtensions.present = RRCConnectionRequest__criticalExtensions_PR_rrcConnectionRequest_r8;
    rrc_request.criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.present = InitialUE_Identity_PR_randomValue;
    uint8_t buffer[1024];
    asn_enc_rval_t enc_ret = der_encode_to_buffer(&asn_DEF_RRCConnectionRequest, &rrc_request, buffer, sizeof(buffer));
    if (enc_ret.encoded == -1) {
        fprintf(stderr, "Encoding failed\n");
        exit(EXIT_FAILURE);
    }
    send_message(sockfd, buffer, enc_ret.encoded);
}

void receive_RRCConnectionSetup(int sockfd) {
    uint8_t buffer[1024];
    ssize_t len = receive_message(sockfd, buffer, sizeof(buffer));
    RRCConnectionSetup_t *rrc_setup = NULL;
    asn_dec_rval_t dec_ret = asn_decode(NULL, ATS_DER, &asn_DEF_RRCConnectionSetup, (void **)&rrc_setup, buffer, len);
    if (dec_ret.code != RC_OK) {
        fprintf(stderr, "Decoding failed\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Received RRC Connection Setup\n");
}

void send_RRCConnectionSetupComplete(int sockfd) {
    RRCConnectionSetupComplete_t rrc_setup_complete;
    memset(&rrc_setup_complete, 0, sizeof(RRCConnectionSetupComplete_t));
    rrc_setup_complete.rrc_TransactionIdentifier = 123;
    rrc_setup_complete.criticalExtensions.present = RRCConnectionSetupComplete__criticalExtensions_PR_c1;
    rrc_setup_complete.criticalExtensions.choice.c1.present = RRCConnectionSetupComplete__criticalExtensions__c1_PR_rrcConnectionSetupComplete_r8;
    uint8_t buffer_complete[1024];
    asn_enc_rval_t enc_ret_complete = der_encode_to_buffer(&asn_DEF_RRCConnectionSetupComplete, &rrc_setup_complete, buffer_complete, sizeof(buffer_complete));
    if (enc_ret_complete.encoded == -1) {
        fprintf(stderr, "Encoding failed\n");
        exit(EXIT_FAILURE);
    }
    send_message(sockfd, buffer_complete, enc_ret_complete.encoded);
}