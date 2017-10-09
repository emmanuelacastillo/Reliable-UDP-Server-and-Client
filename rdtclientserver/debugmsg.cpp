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

#include <iostream>
#include <string.h>

#include "debugmsg.hpp"

using namespace std;

void displayAppLayerMsg(uint16_t cksum, uint16_t hlen, uint16_t dlen, uint16_t numSeg, uint32_t seqno, char *data)
{
    cout << "Receiving... \n";
    cout << "cksum: " << cksum << "\n";
    cout << "hlen: " << hlen << "\n";
    cout << "dlen: " << dlen << "\n";
    cout << "numSeg: " << numSeg << "\n";
    cout << "seqno: " << seqno << "\n";
    cout << "data: " << data << "\n";
    cout << "\n";
}

void displayRcvMsg(uint16_t cksum, uint16_t hlen, uint16_t dlen, uint16_t numSeg, uint32_t seqno, char *data)
{
    cout << "Receiving... \n";
    cout << "cksum: " << cksum << "\n";
    cout << "hlen: " << hlen << "\n";
    cout << "dlen: " << dlen << "\n";
    cout << "numSeg: " << numSeg << "\n";
    cout << "seqno: " << seqno << "\n";
    cout << "data: " << data << "\n";
    cout << "\n";
}

void displaySendMsg(uint16_t cksum, uint16_t hlen, uint16_t dlen, uint16_t numSeg, uint32_t seqno, char *data)
{
    cout << "Sending... \n";
    cout << "cksum: " << cksum << "\n";
    cout << "hlen: " << hlen << "\n";
    cout << "dlen: " << dlen << "\n";
    cout << "numSeg: " << numSeg << "\n";
    cout << "seqno: " << seqno << "\n";
    cout << "data: " << data << "\n";
    cout << "\n";
}

void displaySendAckMsg(uint16_t cksum, uint16_t hlen, uint32_t ackno)
{
    cout << "Sending ACK...\n";
    cout << "cksum: " << cksum << "\n";
    cout << "len: " << hlen << "\n";
    cout << "ackno: " << ackno << "\n";
    cout << "\n";
}

void displayRcvAckMsg(uint16_t cksum, uint16_t hlen, uint32_t ackno)
{
    cout << "Receiving ACK... \n";
    cout << "cksum: " << cksum << "\n";
    cout << "len: " << hlen << "\n";
    cout << "ackno: " << ackno << "\n";
    cout << "\n";
}

void displayPktInfo(DATA_PKT pkt)
{
    cout << "cksum: " << pkt.cksum << "\n";
    cout << "hlen: " << pkt.hlen << "\n";
    cout << "dlen: " << pkt.dlen << "\n";
    cout << "numSeg: " << pkt.numSeg << "\n";
    cout << "seqno: " << pkt.seqno << "\n";
    cout << "data: " << pkt.data << "\n";
    cout << "\n";
}

void displayAckPktInfo(ACK_PKT pkt)
{
    cout << "cksum: " << pkt.cksum << "\n";
    cout << "len: " << pkt.hlen << "\n";
    cout << "ackno: " << pkt.ackno << "\n";
    cout << "\n";
}
