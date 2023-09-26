#include "../headers/client.hpp"

Client::Client() : _client_socket(-1)
{
	bzero(&_client, sizeof(_client));
	_clientsize = sizeof(_client);
	bzero(&_host, sizeof(_host));

}

Client::~Client()
{

}

void Client::acceptConnection(int listeningsocket, std::list<Client>clientlist)
{
	bzero(&_client, sizeof(_client));
	_clientsize = sizeof(_client);

	_client_socket = accept(listeningsocket, (sockaddr *)&_client, (socklen_t *)&_clientsize);

	if (_client_socket < 0)
	{
		std::cerr << "No client connected" << std::endl;
		return;
	}
	
	fcntl(_client_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

}