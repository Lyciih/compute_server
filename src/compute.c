#include "compute.h"


extern log_data_t log_data;
extern mqd_t receive_to_compute;
extern mqd_t compute_to_present;
extern char *recv_buffer;
extern char *send_buffer;
extern unsigned int prio;


//compute進程
int compute(int log_on, char *argv[]){
	//初始化log資訊
	log_data.on = log_on;
	log_data.process_name = "compute";
	log_data.pid = getpid();
	log_data.color = 4;

	if(log_data.on == 1){
		log_head(&log_data);
		printf("I am compute process\n");
	}

	while(1){
		if(mq_receive(receive_to_compute, recv_buffer, atoi(argv[7]), &prio) == -1){
			log_head(&log_data);
			perror("mqueue_receive");
		}
		else{
			//printf("%s\n", recv_buffer);
			
			if(mq_send(compute_to_present, recv_buffer, atoi(argv[6]), 0) == -1){
				log_head(&log_data);
				perror("mqueue_send");
			}
			
		}
	}
	return 0;
}
