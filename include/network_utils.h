#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include "utils.h"

typedef struct sockaddr_in sockaddr_in_t;

typedef struct {
	int fd;
	sockaddr_in_t addr;
	char ip[INET_ADDRSTRLEN];
	unsigned short port;
	socklen_t len;
} client_t;


int get_listen_socket(log_data_t *log_data, int ip_reuse);
int get_socket(log_data_t *log_data);
sockaddr_in_t set_server_addr(log_data_t *log_data, char *ip, char *port);
int listen_socket_bind_address(log_data_t *log_data, int listen_fd, sockaddr_in_t *server_addr);
int start_listen(log_data_t *log_data, int listen_fd, int backlog);
client_t* accept_client(log_data_t *log_data, int listen_fd);
int connect_server(log_data_t *log_data, int client_fd, sockaddr_in_t *server_addr);



#endif
