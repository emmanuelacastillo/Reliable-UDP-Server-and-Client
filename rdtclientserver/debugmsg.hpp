/*
 * Emmanuel A. Castillo
 * Brooke Thielen
 *
 * Reliable Data Transfer UDP
 * ---------------------------------
 * The sole purpose of this header file
 * is to display packet information into
 * the console for debugging purposes.
 */

#ifndef DEBUGMSG_HPP_INCLUDED
#define DEBUGMSG_HPP_INCLUDED

#include <stdint.h>
#include <string.h>

#include "packet.hpp"

using namespace std;

void displayAppLayerMsg(uint16_t cksum, uint16_t hlen, uint16_t dlen, uint16_t numSeg, uint32_t seqno, char *data);

void displayRcvMsg(uint16_t cksum, uint16_t hlen, uint16_t dlen, uint16_t numSeg, uint32_t seqno, char *data);

void displaySendMsg(uint16_t cksum, uint16_t hlen, uint16_t dlen, uint16_t numSeg, uint32_t seqno, char *data);

void displaySendAckMsg(uint16_t cksum, uint16_t hlen, uint32_t ackno);

void displayRcvAckMsg(uint16_t cksum, uint16_t hlen, uint32_t ackno);

void displayPktInfo(DATA_PKT pkt);

void displayAckPktInfo(ACK_PKT pkt);

#endif
