#ifndef TIMEOUT_HPP_INCLUDED
#define TIMEOUT_HPP_INCLUDED

#include <netinet/in.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include "rdt.hpp"

#define TIME_OUT_SECS 2
#define MAX_TIMEOUT_RETRY 5

int callTimeout(int filedes, unsigned int seconds);

#endif
