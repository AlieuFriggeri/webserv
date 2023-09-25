#include "../headers/webserv.hpp"

Server::Server()
{
	bzero(&_mypoll, sizeof(_mypoll));

}

Server::~Server()
{

}

void Server::setup(int port)
{
	_listening_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	if (_listening_socket < 0)
	{
		std::cerr << "Socket creation impossible" << std::endl;
		exit(1);
	}

	_server.sin_family = AF_INET;
	_server.sin_port = htons(port);
	inet_pton(AF_INET, "0.0.0.0", &_server.sin_addr);

	if (bind(_listening_socket, (sockaddr *)&_server, (socklen_t)sizeof(_server)) < 0)
	{
		std::cerr << "Error during socket binding" << std::endl;
		exit(2);
	}

	if (listen(_listening_socket, 128) < 0)
	{
		std::cerr << "Error while listening on socket" << std::endl;
		exit(3);
	}
}

void Server::handleConnection(int clientsocket, int i)
{

	int bytesRcv;

	bzero(_buffer, sizeof(_buffer));
	bytesRcv = recv(_mypoll[i].fd, _buffer, sizeof(_buffer), 0);

	if (bytesRcv < 0)
	{
		std::cerr << "Error while connecting" << std::endl;
		return;
	}

	if (bytesRcv == 0)
	{
		std::cout << "Client has disconnected" << std::endl;
		exit(0);
	}

	// afficher le message
	std::cout << "Message recu: " << _buffer << std::endl;

	send(_mypoll[i].fd, _buffer, bytesRcv + 1, 0);
}