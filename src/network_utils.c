#include "network_utils.h"

//取得 listen socket
int get_listen_socket(log_data_t *log_data, int ip_reuse){
	int temp = 0;
	temp = socket(AF_INET, SOCK_STREAM, 0);

	if(temp < 0){
		log_head(log_data);
		perror("create listen socket error");
		return 1;
		exit(1);
	}
	else{
		if(log_data->on == 1){
			log_head(log_data);
			printf("create listen socket success: %d\n", temp);
		}

		//將 listen socket 設為地址可重用，這樣 server 中止後可立即重啟，不用等待 kernel 回收完
		if(ip_reuse == 1){
			int reuse_addr_on = 1;
			if(setsockopt(temp, SOL_SOCKET, SO_REUSEADDR, &reuse_addr_on, sizeof(reuse_addr_on)) < 0){
				log_head(log_data);
				perror("set listen socket ip reuse error");
				exit(1);
			}
			else{
				if(log_data->on == 1){
					log_head(log_data);
					printf("set listen socket ip reuse success: %d\n", temp);
				}
			}
		}

		return temp;
	}
}



//取得 socket (通常用在client端)
int get_socket(log_data_t *log_data){
	int temp = 0;
	temp = socket(AF_INET, SOCK_STREAM, 0);

	if(temp < 0){
		log_head(log_data);
		perror("create socket error");
		return 1;
		exit(1);
	}
	else{
		if(log_data->on == 1){
			log_head(log_data);
			printf("create socket success: %d\n", temp);
		}
		return temp;
	}
}






//設定 server 的地址
sockaddr_in_t set_server_addr(log_data_t *log_data, char *ip, char *port){
	sockaddr_in_t server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	
	if(inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0){
		log_head(log_data);
		printf("server ip error\n");
		exit(1);
	}
	else{
		if(log_data->on == 1){
			log_head(log_data);
			printf("set server ip success: %s\n", ip);
		}
	}

	int port_convert = atoi(port);
	if(port_convert == 0){
		log_head(log_data);
		printf("server port error: %s\n", port);
		exit(1);
	}
	else{
		server_addr.sin_port = htons(port_convert);
		
		if(log_data->on == 1){
			log_head(log_data);
			printf("set server port success: %s\n", port);
		}
	}

	return server_addr;
}



//綁定 listen socket 和 server 地址
int listen_socket_bind_address(log_data_t *log_data, int listen_fd, sockaddr_in_t *server_addr){
	if(bind(listen_fd, (struct sockaddr*)server_addr, sizeof(*server_addr)) < 0){
		log_head(log_data);
		perror("listen socket bind address error");
		exit(1);
	}
	else{
		if(log_data->on == 1){
			log_head(log_data);
			printf("listen socket bind address success\n");
		}
		return 0;
	}
}



//開始監聽端口
int start_listen(log_data_t *log_data, int listen_fd, int backlog){
	if(listen(listen_fd, backlog) < 0){
		log_head(log_data);
		perror("start listen error");
		exit(1);
	}
	else{
		if(log_data->on == 1){
			log_head(log_data);
			printf("start listen success\n");
		}
		return 0;
	}
}



//接受 client 端的連線
client_t* accept_client(log_data_t *log_data, int listen_fd){
	client_t *client = (client_t *)malloc(sizeof(client_t));
	client->len = sizeof(client->addr);

	client->fd = accept(listen_fd, (struct sockaddr *)&(client->addr), &(client->len));
	if(client->fd == -1){
		log_head(log_data);
		perror("client accept error");
		exit(1);
	}
	else{
		if(log_data->on == 1){
			log_head(log_data);
			printf("client accept success\n");
		}
		return client;
	}

}

//連上server端
int connect_server(log_data_t *log_data, int client_fd, sockaddr_in_t *server_addr){

	if(connect(client_fd, (struct sockaddr *)server_addr, sizeof(*server_addr)) == -1){
		log_head(log_data);
		perror("connect server error");
		exit(1);
	}
	else{
		if(log_data->on == 1){
			log_head(log_data);
			printf("connect server success\n");
		}
		return 0;
	}
}
