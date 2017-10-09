/*
 * Emmanuel A. Castillo
 * Brooke Thielen
 *
 * Reliable Data Transfer UDP
 * ---------------------------------
 * This provides the prototypes
 * and struct information for
 * packets that will be sent
 * either from receiver or sender.
 */

#ifndef PACKET_HPP_INCLUDED
#define PACKET_HPP_INCLUDED

#include <stdint.h>
#include <queue>

#define DATA_HEAD_SZ 96
#define ACK_HEAD_SZ 64
#define DATA_BUFF 5

using namespace std;

struct DATA_PKT {
    uint16_t cksum;         /* check sum */
    uint16_t hlen;          /* header length */
    uint16_t dlen;          /* data length */
    uint16_t numSeg;        /* number of segments */
    uint32_t seqno;         /* sequence number */
    char data[DATA_BUFF];    /* application layer data */

    /* Used to organize data packet by < operation. */
    bool operator<(const DATA_PKT& rhs) const
    {
        return seqno < rhs.seqno;
    }
    /* Used to organize data packet by == operation. */
    bool operator==(const DATA_PKT& pkt) const
    {
        return seqno == pkt.seqno;
    }
};

struct ACK_PKT {
    uint16_t cksum; /* check sum */
    uint16_t hlen;  /* header length */
    uint32_t ackno; /* acknowledge number, which is based on sequence number. */
};

queue<DATA_PKT> make_pkts(char *buffer, int buffer_length);

ACK_PKT make_ackpkt(uint32_t ackno);

bool isSeqExist(deque<DATA_PKT> recv_pkts, DATA_PKT pkt);

#endif
