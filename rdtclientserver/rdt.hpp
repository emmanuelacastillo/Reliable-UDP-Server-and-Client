/*
 * Emmanuel A. Castillo
 * Brooke Thielen
 *
 * Reliable Data Transfer UDP
 * ---------------------------------
 * This provides the prototypes
 * of the RDT UDP implementation.
 */


#ifndef RDT_HPP_INCLUDED
#define RDT_HPP_INCLUDED

#include <netinet/in.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>


int rdt_socket(int address_family, int type, int protocol);

int rdt_bind(int socket_descriptor, const struct sockaddr *local_address, socklen_t address_length);

int rdt_recv(int socket_descriptor, char *buffer, int buffer_length, int flags, struct sockaddr *from_address, int *address_length);

int rdt_sendto(int socket_descriptor, char *buffer, int buffer_length, int flags, struct sockaddr *destination_address, int address_length);

int rdt_close(int fildes);

#endif
