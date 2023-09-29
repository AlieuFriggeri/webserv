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
//#include <sys/event.h>
#include <list>
#include "Socket.hpp"
#include "Client.hpp"
#include <sys/select.h>

#define PORT 667

int main( void ) {

	Socket serv;
	std::list<Client> clientlist;

	serv.setup(PORT);
	clientlist.push_back(Client());
	std::cout << "Launching WEBSERV listening on port " << PORT << std::endl;
	serv.handleConnection(&clientlist);
	
	return 0;
}