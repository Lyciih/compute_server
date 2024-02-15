#include "signal_handle.h"




extern client_t *client;
extern log_data_t log_data;



void signal_handle(int signal, siginfo_t *info, void *ctx){

	//如果當前有連線則關閉
	if(client != NULL){
		close(client->fd);
		free(client);
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
