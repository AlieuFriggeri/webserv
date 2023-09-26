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
#include <list>
#include "../headers/webserv.hpp"
#include "../headers/client.hpp"
#include <sys/select.h>

#define PORT 667

int main( void ) {

	Server serv;
	Client client;
	Client client2;
	//std::list<Client> clientlist;

	serv.setup(PORT);

	bzero(serv._svc, sizeof(serv._svc));
	bzero(client._host, sizeof(client._host));
	bzero(client2._host, sizeof(client2._host));


	client.acceptConnection(serv._listening_socket);

	fcntl(serv._listening_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	
	while (1)
	{

		std::cout << "Waiting for connection" << std::endl;

		serv.handleConnection(client._client_socket);
	}
	return 0;
}