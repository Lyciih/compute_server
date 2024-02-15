#include "compute.h"


extern log_data_t log_data;


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
	return 0;
}
