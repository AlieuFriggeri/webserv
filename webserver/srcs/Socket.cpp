#include "Socket.hpp"
#include "HttpRequest.class.hpp"

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

void	Socket::initsets(std::list<Client> * clientlist)
{

	FD_ZERO(&_read);
	FD_ZERO(&_write);

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
	long rcv = 0;
	std::string str;
	fd_set readcpy;
	fd_set writecpy;
	std::list<Client>::iterator tmp;
	//Client *clients[1024] = {};
	initsets(clientlist);

	FD_ZERO(&readcpy);
	FD_ZERO(&writecpy);

	while(1)
	{
		for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
		{
			if (it->_client_socket == -2)
				clientlist->erase(it);
		}
		readcpy = _read;
		writecpy = _write;
		bzero(_buffer, sizeof(_buffer));
		_timeout.tv_sec = 1;
		_timeout.tv_usec = 0;
		if (_max_sock < _listening_socket)
			_max_sock = _listening_socket;
		for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
		{
				this->_max_sock = std::max(_max_sock, it->_client_socket);
		}
		//std::cout << "max_sock: " << _max_sock << std::endl;
		rcv = select(_max_sock + 1, &readcpy, &writecpy, NULL, &_timeout);

		if (rcv < 0)
		{
			perror("select");
			std::cout << _max_sock << " | " << clientlist->size() << std::endl;
			exit(1);
			//std::cout << "maxsock: " << _max_sock << std::endl;
			//exit(12);
		}
		else if (rcv == 0)
		{
			//std::cout << "rcv = 0" << std::endl;
			//return;
		}
		else
		{	//std::cout << "for loop" << std::endl;
			bzero(_buffer, sizeof(_buffer));
			for (int i = 0; i <= _max_sock; i++)
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
					std::cout << "REQUEST" << std::endl << _buffer << std::endl << std::endl;
					HttpRequest test;
					test.parse(_buffer, rcv);
					test.printMessage();
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
		//checktimeout(clientlist);
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
			setMaxSock(clientlist);
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
				setMaxSock(clientlist);
			}
	}
}

void Socket::readrequest(std::list<Client> *clientlist, int fd, long rcv)
{
	std::string str;


	if (rcv < 0)
	{
		closeconnection(clientlist, fd);
		setMaxSock(clientlist);
		std::cout << "Error in recv" << std::endl;
	}
	else if (rcv == 0)
	{
		closeconnection(clientlist, fd);
		setMaxSock(clientlist);
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

void Socket::setMaxSock(std::list<Client> *clientlist)
{
	_max_sock = _listening_socket;
	
	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
		if (it->_client_socket > _max_sock)
			_max_sock = it->_client_socket;
	}
	//std::cout << "max sock is: " << _max_sock << std::endl;
}
