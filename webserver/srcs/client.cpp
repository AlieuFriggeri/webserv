#include "Client.hpp"

Client::Client() : _client_socket(-1)
{
	bzero(&_client, sizeof(_client));
	_clientsize = sizeof(_client);
	bzero(&_host, sizeof(_host));
	_clientnumber = -1;
}

Client::~Client()
{

}

void Client::acceptConnection(int listeningsocket, int nbclient)
{
	bzero(&_client, sizeof(_client));
	_clientsize = sizeof(_client);

	if (nbclient >= 1024)
	{
		std::cout << "SERVER FULL" << std::endl;
		_client_socket = -1;
		return;
	}


	_client_socket = accept(listeningsocket, (sockaddr *)&_client, (socklen_t *)&_clientsize);

	if (_client_socket < 0)
	{
		return;
	}

	fcntl(_client_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

}

void Client::checknewconnection(std::list<Client> * clientlist)
{
	static int i = 1;
	if (clientlist->back()._client_socket != -1)
	{
		if (clientlist->size() > 1)
			clientlist->back()._clientnumber = ++i;
		else
			clientlist->back()._clientnumber = 1;
		std::cout << "Client [" << clientlist->back()._clientnumber << "] has been connected" << std::endl;
		clientlist->push_back(Client());
	}
	std::cout << clientlist->size() - 1 << " Client actually connected" << std::endl;
}