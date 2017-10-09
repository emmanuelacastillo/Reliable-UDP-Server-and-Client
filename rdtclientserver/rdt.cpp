/*
 * Emmanuel A. Castillo
 * Brooke Thielen
 *
 * Reliable Data Transfer UDP
 * ---------------------------------
 * A reliable data transfer UDP is
 * implemented here. Existing socket, bind and closed
 * is used from the socket library. The new features are the following:
 * 1) Breaking up packets into smaller peices to be sent to the receiver
 * 2) Identifying packets with sequence numbers to handle duplicates
 *    and recombining split up packets together.
 * 3) Sending ACKs to signal a proper packet transfer.
 * 4) Identifying whether a packet or ACK was corrupted through a checksum.
 * 5) Handling instances where no ACK verification occurs through timeout and retransmission.
 */

#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <arpa/inet.h>
#include <queue>
#include <algorithm>

#include "rdt.hpp"
#include "packet.hpp"
#include "debugmsg.hpp"
#include "cksum.hpp"
#include "timeout.hpp"

using namespace std;

/* Initialize a socket for data communication. */
int rdt_socket(int address_family, int type, int protocol)
{
    return socket(address_family, type, protocol);
}

/* Binds a port for data communication. */
int rdt_bind(int socket_descriptor, const struct sockaddr *local_address, socklen_t address_length)
{
    return bind(socket_descriptor, local_address, address_length);
}

/* Data is received from a client and a packet is extracted and sent to the application layer. */
int rdt_recv(int socket_descriptor, char *buffer, int buffer_length, int flags, struct sockaddr *from_address, int *address_length)
{
    deque<DATA_PKT> recv_pkts;
    int statusCd;
    DATA_PKT recv_pkt;

    /* Below loop receives packets and sends an ACK after each obtained valid packet. */
    do
    {
        if (( statusCd = recvfrom(socket_descriptor, (void *)&recv_pkt, sizeof(recv_pkt), flags, from_address, (socklen_t *)address_length)) == -1) {
            fprintf(stderr, "Error on transport layer recvfrom, errno = %d (%s) \n", errno, strerror(errno));
            return statusCd;
        }

        /* Display packet information for debugging purposes on console. */
        displayRcvMsg(recv_pkt.cksum, recv_pkt.hlen, recv_pkt.dlen, recv_pkt.numSeg, recv_pkt.seqno, recv_pkt.data);

        /* Checks and if packet sequence does not exist, adds packet to packet queue for later processing and *
         * sends an ACK if this event is successful.                                                          */
        if(!isSeqExist(recv_pkts, recv_pkt)) {

            if (validate_cksum(recv_pkt.data, recv_pkt.dlen, recv_pkt.cksum)) {
                recv_pkts.push_front(recv_pkt);
                /* Create ACK packet to be sent. */
                ACK_PKT ackPkt = make_ackpkt(recv_pkt.seqno);

                /* Display packet information for debugging purposes on console. */
                displaySendAckMsg(ackPkt.cksum, ackPkt.hlen, ackPkt.ackno);

                /* Send ACK packet to sender. */
                int sendStatus;
                if((sendStatus = sendto(socket_descriptor, (void *)&ackPkt, sizeof(ackPkt), flags, from_address, (socklen_t)*address_length)) == -1) {
                    fprintf(stderr, "Error on transport layer recvfrom while sending ACK through sendto, errno = %d (%s) \n", errno, strerror(errno));
                    return sendStatus;
                }
            }
        }
    }
    while(recv_pkts.size() < recv_pkt.numSeg);

    /* Organize packet queue in correct order by its sequence number. */
    sort(recv_pkts.begin(), recv_pkts.end());

    /* Put packets together to be sent to application layer. */
    string combined_pkts = "";
    while(!recv_pkts.empty())
    {
        recv_pkt = recv_pkts.front();
        combined_pkts = combined_pkts + recv_pkt.data;
        recv_pkts.pop_front();
    }

    /* Set packet data portion to application layer buffer. */
    strcpy(buffer, combined_pkts.data());

    return 0;
}

inline int send_packets(int socket_descriptor, int flags, struct sockaddr *destination_address, int address_length, queue<DATA_PKT> pkts)
{
    DATA_PKT sending_pkt;

    /* Send each packet individually */
    while(!pkts.empty())
    {
        sending_pkt = pkts.front();
        pkts.pop();

        /* Display packet information for debugging purposes on console. */
        displaySendMsg(sending_pkt.cksum, sending_pkt.hlen, sending_pkt.dlen, sending_pkt.numSeg, sending_pkt.seqno, sending_pkt.data);

        /* Send packet to receiver. */
        int sendStatus = sendto(socket_descriptor, (void *)&sending_pkt, (sending_pkt.hlen + sending_pkt.dlen), flags, destination_address, address_length);

        if(sendStatus == -1)
            return sendStatus;
    }

    return 0;
}

/* Data is received from the application layer and packets are created and sent to the source */
int rdt_sendto(int socket_descriptor, char *buffer, int buffer_length, int flags, struct sockaddr *destination_address, int address_length)
{
    /* Create packets based on buffer size and packet size limit and send packets to receiver. */
    queue<DATA_PKT> pkts = make_pkts(buffer, buffer_length);

    if(send_packets(socket_descriptor, flags, destination_address, address_length, pkts) == -1)
        return -1;

    /* Wait for ACK validation to determine whether to resend packets or do nothing because the packets were sent successfully. */
    int statusCd;
    ACK_PKT ackPkt;
    uint32_t ackSeqNum = 1;
    uint32_t num_sending_pkts = pkts.size();

    int timeoutStatus;
    int timeOutCounter = 0;
    /* Wait for ACKs to verify a successful packet sent to receiver. If not verified, resubmit. */
    while(ackSeqNum <= num_sending_pkts)
    {
        timeoutStatus = callTimeout(socket_descriptor, TIME_OUT_SECS);
        if(timeoutStatus == 0)
        {
            cout << "Timeout occured. Resending packets.\n\n";
            timeOutCounter++;

            if(timeOutCounter == MAX_TIMEOUT_RETRY) {
                cout << "Max timeout retries met. Closing connection.\n";
                return 0;
            }

            /* Recreate packets based on buffer size and packet size limit and send packets to receiver. */
             queue<DATA_PKT> resent_pkts = make_pkts(buffer, buffer_length);
             if(send_packets(socket_descriptor, flags, destination_address, address_length, resent_pkts) == -1)
                 return -1;

            /* Reinitialize ACK verification veriables. */
            num_sending_pkts = resent_pkts.size();
            ackSeqNum = 1;
            timeoutStatus = callTimeout(socket_descriptor, TIME_OUT_SECS);
        }

        /* Receive expected ACKs from receiver. */
        if (( statusCd = recvfrom(socket_descriptor, (void *)&ackPkt, sizeof(ackPkt), flags, (struct sockaddr *)&destination_address, (socklen_t *)&address_length)) == -1) {
                fprintf(stderr, "Error on transport layer recvfrom, errno = %d (%s) \n", errno, strerror(errno));
            return statusCd;
        }

        /* Reinitialize timeout counter because ACK has been received.*/
        timeOutCounter = 0;

        /* Display packet information for debugging purposes on console. */
        displayRcvAckMsg(ackPkt.cksum, ackPkt.hlen, ackPkt.ackno);

        ackSeqNum++;
    }
    return 0;
}

/* Closes the communication socket. */
int rdt_close(int fildes)
{
    return close(fildes);
}
