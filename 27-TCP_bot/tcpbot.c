#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>


int get_result(char buffer[2000])
{
	char left[10];
	char right[10]; 
	char temp[10];
	int index = 0;
	char operator;
	for(int x=0; buffer[x] != '='; x++) {
		if(isdigit(buffer[x])) {
			temp[index] = buffer[x];
			index++;	
		} 
		else {
			temp[index] = '\0';
			index = 0;
			strcpy(left,temp);
			operator = buffer[x];
		}	
	}
	temp[index] = '\0';
	strcpy(right,temp);
	switch(operator) {
	case '+': 	
		return atoi(left) + atoi(right);
	case '-': 
		return atoi(left) - atoi(right);	
	case '*': 
		return atoi(left) * atoi(right);
	case '/': 	
		return atoi(left) / atoi(right);
	}
}

void print_buffer(int size, char buffer[]) 
{
	for(int x=0; x<size;x++) {
		printf("%c",buffer[x]);
	}
	printf("\n");
}

int main(int argc, char *argv[]) 
{
	int socket_desc;
	struct sockaddr_in server;
	char input_buffer[2000];
	char *output;
	
	if(argc != 3) {
		fprintf(stderr,"usage: <ip_address> <port>\n");
		close(socket_desc);
		return EXIT_FAILURE;
	}	

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1) {
		fprintf(stderr, "Could not create socket\n");
		close(socket_desc);
		return EXIT_FAILURE;
	}

	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));

	if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
		fprintf(stderr, "Connection error\n");
		close(socket_desc);
		return EXIT_FAILURE;
	}
	
	printf("Connected to %s\n",argv[1]);
	
	
	//Send GO message
	output = "GO";
	
	if(send(socket_desc, output, strlen(output), 0) < 0) {
		fprintf(stderr,"Failed to send reply\n");
	}
	
	printf("SENT: %s\n",output);
	int result = 0;
	char data[10];
	int recv_len = recv(socket_desc, input_buffer, 2000, 0);
	while(recv_len > 0) {
		if(!isdigit(input_buffer[0])) {	
			print_buffer(recv_len, input_buffer);
			recv_len = recv(socket_desc, input_buffer, 2000, 0);
			continue;
		}
		print_buffer(recv_len, input_buffer);	
		result = get_result(input_buffer);
		sprintf(data, "%d", result);
		printf("Answer is: %d\n",result);
		if(send(socket_desc, data, strlen(data), 0) < 0) {
			fprintf(stderr,"Failed to send reply\n");
		};
		recv_len = recv(socket_desc, input_buffer, 2000, 0);
	}

	close(socket_desc);	
	return 0;	
}
