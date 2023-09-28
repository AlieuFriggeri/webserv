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

	fcntl(_listening_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	
}

void Socket::handleConnection(std::list<Client> * clientlist)
{
	long rcv = 0;
	_max_sock = 0;

	FD_ZERO(&_read);
	FD_ZERO(&_write);
	FD_ZERO(&_except);

	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
		if(it->_client_socket != -1)
		{
			FD_SET(it->_client_socket, &_read);
			FD_SET(it->_client_socket, &_write);
			FD_SET(it->_client_socket, &_except);
		}

	}

	bzero(_buffer, sizeof(_buffer));

	_timeout.tv_sec = 1;
	_timeout.tv_usec = 42;

	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
		if(it->_client_socket != -1)
			_max_sock = std::max(_max_sock, it->_client_socket);
	}

	rcv = select(1024, &_read, &_write, &_except, &_timeout);

	if (rcv < 0)
	{
		perror("select");
		std::cout << "maxsock: " << _max_sock << std::endl;
		exit(12);
	}
	else if (rcv == 0)
	{
		//std::cout << "No pending data" << std::endl;
		return;
	}
	else
	{
		_timeout.tv_sec = 1;
		_timeout.tv_usec = 42;

		for (int i = 0; i < _max_sock + 1; i++)
		{
			if (FD_ISSET(i, &_read))
			{
				rcv = recv(i, _buffer, sizeof(_buffer), 0);
				if (rcv < 0)
				{
					//FD_CLR(i, &_write);
					FD_CLR(i, &_read);
				}
				else if (rcv == 0)
				{
					FD_CLR(i, &_read);
					for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
					{
						if (it->_client_socket == i)
						{
							std::cout << "Client[" << it->_clientnumber << "] terminated connection" << std::endl;
							close(it->_client_socket);
							clientlist->erase(it);
							return;
						}
					}
				}
				else
				{
					for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
					{
						if (it->_client_socket == i)
						{
							std::cout << "Request  received from Client " << it->_clientnumber << std::endl;
							if (FD_ISSET(i, &_write))
							{

								int index = open("./index.html", O_RDONLY);
								char buffer2[4096];
								read(index, buffer2, sizeof(buffer2));
								send(it->_client_socket, buffer2, sizeof(buffer2), 0);
								std::cout << "response sent to Client[" << it->_clientnumber << "]" << std::endl;
							}
						}
					}
				}
				rcv = 0;
				break;
			}
			//else if ()
		}
		
	}
}