/*
 * Emmanuel A. Castillo
 * Brooke Thielen
 *
 * Reliable Data Transfer UDP
 * ---------------------------------
 * This set of code handles the creation
 * and handling of packets.
 */

#include <math.h>
#include <string.h>
#include <stdio.h>

#include "packet.hpp"
#include "cksum.hpp"

/* Create packets based on application layer data. */
queue<DATA_PKT> make_pkts(char *buffer, int buffer_length)
{
    queue<DATA_PKT> pkts;

    /* Set up information that determines the amount of packets that will created.*/
    int current_buff_length = buffer_length;
    int num_pkts = (buffer_length - DATA_BUFF) <= 0 ? 1 : ceil((double)buffer_length / (double)DATA_BUFF);

    /* Declare variables used for packet information. */
    int seq_num = 1;
    int data_len;

    /* Create packet based on application layer data and adding header information. */
    do
    {
        data_len = current_buff_length > DATA_BUFF ? DATA_BUFF : current_buff_length;

        DATA_PKT pkt;
        pkt.hlen = DATA_HEAD_SZ;
        pkt.dlen = data_len;
        pkt.numSeg = num_pkts;
        pkt.seqno = seq_num;

        if(seq_num < num_pkts) {
            strncpy(pkt.data, buffer, data_len);
            pkt.data[data_len] = '\0';
            buffer += data_len;
        } else {
            strcpy(pkt.data, buffer);
        }

        pkt.cksum = create_cksum(pkt.data, pkt.dlen);
        pkts.push(pkt);

        current_buff_length = current_buff_length - DATA_BUFF;
        seq_num++;
    }
    while(seq_num <= num_pkts);

    return pkts;
}

/* Generates an ACK packet to be sent to the receiver, verifying its packet has been received. */
ACK_PKT make_ackpkt(uint32_t ackno)
{
    char ackChar[10]; /* 11 bytes: 10 for the digits, 1 for the null character */
    sprintf( ackChar, "%u", ackno );

    ACK_PKT pkt;
    pkt.cksum = create_cksum(ackChar, sizeof(ackno));
    pkt.hlen = ACK_HEAD_SZ;
    pkt.ackno = ackno;

    return pkt;
}

/* Checks whether the received packet has been received already based on sequence number. */
bool isSeqExist(deque<DATA_PKT> recv_pkts, DATA_PKT pkt) {
    while(!recv_pkts.empty()) {
        if(recv_pkts.front().seqno == pkt.seqno) {
            return true;
        }
        recv_pkts.pop_front();
    }
    return false;
}
