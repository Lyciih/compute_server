#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

typedef struct {
	int on;
	const char* process_name;
	int pid;
} log_data_t;


int log_head(log_data_t* log_data);



#endif
