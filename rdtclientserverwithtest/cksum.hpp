/*
 * Emmanuel A. Castillo
 * Brooke Thielen
 *
 * Reliable Data Transfer UDP
 * ---------------------------------
 * This provides to prototypes in
 * creating and validating checksums
 * for the packets.
 */

#ifndef CKSUM_HPP_INCLUDED
#define CKSUM_HPP_INCLUDED

#include <netinet/in.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "rdt.hpp"


unsigned short create_cksum(char *buff, int nbytes);

bool validate_cksum(char *buff, int nbytes, unsigned short checksum);

#endif
