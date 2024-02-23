#include "signal_handle.h"




extern client_t *client;
extern log_data_t log_data;
extern mqd_t receive_to_compute;
extern mqd_t compute_to_present;
extern char *recv_buffer;
extern char *send_buffer;



void signal_handle(int signal, siginfo_t *info, void *ctx){

	//如果當前有連線則關閉
	if(client != NULL){
		close(client->fd);
		free(client);
	}


	if(recv_buffer != NULL){
		free(recv_buffer);
	}

	
	if(send_buffer != NULL){
		free(send_buffer);
	}


	if(mq_close(receive_to_compute) == -1){
		perror("mq_close");
	}
	
	if(mq_close(compute_to_present) == -1){
		perror("mq_close");
	}

	if(log_data.on == 1){
	log_head(&log_data);
	printf("close\n");
	}
	

	

	
	//此處應用 exit 而不是return ，不然會無法終止程式
	exit(0);
}






void set_signal_handle(){
	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_sigaction = signal_handle;
	action.sa_flags = SA_SIGINFO;
	
	sigaction(SIGRTMIN, &action, NULL);
}
