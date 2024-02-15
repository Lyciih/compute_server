#include "present.h"
#include "network_utils.h"


extern client_t *client;
extern log_data_t log_data;

//present進程
int present(int log_on, char *argv[]){
	//初始化log資訊
	log_data.on = log_on;
	log_data.process_name = "present";
	log_data.pid = getpid();
	log_data.color = 3;

	if(log_data.on == 1){
		log_head(&log_data);
		printf("I am present process\n");
	}

	//set_signal_handle();

	//取得 listen socket
	int listen_fd = get_listen_socket(&log_data, 1);
	//填寫 server ip
	sockaddr_in_t server_addr = set_server_addr(&log_data, argv[2], argv[5]);
	//綁定 listen socket 和 ip
	listen_socket_bind_address(&log_data, listen_fd, &server_addr);
	//監聽 listen socket
	start_listen(&log_data, listen_fd, 5);
	
	
	char buffer[] = "hi I am compute server present port\n";
	char recv_buffer[1024];
	
	while(1){
		//接受 client端連線
		client = accept_client(&log_data, listen_fd);
		if(client != NULL){
			send(client->fd, buffer, sizeof(buffer), 0);

			while(1){
				recv(client->fd, recv_buffer, sizeof(recv_buffer), 0);
				printf("%s", recv_buffer);
			}
		}
	}

	return 0;
}
