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
#include "ServerConfig.hpp"
#include <sys/select.h>
#include "HttpRequest.class.hpp"

#define PORT 8002

int main( int ac,char **av) {
	ServerConfig configuration;
	if (ac != 2)
	{
		std::cerr << "Usage: ./webserv CONFIG_FILE.UDC" << std::endl;
		exit(1);
	}
	Socket *serverarray = NULL;
	serverarray = ServerConfig::parsefile(av[1]);
	Socket::setup(serverarray);

	delete [] serverarray;
	return 0;
}
