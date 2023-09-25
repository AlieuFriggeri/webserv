#ifndef WEBSERV_HPP
#define WEBSERV_HPP

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


class Server{


	public:
	Server();
	~Server();
	void setup(int port);
	void handleConnection(int clientsocket);


	int _listening_socket;
	sockaddr_in _server;
	fd_set _current_socket, _ready_socket;
	char _svc[NI_MAXSERV];
	char _buffer[4096];
	private:


};











#endif