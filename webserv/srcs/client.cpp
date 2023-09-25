#include "../headers/client.hpp"

Client::Client()
{

}

Client::~Client()
{

}

void Client::acceptConnection(int listeningsocket)
{
	bzero(&_client, sizeof(_client));
	_clientsize = sizeof(_client);

	_client_socket = accept(listeningsocket, (sockaddr *)&_client, (socklen_t *)&_clientsize);

	if (_client_socket < 0)
	{
		std::cerr << "Error while connecting to client" << std::endl;
		exit(5);
	}

}