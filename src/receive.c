#include "receive.h"
#include "network_utils.h"




//receive進程
int receive(int log_on, char *argv[]){
	//初始化log資訊
	log_data_t log_data;
	log_data.on = log_on;
	log_data.process_name = "receive";
	log_data.pid = getpid();
	log_data.color = 2;

	if(log_data.on == 1){
		log_head(&log_data);
		printf("I am receive process\n");
	}


	//取得 listen socket
	int listen_fd = get_listen_socket(&log_data, 1);
	//填寫 server ip
	sockaddr_in_t server_addr = set_server_addr(&log_data, argv[2], argv[4]);
	//綁定 listen socket 和 ip
	listen_socket_bind_address(&log_data, listen_fd, &server_addr);
	//監聽 listen socket
	start_listen(&log_data, listen_fd, 5);
	//接受 client端連線
	client_t *client = accept_client(&log_data, listen_fd);

	char buffer[] = "hi I am compute server receive port";
	send(client->fd, buffer, sizeof(buffer), 0);

	//char recv_buffer[1024];
	//recv(client->fd, recv_buffer, sizeof(recv_buffer), 0);
	//printf("%s", recv_buffer);

	close(client->fd);
	free(client);
	return 0;
}
