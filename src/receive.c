#include "receive.h"

extern client_t *client;
extern log_data_t log_data;
extern char *recv_buffer;
extern char *send_buffer;
extern mqd_t receive_to_compute;
extern unsigned int prio;

//receive進程
int receive(int log_on, char *argv[]){
	//初始化log資訊
	log_data.on = log_on;
	log_data.process_name = "receive";
	log_data.pid = getpid();
	log_data.color = 2;

	if(log_data.on == 1){
		log_head(&log_data);
		printf("I am receive process\n");
	}



	//消息隊列
	if(mq_send(receive_to_compute, "ha", sizeof("ha"), 0) == -1){
		log_head(&log_data);
		perror("mq_send");
	}

	if(mq_receive(receive_to_compute, recv_buffer, atoi(argv[7]), &prio) == -1){
		log_head(&log_data);
		perror("mq_receive");
	}
	else{
		log_head(&log_data);
		printf("%s\n", recv_buffer);
		fflush(stdout);
	}




	//取得 listen socket
	int listen_fd = get_listen_socket(&log_data, 1);
	//填寫 server ip
	sockaddr_in_t server_addr = set_server_addr(&log_data, argv[2], argv[4]);
	//綁定 listen socket 和 ip
	listen_socket_bind_address(&log_data, listen_fd, &server_addr);
	//監聽 listen socket
	start_listen(&log_data, listen_fd, 5);
	


	char buffer[] = "hi I am compute server receive port";


	while(1){
		//接受 client端連線
		client = accept_client(&log_data, listen_fd);
		if(client != NULL){
			send(client->fd, buffer, sizeof(buffer), 0);

			while(1){
				if(recv(client->fd, recv_buffer, atoi(argv[7]), 0) == 0){
					log_head(&log_data);
					printf("connect close : %s %d\n", client->ip, client->port);
					break;
				}
				else{
	
					recv_buffer[strcspn(recv_buffer, "\n")] = '\0';
					recv_buffer[strcspn(recv_buffer, "\r")] = '\0';
					

					if(mq_send(receive_to_compute, recv_buffer, atoi(argv[7]), 0) == -1){
						log_head(&log_data);
						perror("mq_send");
					}
				}
			}
		}
	}
	return 0;
}
