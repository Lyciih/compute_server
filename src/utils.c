#include "utils.h"

#define COLOR_RESET	"\x1b[0m"
#define COLOR_1		"\x1b[31m"
#define COLOR_2		"\x1b[32m"
#define COLOR_3		"\x1b[33m"

//用來印出log的各種資訊
int log_head(log_data_t* log_data){
	if(log_data == NULL){
		return 1;
	}
	else{
		switch(log_data->color){
			case 1:
				printf(COLOR_1 "%d " COLOR_RESET, log_data->pid);
				break;
			case 2:
				printf(COLOR_2 "%d " COLOR_RESET, log_data->pid);
				break;
			case 3:
				printf(COLOR_3 "%d " COLOR_RESET, log_data->pid);
				break;
			default:
				printf("%d ", log_data->pid);
		}
		
		if(log_data->process_name != NULL){
			printf("%s ", log_data->process_name);
		}

		printf(": ");
		
		return 0;
	}
}
