#include "Socket.hpp"

Socket::Socket()
{

	FD_ZERO(&_write);
	FD_ZERO(&_read);
	FD_ZERO(&_except);
	FD_ZERO(&_main);
	_timeout.tv_sec = 1;
	_timeout.tv_usec = 0;
	bzero(&_svc, sizeof(_svc));
	_max_sock = 0;
}

Socket::~Socket()
{

}

void Socket::setup(int port)
{
	int yes = 1;

	_listening_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	if (_listening_socket < 0)
	{
		std::cerr << "Socket creation impossible" << std::endl;
		exit(1);
	}

	_server.sin_family = AF_INET;
	_server.sin_port = htons(port);
	_server.sin_addr.s_addr = INADDR_ANY;

	if (setsockopt(_listening_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		std::cout << "setsockopt error cheh" << std::endl;
		exit(-9);
	}

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

	fcntl(_listening_socket, F_SETFL, O_NONBLOCK);
	
}

void	Socket::initsets(std::list<Client> * clientlist, Client *clientarray[])
{
	FD_ZERO(&_read);
	FD_ZERO(&_write);
	FD_ZERO(&_main);
	(void)clientarray;
	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
		if(it->_client_socket > 0)
		{
			FD_SET(it->_client_socket, &_read);
			FD_SET(it->_client_socket, &_write);
		}
		else
		{
			FD_CLR(it->_client_socket, &_read);
			FD_CLR(it->_client_socket, &_write);
		}
	}

	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
			this->_max_sock = std::max(_max_sock, it->_client_socket);
	}

	FD_SET(_listening_socket, &_read);
}

void Socket::handleConnection(std::list<Client> * clientlist)
{
	std::string str;
	(void)clientlist;
	Client *clientarray[1024] = {};
	FD_ZERO(&_read);
	FD_ZERO(&_write);
	FD_ZERO(&_main);

	FD_SET(_listening_socket, &_main);

	while (true)
	{
		_timeout.tv_sec = 1;
		_timeout.tv_usec = 1;

		_read = _write = _except = _main;
		setMaxSock(clientarray);
		
		int ret = select(_max_sock + 1, &_read, &_write, &_except, &_timeout);

		if (ret == 0)
		{
			std::cout << "Waiting..." << std::endl;
		}
		else if (ret == -1)
		{
			std::cout << "Error with select, shutting down server..." << std::endl;
			exit(17);
		}

		//std::cout << "Max_sock is " << _max_sock << std::endl;

		for (int i = 0; i <= _max_sock; i++)
		{
			if (FD_ISSET(i, &_read) && i == _listening_socket)
			{

				if(FD_ISSET(i, &readcpy) && i == _listening_socket)
				{
					clientlist->back().acceptConnection(_listening_socket, clientlist->size() - 1, &_read, clientlist);
					if (clientlist->back()._client_socket != -1)
						addfdtoset(clientlist->back()._client_socket, &_read);
					clientlist->back().checknewconnection(clientlist);
				}
				else if (FD_ISSET(i, &readcpy) && i != _listening_socket)
				{
					rcv = read(i, _buffer, sizeof(_buffer));
					readrequest(clientlist, i, rcv);
				}
				else if (FD_ISSET(i, &writecpy))
				{
					for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
					{
						if (it->_client_socket == i)
						{
							int index = open("./index.html", O_RDONLY);
							char buffer2[4096];
							bzero(buffer2, sizeof(buffer2));
							read(index, buffer2, sizeof(buffer2));
							close(index);
							write(i, buffer2, sizeof(buffer2));
							//std::cout << "Response sent to Client[" << it->_clientnumber << "]" << std::endl;
							str = _buffer;
//							if (str.find("close") == true)
							closeconnection(clientlist, i);
							std::cout << i << std::endl;
							// else
							// {
							// 	rmfdfromset(i, &_write);
							// 	addfdtoset(i, &_read);
							// 	setMaxSock(clientlist);
							// }
							
							// if(close(i) != 0)
							// {
							// 	perror("close");
							// 	exit(3);
							// }
							break;
						}
					}
				}
			}
		}
		checktimeout(clientarray);
	}
}

void Socket::rmfdfromset(int fd, fd_set *set)
{
	if (fd == _max_sock)
		_max_sock--;
	FD_CLR(fd, set);
	//std::cout << _max_sock << " in rmfd" << std::endl;
}

void Socket::addfdtoset(int fd, fd_set *set)
{
	FD_SET(fd, set);
	if (_max_sock < fd)
		_max_sock = fd;
}

void Socket::closeconnection(std::list<Client> *clientlist, int i)
{
	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
		if (it->_client_socket == i)
		{
			std::cout << "Client [" << it->_clientnumber << "] disconnected" << std::endl;
			//if (FD_ISSET(i, &_read))
			rmfdfromset(i, &_read);
			//if (FD_ISSET(i, &_write))
			rmfdfromset(i, &_write);
			std::cout << i << std::endl;
			//close(it->_client_socket);
			if(close(i) != 0)
			{
				perror("close");
				exit(3);
			}
			std::cout << i << std::endl;
			i = 17;
			close(it->_client_socket);
			it->_client_socket = -2;
			//clientlist->erase(it);
			std::cout << _max_sock << " after closeconn" << std::endl;
			//setMaxSock(clientarray);
			break;
		}
	}
}

void Socket::checktimeout(Client *clientarray[])
{
	Client *client;

	for (int i = 0; i < 1024; i++)
	{
		if (clientarray[i])
		{
			client = clientarray[i];

			if (time(NULL) - client->_last_msg > 5)
			{
				std::cout << "Client timed out on socket " << client->_client_socket << std::endl;
				FD_CLR(client->_client_socket, &_main);
				close(client->_client_socket);
				delete client;
				clientarray[i] = NULL;
			}
		}
	}

	setMaxSock(clientarray);
}

void Socket::readrequest(Client *clientarray[], int fd)
{
	Client *client = clientarray[fd];
	char *buff = new char[4096];
	int rcv = 0;

	std::cout << "Receiving data on socket " << client->_client_socket << std::endl;

	bzero(buff, 4096);

	rcv = recv(fd, buff, sizeof(buff), 0);
	client->_last_msg = time(NULL);
	if (rcv <= 0)
	{
		if (rcv == 0)
			std::cout << "client closed connection on socket " << client->_client_socket << std::endl;
		else
			std::cout << "Error on socket " << client->_client_socket << " closing connection"<< std::endl;
		perror("recv");
		FD_CLR(fd, &_main);
		close(fd);
		delete client;
		clientarray[fd] = NULL;
		delete [] buff;
		setMaxSock(clientarray);
		return;
	}

	delete [] buff;
	client->_bytesrcv = rcv;
}

void Socket::sendresponse(Client *clientarray[], int fd)
{
	//Client *client = clientarray[fd];
	char response[4096];
	int html = open("./index.html", O_RDONLY);

	read(html, response, sizeof(response));

	unsigned long rcv = send(fd, response, sizeof(response), 0);

	close(html);

	if (rcv < sizeof(response))
		std::cout << "Could not send all index.html to client with socket " << fd << std::endl;
	else
		std::cout << "Server sent response to client with socket " << fd << std::endl;

	std::cout << "Terminating connection with socket " << fd << std::endl;
	FD_CLR(fd, &_main);

	if(close(fd))
	{
		perror("close");
		exit(2);
	}

	delete clientarray[fd];
	clientarray[fd] = NULL;
}

int Socket::findclient(std::list<Client> *clientlist, int fd)
{
	int i = 1;
	for(std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
		if (it->_client_socket == fd)
			return i;
		i++;
	}
	return 0;
}

void Socket::setMaxSock(Client *clientarray[])
{
	_max_sock = _listening_socket;
	//std::cout << _listening_socket << std::endl;
	for (int i = 0; i < 1024;  i++)
	{
		if (clientarray[i])
		{
			if (clientarray[i]->_client_socket > _max_sock)
				_max_sock = clientarray[i]->_client_socket;
		}
	}
	// for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	// {
	// 	if (it->_client_socket > _max_sock)
	// 		_max_sock = it->_client_socket;
	// }
	//std::cout << "max sock is: " << _max_sock << std::endl;

}

void Socket::acceptConnection2(int listeningsocket, Client *clientarray[])
{
	Client *tmp = new Client();
	
	tmp->_client_socket = accept(listeningsocket, (sockaddr *)&tmp->_client, (socklen_t *)&tmp->_clientsize);

	if (tmp->_client_socket < 0)
	{
		std::cout << "Error while accepting client" << std::endl;
		perror("accept");
		close(tmp->_client_socket);
		delete tmp;
		//exit(1);
		return;
	}


	clientarray[tmp->_client_socket] = tmp;
	FD_SET(tmp->_client_socket, &_main);
	if (tmp->_client_socket > _max_sock)
		_max_sock = tmp->_client_socket;
	if (fcntl(tmp->_client_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cout << "cannot set socket to non block mode on socket " << tmp->_client_socket << std::endl;
		perror("fcntl");
		exit(21);
	}
	std::cout << "New connection from client with socket " << tmp->_client_socket << std::endl;
}