#ifndef TESTCASES_HPP_INCLUDED
#define TESTCASES_HPP_INCLUDED

#include <queue>

#include "packet.hpp"
#include "debugmsg.hpp"

using namespace std;

/* Test 1: Packet sent successfully */
void success_pktinorder();

/* Test 2: Packet sent out of order successfully */
queue<DATA_PKT> success_pktoutoforder(queue<DATA_PKT> pkts);

/* Test 3: Corrupted packet received at receiver */
queue<DATA_PKT> error_corruptdata(queue<DATA_PKT> pkts);

/* Test 4: Packet never received from receiver */
void error_losspkttoreceiver(queue<DATA_PKT> pkts);

/* Test 5: Sender meets max timeout */
void error_maxtimeoutretry(queue<DATA_PKT> pkts);

/* Validate a correct test case has been inputted. */
bool isValidTestCase(char *testCase);

#endif
