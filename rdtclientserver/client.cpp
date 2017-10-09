/*
 * Emmanuel A. Castillo
 * Brooke Thielen
 *
 * Reliable Data Transfer UDP
 * ---------------------------------
 * This application is a component of
 * a full project that consist of both
 * a client and server. In this case,
 * it is the client that is used to
 * request data to the server.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>

#include "rdt.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ip_address> <port> <msg>\n", argv[0]);
        return -1;
    }

    /* Open a UCP based socket connection */
    int sockfd;
    sockfd = rdt_socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "Error opening socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return -1;
    }

    /* In C++ this is required from the client to properly send data to the server.   *
     * The client socket is advertised as broadcast that permits it to send a request *
     * to a server.                                                                   */
    int broadcastEnable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) == -1) {
        fprintf(stderr, "Error sending socket to broadcast mode, errno = %d (%s) \n", errno, strerror(errno));
        rdt_close(sockfd);
        return -1;
    }

    /* Configure server information for the client to send a request to. */
    char* addr = argv[1];
    char* port = argv[2];
    struct sockaddr_in saddr;
    memset((void *)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(addr);
    saddr.sin_port = htons(atoi(port));

    /* Set up request data that the client will send to the server. */
    char *msg = argv[3];

    /* Utilize implemented RDT UDP send to function to send data to server. */
    if (rdt_sendto(sockfd, msg, strlen(msg), 0,(struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
    //if (rdt_sendto(sockfd, msg, strlen(msg), 0,(struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
        fprintf(stderr, "Error sending udp pkt to server, errno = %d (%s) \n", errno, strerror(errno));
        rdt_close(sockfd);
        return -1;
    }

    /* Close socket after successful request to server or destroyed connection. */
    rdt_close(sockfd);

    return 0;
}
