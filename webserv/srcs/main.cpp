#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <poll.h>
#include <sys/event.h>
#include "../headers/webserv.hpp"
#include "../headers/client.hpp"

#define PORT 8080

int main( void ) {

	Server serv;
	Client client;

	serv.setup(PORT);

	// FD_ZERO(&serv._current_socket);
	// FD_SET(serv._listening_socket, &serv._current_socket);

	bzero(serv._svc, sizeof(serv._svc));
	bzero(client._host, sizeof(client._host));
	client.acceptConnection(serv._listening_socket);

	while (1)
	{
		std::cout << "Waiting for connection" << std::endl;

		serv.handleConnection(client._client_socket);
	}
	return 0;
}