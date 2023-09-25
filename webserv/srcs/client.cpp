#include "../headers/client.hpp"

Client::Client()
{

}

Client::~Client()
{

}

void Client::acceptConnection(int listeningsocket, int *fd)
{
	bzero(&_client, sizeof(_client));
	_clientsize = sizeof(_client);

	*fd = accept(listeningsocket, (sockaddr *)&_client, (socklen_t *)&_clientsize);

	if (*fd < 0)
	{
		std::cerr << "Error while connecting to client" << std::endl;
		exit(5);
	}

}