#include "present.h"
#include "network_utils.h"


extern client_t *client;
extern log_data_t log_data;
extern char *recv_buffer;
extern char *send_buffer;
extern mqd_t compute_to_present;
extern unsigned int prio;

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


	//消息隊列
	if(mq_send(compute_to_present, "la", sizeof("la"), 0) == -1){
		log_head(&log_data);
		perror("mq_send");
	}

	if(mq_receive(compute_to_present, recv_buffer, atoi(argv[7]), &prio) == -1){
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
	sockaddr_in_t server_addr = set_server_addr(&log_data, argv[2], argv[5]);
	//綁定 listen socket 和 ip
	listen_socket_bind_address(&log_data, listen_fd, &server_addr);
	//監聽 listen socket
	start_listen(&log_data, listen_fd, 5);
	
	
	char buffer[] = "hi I am compute server present port";
	char client_receive[1024];

	int have_data = 0;
	
	while(1){
		//接受 client端連線
		client = accept_client(&log_data, listen_fd);
		if(client != NULL){
			send(client->fd, buffer, sizeof(buffer), 0);

			while(1){
				if(have_data == 0){
					if(mq_receive(compute_to_present, recv_buffer, atoi(argv[7]), &prio) == -1){
						log_head(&log_data);
						perror("mq_receive");
					}
					else{
						have_data = 1;
					}
				}
				else{
					send(client->fd, "test", sizeof("test"), 0);
					if(recv(client->fd, client_receive, atoi(argv[7]), 0) == 0){
						log_head(&log_data);
						fflush(stdout);
						perror("connect close");
						break;
					}
					else{
						client_receive[strcspn(client_receive, "\n")] = '\0';
						client_receive[strcspn(client_receive, "\r")] = '\0';

						if(strcmp(client_receive, "ready") == 0){
							if(send(client->fd, recv_buffer, atoi(argv[7]), 0) == -1){
								log_head(&log_data);
								perror("connect close");
								break;
							}
							else{
								have_data = 0;
							}
						}
						else if(strcmp(client_receive, "exit") == 0){
							close(client->fd);
							
							log_head(&log_data);
							printf("connect close\n");
							break;
						}
					}
				}
			}
		}
	}

	return 0;
}
