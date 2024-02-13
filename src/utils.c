#include "utils.h"


//用來印出log的各種資訊
int log_head(log_data_t* log_data){
	if(log_data == NULL){
		return 1;
	}
	else{
		printf("%d ", log_data->pid);
		
		if(log_data->process_name != NULL){
			printf("%s ", log_data->process_name);
		}

		printf(": ");
		
		return 0;
	}
}
