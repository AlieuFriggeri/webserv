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
#include "../headers/Socket.hpp"
#include "../headers/client.hpp"
#include <sys/select.h>

#define PORT 667

int main( void ) {

	Socket serv;
	Client client;
	client._client_socket = -1;
	//std::list<Client> clientlist;

	serv.setup(PORT);

	bzero(serv._svc, sizeof(serv._svc));
	bzero(client._host, sizeof(client._host));

	fcntl(serv._listening_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);


	
	while (1)
	{
		if (client._client_socket == -1)
		{
			client.acceptConnection(serv._listening_socket);
			fcntl(client._client_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
		}
		
		std::cout << "Waiting for connection . . ." << std::endl;

		serv.prepareConnection(client._client_socket);

	}
	return 0;
}