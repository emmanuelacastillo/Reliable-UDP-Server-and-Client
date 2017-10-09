/*
 * Emmanuel A. Castillo
 * Brooke Thielen
 *
 * Reliable Data Transfer UDP
 * ---------------------------------
 * The checksum implementation is
 * handled here. In addition,
 * it provides a method in validating
 * checksum.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>

using namespace std;

/* Create check sum. */
unsigned short create_cksum(char *buff, int nbytes)
{
    unsigned int sum = 0;

    while(nbytes > 1) {
        sum = sum + *(unsigned short *)buff;
        buff++;
        nbytes -= 2;
    }

    if (nbytes > 0) {
        sum = sum + *((unsigned char *) buff);
    }

    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return ((unsigned short)~sum);
}

/* Validate check sum. */
bool validate_cksum(char *buff, int nbytes, unsigned short checksum)
{
    unsigned short val_check = create_cksum(buff, nbytes);

    if (val_check != checksum) {
        cout << "Checksum is invalid, the validation is " << val_check << " and packet checksum is " << checksum << "\n\n";
        return false;
    }
    else
        cout << "Checksum is valid\n\n";

    return true;
}
