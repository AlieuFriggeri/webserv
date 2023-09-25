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

#define PORT 8080

int main( void ) {

	Server serv;
	Client client;
	Client client2;
	//std::list<Client> clientlist;

	serv.setup(PORT);

	bzero(serv._svc, sizeof(serv._svc));
	bzero(client._host, sizeof(client._host));
	bzero(client2._host, sizeof(client2._host));


	client.acceptConnection(serv._listening_socket, &serv._mypoll[0].fd);
	client2.acceptConnection(serv._listening_socket, &serv._mypoll[1].fd);

	fcntl(serv._listening_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	
	while (1)
	{
		poll(serv._mypoll, 2, 100);

		std::cout << "Waiting for connection" << std::endl;

		serv.handleConnection(client._client_socket, 0);
		serv.handleConnection(client2._client_socket, 1);
	}
	return 0;
}