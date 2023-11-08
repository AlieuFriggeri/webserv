#include "Socket.hpp"
#include "HttpRequest.class.hpp"

int max_sock;

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

void Socket::setup(Socket *servers)
{
	for (int i = 0; i < servers[0]._totalserv; i++)
	{
//		servers[i]._listening_socket = socket(AF_INET, SOCK_STREAM, 0);
	
		int yes = 1;

		servers[i]._listening_socket = socket(AF_INET, SOCK_STREAM, 0);
		
		if (servers[i]._listening_socket < 0)
		{
			std::cerr << "Socket creation impossible" << std::endl;
			exit(1);
		}

		servers[i]._server.sin_family = AF_INET;
		servers[i]._server.sin_port = htons(servers[i]._port);
		servers[i]._server.sin_addr.s_addr = INADDR_ANY;

		if (setsockopt(servers[i]._listening_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		{
			std::cout << "setsockopt error cheh" << std::endl;
			exit(-9);
		}

		if (bind(servers[i]._listening_socket, (sockaddr *)&servers[i]._server, (socklen_t)sizeof(servers[i]._server)) < 0)
		{
			std::cerr << "Error during socket binding" << std::endl;
			exit(2);
		}

		if (listen(servers[i]._listening_socket, 128) < 0)
		{
			std::cerr << "Error while listening on socket" << std::endl;
			exit(3);
		}

		if (fcntl(servers[i]._listening_socket, F_SETFL, O_NONBLOCK) < 0)
		{
			std::cerr << "Error with fcntl" << std::endl;
			exit(4);
		}
	}
	
}

int	Socket::initsets(std::list<Client> * clientlist, fd_set *_read, fd_set *_write, Socket *servers)
{
	int res = 0;
	FD_ZERO(_read);
	FD_ZERO(_write);

	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
		if(it->_client_socket > 0)
		{
			FD_SET(it->_client_socket, _read);
			FD_SET(it->_client_socket, _write);
		}
		else
		{
			FD_CLR(it->_client_socket, _read);
			FD_CLR(it->_client_socket, _write);
		}
	}

	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
			res = std::max(res, it->_client_socket);
	}
	int i = 0;

	while (i < servers[0]._totalserv)
	{
		FD_SET(servers[i]._listening_socket, _read);
		res = std::max(res, servers[i]._listening_socket);
		i++;
	}
	return res;
}

int checkport(int i, Socket *servers)
{
	for (int j = 0; j < servers[0]._totalserv; j++)
	{
		if (servers[j]._listening_socket == i)
			return servers[j]._port;
	}
	return -1;
}

int checklisteningsock(int i, Socket *servers)
{
	for (int j = 0; j < servers[0]._totalserv; j++)
	{
		if (servers[j]._listening_socket == i)
			return i;
	}
	return -1;
}

void Socket::handleConnection(std::list<Client> * clientlist, Socket *servers)
{
	long rcv = 0;
	std::string str;
	char buffer[4096];
	fd_set readcpy;
	fd_set writecpy;
	fd_set readset;
	fd_set writeset;
	timeval timeout;
	std::list<Client>::iterator tmp;
	initsets(clientlist, &readset, &writeset, servers);

	FD_ZERO(&readcpy);
	FD_ZERO(&writecpy);

	while(1)
	{
		for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
		{
			if (it->_client_socket == -2)
				clientlist->erase(it);
		}
		readcpy = readset;
		writecpy = writeset;
		bzero(buffer, sizeof(buffer));
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		for (int i = 0; i < servers[0]._totalserv; i++)
		{
			if (max_sock < servers[i]._listening_socket)
				max_sock = servers[i]._listening_socket;
		}
		for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
		{
				max_sock = std::max(max_sock, it->_client_socket);
		}
		//std::cout << "max_sock: " << max_sock << std::endl;
		rcv = select(max_sock + 1, &readcpy, &writecpy, NULL, &timeout);

		if (rcv < 0)
		{
			perror("select");
			std::cout << max_sock << " | " << clientlist->size() << std::endl;
			exit(1);
			//std::cout << "maxsock: " << max_sock << std::endl;
			//exit(12);
		}
		else if (rcv == 0)
		{
			std::cout << "max_sock = " << max_sock << std::endl;
		}
		else
		{
			bzero(buffer, sizeof(buffer));
			for (int i = 0; i <= max_sock; i++)
			{
				if(FD_ISSET(i, &readcpy) && checklisteningsock(i, servers) == i)
				{
					clientlist->back().acceptConnection(checklisteningsock(i, servers), clientlist->size() - 1, &readset, clientlist, checkport(i, servers));
					if (clientlist->back()._client_socket != -1)
						max_sock = addfdtoset(clientlist->back()._client_socket, &readset, max_sock);
					clientlist->back().checknewconnection(clientlist);
				}
				else if (FD_ISSET(i, &readcpy) && i != checklisteningsock(i, servers))
				{
					rcv = read(i, buffer, sizeof(buffer));
					readrequest(clientlist, i, rcv, &readset, &writeset, buffer);
				}
				else if (FD_ISSET(i, &writecpy))
				{
					for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
					{
						if (it->_client_socket == i)
						{
							// int index = open("./index.html", O_RDONLY);
							// char buffer2[4096];
							// bzero(buffer2, sizeof(buffer2));
							// read(index, buffer2, sizeof(buffer2));
							// close(index);
							// write(i, buffer2, sizeof(buffer2));
							// str = buffer;
							// closeconnection(clientlist, i, &readset, &writeset);
							// std::cout << i << std::endl;
							sendresponse(clientlist, i);
							closeconnection(clientlist, i, &readset, &writeset);
							break;
						}
					}
				}
			}
		}
		//checktimeout(clientlist);
	}
}


int Socket::rmfdfromset(int fd, fd_set *set, int max_sock)
{
	if (fd == max_sock)
		max_sock--;
	FD_CLR(fd, set);
	return max_sock;
	//std::cout << max_sock << " in rmfd" << std::endl;
}

int Socket::addfdtoset(int fd, fd_set *set, int max_sock)
{
	FD_SET(fd, set);
	if (max_sock < fd)
		max_sock = fd;
	return max_sock;
}

void Socket::closeconnection(std::list<Client> *clientlist, int i, fd_set *readset, fd_set *writeset)
{
	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
		if (it->_client_socket == i)
		{
			std::cout << "Client [" << it->_clientnumber << "] disconnected" << std::endl;
			//if (FD_ISSET(i, &readset))
			max_sock = rmfdfromset(i, readset, max_sock);
			//if (FD_ISSET(i, &writeset))
			max_sock = rmfdfromset(i, writeset, max_sock);
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
			std::cout << max_sock << " after closeconn" << std::endl;
			setMaxSock(clientlist);
			break;
		}
	}
}

void Socket::checktimeout(std::list<Client> *clientlist, fd_set *readset, fd_set *writeset)
{
	for ( std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
			if (time(NULL) - it->_last_msg > 10 && it->_client_socket != -1)
			{
				//std::cout << "Client number {" << it->_clientnumber << "} has timed out" << std::endl;
				closeconnection(clientlist, it->_client_socket, readset, writeset);
				setMaxSock(clientlist);
			}
	}
}

void Socket::readrequest(std::list<Client> *clientlist, int fd, long rcv, fd_set *readset, fd_set *writeset, char *buffer)
{
	std::string str;


	if (rcv < 0)
	{
		closeconnection(clientlist, fd, readset, writeset);
		setMaxSock(clientlist);
		perror("recv");
		exit(1);
	}
	else if (rcv == 0)
	{
		closeconnection(clientlist, fd, readset, writeset);
		setMaxSock(clientlist);
	}
	else
	{
		for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
		{
			if (fd == it->_client_socket)
			{
				it->_buff = buffer;
				it->_last_msg = time(NULL);
				it->_bytesrcv = rcv;
				max_sock = addfdtoset(fd, writeset, max_sock);
				max_sock = rmfdfromset(fd, readset, max_sock);

				std::cout << "Request received from Client " << it->_clientnumber << std::endl;
				//str = buffer;
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
	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
		if (it->_client_socket > max_sock)
			max_sock = it->_client_socket;
	}
	//std::cout << "max sock is: " << max_sock << std::endl;
}

void Socket::sendresponse(std::list<Client> *clientlist, int fd)
{
	std::string response;
	std::ifstream file;
	std::string filename = ".";
	std::string line;

	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
		if (it->_client_socket == fd)
		{
			std::cout << "req is: " << it->_buff << std::endl;
			it->_req.parse(it->_buff.c_str(), it->_bytesrcv);
			it->_req.printMessage();
			if (!it->_req.isParsingDone())
			{
				std::cerr<< "Bad request in sendreponse" << std::endl;
				//exit(1);
			}
			switch(it->_req.getMethod())
			{
				//	METTRE LES CONFIG DANS LES CREATIONS DES methodHandler
				case GET: 
				{
					GetRequestHandler	methodHandler;
					it->_resp = methodHandler.handleRequest(it->_req);
					break;
				}
				case POST: 
				{
					PostRequestHandler	methodHandler;
					it->_resp = methodHandler.handleRequest(it->_req);
					break;
				}
				case DELETE: 
				{
					DeleteRequestHandler	methodHandler;
					it->_resp = methodHandler.handleRequest(it->_req);
					break;
				}
				case NONE:
					break;
			}
		}
		std::cout << "resp is: " << it->_resp.getResp() << std::endl;
		exit(1);
	}
}