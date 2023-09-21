#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

int main( void ) {
	int server_fd;
	int new_socket;
	long valread;
	struct sockaddr_in address;

	int addrlen = sizeof(address);

	char * hello = "CA A MARCHE\n";

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		exit(1);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	memset(address.sin_zero, '\0', sizeof(address.sin_zero));

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		exit(2);
	}

	if (listen(server_fd, 10) < 0)
	{
		exit(3);
	}

	while(1)
	{
		printf("\n____________ Waiting for new connection ____________\n\n");

		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
		{
			exit(4);
		}

		char buffer[30000] = {0};
		valread = read(new_socket, buffer, 30000);
		printf("%s\n", buffer);
		write(new_socket, hello, strlen(hello));
		printf("Message Sent to client\n");
		close(new_socket);
	}

	return 0;
}