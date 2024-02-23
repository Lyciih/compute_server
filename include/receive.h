#ifndef RECEIVE_H
#define RECEIVE_H

#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include "utils.h"
#include "network_utils.h"
#include "signal_handle.h"

int receive(int report_on, char *argv[]);



#endif
