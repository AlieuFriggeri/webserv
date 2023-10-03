#include "Socket.hpp"

Socket::Socket()
{

	FD_ZERO(&_write);
	FD_ZERO(&_read);
	FD_ZERO(&_except);
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

	fcntl(_listening_socket, F_SETFL, O_NONBLOCK);
	
}

void	Socket::initsets(std::list<Client> * clientlist)
{

	FD_ZERO(&_read);
	FD_ZERO(&_write);

	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
		if(it->_client_socket != -1)
			FD_SET(it->_client_socket, &_read);
	}

	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
		if(it->_client_socket != -1)
			this->_max_sock = std::max(_max_sock, it->_client_socket);
	}

	FD_SET(_listening_socket, &_read);
}

void Socket::handleConnection(std::list<Client> * clientlist)
{
	long rcv = 0;
	std::string str;
	fd_set readcpy;
	fd_set writecpy;
	std::list<Client>::iterator tmp;
	initsets(clientlist);

	FD_ZERO(&readcpy);
	FD_ZERO(&writecpy);

	while(1)
	{
		// for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
		// {
		// 	if (it->_client_socket == -2)
		// 		clientlist->erase(it);
		// }
		readcpy = _read;
		writecpy = _write;
		bzero(_buffer, sizeof(_buffer));
		_timeout.tv_sec = 1;
		_timeout.tv_usec = 0;
		if (_max_sock < _listening_socket)
			_max_sock = _listening_socket;
		for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
		{
			if(it->_client_socket != -1)
				this->_max_sock = std::max(_max_sock, it->_client_socket);
		}
		//std::cout << "max_sock: " << _max_sock << std::endl;
		rcv = select(_max_sock + 1, &readcpy, &writecpy, NULL, &_timeout);

		if (rcv < 0)
		{
			perror("select");
			//std::cout << _max_sock << " | " << clientlist->size() << std::endl;
			exit(1);
			//std::cout << "maxsock: " << _max_sock << std::endl;
			//exit(12);
		}
		else if (rcv == 0)
		{
			std::cout << "rcv = 0" << std::endl;
			//return;
		}
		else
		{	//std::cout << "for loop" << std::endl;
			for (int i = 0; i <= _max_sock; i++)
			{
				bzero(_buffer, sizeof(_buffer));

				if(FD_ISSET(i, &readcpy) && i == _listening_socket)
				{
					clientlist->back().acceptConnection(_listening_socket, clientlist->size() - 1, &_read, clientlist);
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
							write(i, buffer2, sizeof(buffer2));
							//std::cout << "Response sent to Client[" << it->_clientnumber << "]" << std::endl;
							str = it->_buff;
							close(i);
							closeconnection(clientlist, i);
						}
					}
				}
			}
		}
		checktimeout(clientlist);
	}
}

void Socket::rmfdfromset(int fd, fd_set *set)
{
	FD_CLR(fd, set);
	if (fd == _max_sock)
		_max_sock--;
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
			//std::cout << "Client [" << it->_clientnumber << "] disconnected" << std::endl;
			//if (FD_ISSET(i, &_read))
			rmfdfromset(i, &_read);
			//if (FD_ISSET(i, &_write))
			rmfdfromset(i, &_write);
			//close(it->_client_socket);
			close(i);
			it->_client_socket = -2;
			//clientlist->erase(it);
			break;
		}
	}
}

void Socket::checktimeout(std::list<Client> *clientlist)
{
	for ( std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
			if (time(NULL) - it->_last_msg > 10 && it->_client_socket != -1)
			{
				//std::cout << "Client number {" << it->_clientnumber << "} has timed out" << std::endl;
				closeconnection(clientlist, it->_client_socket);
			}
	}
}

void Socket::readrequest(std::list<Client> *clientlist, int fd, long rcv)
{
	std::string str;

	if (rcv < 0)
	{
		closeconnection(clientlist, fd);
		std::cout << "Error in recv" << std::endl;
	}
	else if (rcv == 0)
	{
		closeconnection(clientlist, fd);
	}
	else
	{
		for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
		{
			if (fd == it->_client_socket)
			{
				bzero(it->_buff, sizeof(it->_buff));
				it->_last_msg = time(NULL);

				addfdtoset(fd, &_write);
				rmfdfromset(fd, &_read);

				//std::cout << "Request received from Client " << it->_clientnumber << std::endl;
				str = _buffer;
				break;
			}
		}
	}
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