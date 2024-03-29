#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mqueue.h>
#include "utils.h"
#include "receive.h"
#include "present.h"
#include "compute.h"
#include "network_utils.h"
#include "signal_handle.h"


client_t *client = NULL;
log_data_t log_data;
mqd_t receive_to_compute;
mqd_t compute_to_present;
unsigned int prio;
char *recv_buffer = NULL;
char *send_buffer = NULL;


int main(int argc, char *argv[]){
	//初始化log資訊
	log_data.on = 0;
	log_data.process_name = "main   ";
	log_data.pid = getpid();
	log_data.color = 1;

	//檢查參數完整性
	if(argc < 8){
		log_head(&log_data);
		printf("缺少參數\n");
		exit(1);
	}
	else{
		//是否開啟狀態回報----------------------------------------------------------------
		log_data.on = atoi(argv[1]);
	}

	//準備傳輸buffer
	recv_buffer = (char *)malloc(atoi(argv[7]));
	send_buffer = (char *)malloc(atoi(argv[7]));

	//註冊信號事件----------------------------------------------------------------------------
	set_signal_handle();

	//建立消息隊列----------------------------------------------------------------------------
	struct mq_attr attr;
	attr.mq_msgsize = atoi(argv[7]);
	attr.mq_maxmsg = atoi(argv[6]) / 2;

	receive_to_compute = mq_open("/receive_to_compute", O_CREAT | O_EXCL | O_RDWR, 0666, &attr);
	if(receive_to_compute == (mqd_t)-1){
		perror("mq_open");
	}

	compute_to_present = mq_open("/compute_to_present", O_CREAT | O_EXCL | O_RDWR, 0666, &attr);
	if(compute_to_present == (mqd_t)-1){
		perror("mq_open");
	}



	//分裂出三個子進程------------------------------------------------------------------------
	pid_t receive_pid;
	pid_t compute_pid;
	pid_t present_pid;
	int process_status;
	
	receive_pid = fork();
	if(receive_pid == -1){
		perror("receive child process fork fail");
	}
	else if(receive_pid == 0){
		receive(log_data.on, argv);
	}
	else{
		compute_pid = fork();
		if(compute_pid == -1){
			perror("compute child process fork fail");
		}
		else if(compute_pid == 0){
			compute(log_data.on, argv);
		}
		else{
			present_pid = fork();
			if(present_pid == -1){
				perror("present child process fork fail");
			}
			else if(present_pid == 0){
				present(log_data.on, argv);
			}
			else{
				//主進程從這裡開始------------------------------------------------

				if(log_data.on == 1){
					log_head(&log_data);
					printf("I am main process\n");
				}

        			//取得 listen socket
        			int listen_fd = get_listen_socket(&log_data, 1); 
        			//填寫 server ip
        			sockaddr_in_t server_addr = set_server_addr(&log_data, argv[2], argv[3]);
        			//綁定 listen socket 和 ip
        			listen_socket_bind_address(&log_data, listen_fd, &server_addr);
        			//監聽 listen socket
        			start_listen(&log_data, listen_fd, 5); 
        			

				//準備連線時需要用到的變數
        			client_t *client = NULL;
				char buffer[] = "hi I am compute server control port";
				//char recv_buffer[atoi(argv[7])];
				
				while(1){
					//接受 client端連線
					client = accept_client(&log_data, listen_fd);
					send(client->fd, buffer, sizeof(buffer), 0); 

					while(1){
						if(recv(client->fd, recv_buffer, atoi(argv[7]), 0) == 0){
							log_head(&log_data);
							printf("connect close : %s %d\n", client->ip, client->port);
							break;
						} 


						//去除收到的訊息尾巴的換行或回車符
						recv_buffer[strcspn(recv_buffer, "\n")] = '\0';
						recv_buffer[strcspn(recv_buffer, "\r")] = '\0';
						
						printf("%s\n", recv_buffer);
						
						if(strcmp(recv_buffer, "exit") == 0){
							close(client->fd);
							free(client);

							free(recv_buffer);
							free(send_buffer);

							union sigval value;
							value.sival_int = 0;
							sigqueue(receive_pid, SIGRTMIN, value);
							sigqueue(present_pid, SIGRTMIN, value);
							sigqueue(compute_pid, SIGRTMIN, value);

							//等待其它進程結束
							waitpid(receive_pid, &process_status, 0);
							waitpid(present_pid, &process_status, 0);
							waitpid(compute_pid, &process_status, 0);
							
							//關閉消息隊列
							if(mq_close(receive_to_compute) == -1){
								perror("mq_close");
							}
							
							if(mq_close(compute_to_present) == -1){
								perror("mq_close");
							}

							if(mq_unlink("/receive_to_compute") == -1){
								perror("mq_unlink");
							}
							
							if(mq_unlink("/compute_to_present") == -1){
								perror("mq_unlink");
							}

							return 0;
						}
					}
				}
			}
		}
	}
}
