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
	std::list<Client> clientlist;

	serv.setup(PORT);
	clientlist.push_back(Client());

	while (1)
	{
		if (clientlist.back()._client_socket == -1)
		{
			std::cout << "Waiting for connection . . ." << std::endl;
			clientlist.back().acceptConnection(serv._listening_socket, clientlist);
			if (clientlist.back()._client_socket != -1)
			{
				clientlist.push_back(Client());
				std::cout << "New client has been connected" << std::endl;
			}
		}

		serv.prepareConnection(clientlist.front()._client_socket);

	}
	
	return 0;
}