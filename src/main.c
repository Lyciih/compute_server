#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "utils.h"
#include "receive.h"

int main(int argc, char *argv[]){
	//初始化log資訊
	log_data_t log_data;
	log_data.on = 0;
	log_data.process_name = "main   ";
	log_data.pid = getpid();

	//檢查參數完整性
	if(argc < 4){
		log_head(&log_data);
		printf("缺少參數\n");
		exit(1);
	}
	else{
		//是否開啟狀態回報----------------------------------------------------------------
		log_data.on = atoi(argv[1]);
	}


	//註冊信號事件----------------------------------------------------------------------------


	//分裂出三個子進程------------------------------------------------------------------------
	pid_t receive_pid;
	pid_t compute_pid;
	pid_t present_pid;
	
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
			printf("I am compute process\n");
		}
		else{
			present_pid = fork();
			if(present_pid == -1){
				perror("present child process fork fail");
			}
			else if(present_pid == 0){
				printf("I am present process\n");
			}
			else{
				//主進程從這裡開始------------------------------------------------

				if(log_data.on == 1){
					log_head(&log_data);
					printf("I am main process\n");
				}

			}
		}
	}
}
