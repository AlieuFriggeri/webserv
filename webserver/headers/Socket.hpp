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
#include <map>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include "Client.hpp"
#include "ServerConfig.hpp"
#include "Route.hpp"
#include "GetRequestHandler.class.hpp"
#include "PostRequestHandler.class.hpp"
#include "DeleteRequestHandler.class.hpp"

class Route;

class Socket{


	public:
	Socket();
	~Socket();
	static void setup(Socket *servers);
	//void handleConnection(int clientsocket);
	//void prepareConnection(int clientsocket);
	static void handleConnection(std::list<Client> *clientlist, Socket *servers);
	static int initsets(std::list<Client> * clientlist, fd_set *_read, fd_set *_write, Socket *servers);
	static int rmfdfromset(int fd, fd_set *set, int max_sock);
	static int addfdtoset(int fd, fd_set *set, int max_sock);
	static void closeconnection(std::list<Client> *clientlist, int i, fd_set *readset, fd_set *writeset);
	void checktimeout(std::list<Client> *clientlist, fd_set *readset, fd_set *writeset);
	static void readrequest(std::list<Client> *clientlist, int fd, long rcv, fd_set *readset, fd_set *writeset, char *buffer);
	int findclient(std::list<Client> *clientlist, int fd);
	static void setMaxSock(std::list<Client> *clientlist);
	static void sendresponse(std::list<Client> *clientlist, int fd, Socket *servers);
	static void checkroute(Client *client, Socket *server);

	sockaddr_in _server;
	fd_set _read, _write, _except, _main;
	timeval _timeout;
	int _listening_socket, _r, _w, _e;
	char _svc[NI_MAXSERV];
	char *_buffer;
	int _max_sock;
	std::string _servername;
	int _maxbodysize;
	int _port;
	std::string _root;
	std::map<std::string, Route> _route;
	std::map<std::string, std::string> _config;
	int _totalserv;

	private:


};











#endif
