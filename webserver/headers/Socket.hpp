#ifndef SOCKET_HPP
#define SOCKET_HPP

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
#include <sys/time.h>
#include <fcntl.h>
#include <sys/select.h>
#include <algorithm>
#include <list>
#include "Client.hpp"


class Socket{


	public:
	Socket();
	~Socket();
	void setup(int port);
	//void handleConnection(int clientsocket);
	//void prepareConnection(int clientsocket);
	void handleConnection(std::list<Client> * clientlist);

	sockaddr_in _server;
	fd_set _read, _write, _except;
	timeval _timeout;
	int _listening_socket, _r, _w, _e;
	char _svc[NI_MAXSERV];
	char _buffer[4096];
	private:


};











#endif