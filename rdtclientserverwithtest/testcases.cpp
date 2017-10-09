#include <iostream>
#include <queue>
#include <stdlib.h>

#include "packet.hpp"
#include "debugmsg.hpp"
#include "testcases.hpp"

using namespace std;

/* Test 1: Packet sent successfully */
void success_pktinorder()
{
    cout << "Running test case: SUCCESS_PKTINORDER\n\n";
}

/* Test 2: Packet sent out of order successfully */
queue<DATA_PKT> success_pktoutoforder(queue<DATA_PKT> pkts)
{
    if(pkts.size() < 3)
    {
        cout << "Test case requires 3 or more pkts\n";
        exit(1);
    }

    cout << "Running test case: SUCCESS_PKTOUTOFORDER\n";
    cout << "Packets are being reordered...\n";
    DATA_PKT pkt1 = pkts.front();
    pkts.pop();
    DATA_PKT pkt2 = pkts.front();
    pkts.pop();
    DATA_PKT pkt3 = pkts.front();
    pkts.pop();
    pkts.push(pkt2);
    pkts.push(pkt3);
    pkts.push(pkt1);

    return pkts;
}

/* Test 3: Corrupted packet received at receiver */
queue<DATA_PKT> error_corruptdata(queue<DATA_PKT> pkts)
{
    cout << "Running test case: ERROR_CORRUPTDATA\n";
    DATA_PKT corruptPkt = pkts.front();
    pkts.pop();
    corruptPkt.cksum = 0xff;
    cout << "Data has been corrupted.\n\n";
    displayPktInfo(corruptPkt);
    pkts.push(corruptPkt);

    return pkts;
}

/* Test 4: Packet never received from receiver */
void error_losspkttoreceiver(queue<DATA_PKT> pkts)
{
    cout << "Running test case: ERROR_LOSSPKTTORECEIVER\n";
    cout << "Packets below will not be sent to receiver.\n";
    while(!pkts.empty())
    {
        displayPktInfo(pkts.front());
        pkts.pop();
    }
    cout << "Packets are not sent.\n";
}

/* Test 5: Sender meets max timeout */
void error_maxtimeoutretry(queue<DATA_PKT> pkts)
{
    cout << "Running test case: ERROR_MAXTIMEOUTRETRY\n";
    cout << "Timing out 5 times simulating ACK not received after the following packets are sent.\n";
    while(!pkts.empty())
    {
        displayPktInfo(pkts.front());
        pkts.pop();
    }
}

/* Validate a correct test case has been inputted. */
bool isValidTestCase(char *testCase)
{
    if(!strcmp(testCase, "SUCCESS_PKTINORDER")
        && !strcmp(testCase, "SUCCESS_PKTOUTOFORDER")
        && !strcmp(testCase, "ERROR_CORRUPTDATA")
        && !strcmp(testCase, "ERROR_LOSSPKTTORECEIVER")
        && !strcmp(testCase, "ERROR_MAXTIMEOUTRETRY"))
        return false;

    return true;
}
