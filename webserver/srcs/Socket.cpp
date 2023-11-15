#include "Socket.hpp"
#include "HttpRequest.class.hpp"

int max_sock;

sockaddr_in Socket::getSockaddr(void)
{
	return this->_server;
}

void Socket::setSockaddr(sockaddr_in &server)
{
	this->_server = server;
}

int Socket::getListening(void)
{
	return this->_listening_socket;
}

void Socket::setListening(int &socket)
{
	this->_listening_socket = socket;
}

char *Socket::getSvc(void)
{
	return this->_svc;
}

std::string Socket::getServerName(void)
{
	return this->_servername;
}

void Socket::setServerName(std::string &name)
{
	this->_servername = name;
}

int Socket::getMaxBodySize(void)
{
	return this->_maxbodysize;
}

void Socket::setMaxBodySize(int max)
{
	this->_maxbodysize = max;
}

int Socket::getPort(void)
{
	return this->_port;
}

void Socket::setPort(int port)
{
	this->_port = port;
}

std::string Socket::getRoot(void)
{
	return this->_root;
}

void Socket::setRoot(std::string root)
{
	this->_root = root;
}

std::map<std::string, Route> Socket::getRouteMap(void)
{
	return this->_route;
}

void Socket::setRouteMap(std::string key, Route value)
{
	this->_route[key] = value;
}

std::map<std::string, std::string> Socket::getConfigMap(void)
{
	return this->_config;
}

void Socket::setConfigMap(std::map<std::string, std::string> map)
{
	this->_config = map;
}

int Socket::getTotalServ(void)
{
	return this->_totalserv;
}

void Socket::setTotalServ(int total)
{
	this->_totalserv = total;
}




Socket::Socket()
{
	bzero(&_svc, sizeof(_svc));
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
			std::cout << "setsockopt error cheh (" << strerror(errno) << ")" << std::endl;
			exit(-9);
		}

		if (bind(servers[i]._listening_socket, (sockaddr *)&servers[i]._server, (socklen_t)sizeof(servers[i]._server)) < 0)
		{
			std::cerr << "Error during socket binding (" << strerror(errno) << ")" << std::endl;
			exit(2);
		}

		if (listen(servers[i]._listening_socket, 128) < 0)
		{
			std::cerr << "Error while listening on socket (" << strerror(errno) << ")" << std::endl;
			exit(3);
		}

		if (fcntl(servers[i]._listening_socket, F_SETFL, O_NONBLOCK) < 0)
		{
			std::cerr << "Error with fcntl (" << strerror(errno) << ")" << std::endl;
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
	for (int j = 0; j < servers[0].getTotalServ(); j++)
	{
		if (servers[j].getListening() == i)
			return servers[j].getPort();
	}
	return -1;
}

int checklisteningsock(int i, Socket *servers)
{
	for (int j = 0; j < servers[0].getTotalServ(); j++)
	{
		if (servers[j].getListening() == i)
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
					// std::cout << "READING FROM SOCKET" << std::endl;
					bzero(buffer, sizeof(buffer));
					rcv = read(i, buffer, sizeof(buffer));
					readrequest(clientlist, i, rcv, &readset, &writeset, buffer);
				}
				else if (FD_ISSET(i, &writecpy))
				{
					for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
					{
						if (it->_client_socket == i)
						{
							sendresponse(clientlist, i, servers);
							closeconnection(clientlist, i, &readset, &writeset);
							// if (it->_req.keepAlive() == false)
							// {
							// 	std::cout << std::endl << "close the connection with the client" << std::endl;
							// }
							// else
							// {
							// 	std::cout << std::endl << "keep the connection with the client" << std::endl;
							// max_sock = rmfdfromset(i, &writeset, max_sock);
							// max_sock = addfdtoset(i, &readset, max_sock);
							// 	//std::cout << "ON CLOSE POUR PAS INFINIT LOOP\t";closeconnection(clientlist, i, &readset, &writeset);
							// }
							// it->_req.resetRequest();
							break;
						}
					}
				}
				rcv = 0;
			}
		}
		checktimeout(clientlist, &readset, &writeset);
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
			//std::cout << i << std::endl;
			//close(it->_client_socket);
			if(close(i) != 0)
			{
				perror("close");
				exit(3);
			}
			// std::cout << i << std::endl;
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
			if (time(NULL) - it->_last_msg > 5 && it->_client_socket != -1)
			{
				std::cout << "Client number {" << it->_clientnumber << "} has timed out" << std::endl;
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

void Socket::sendresponse(std::list<Client> *clientlist, int fd, Socket *servers)
{
	std::string response;
	std::ifstream file;
	std::string filename = ".";
	std::string line;
	std::string cgiresp;

	for (std::list<Client>::iterator it = clientlist->begin(); it != clientlist->end(); it++)
	{
		if (it->_client_socket == fd && it->_bytesrcv > 0)
		{
			it->_req.parse(it->_buff.c_str(), it->_bytesrcv);
			std::cout << "path\t" << it->_req.getPath() << std::endl;
			checkroute(&*it, servers);
			//it->_req.printMessage();
			if (!it->_req.isParsingDone())
				std::cerr<< "Bad request in sendreponse" << std::endl;
			else if ((it->_req.getPathRelative()).empty())
			{
				it->_req.setErrorCode(404);
				std::cerr << "Relative path not found" << std::endl;
			}
			switch(it->_req.getMethod())
			{
				//	METTRE LES CONFIG DANS LES CREATIONS DES methodHandler
				case GET: 
				{
					GetRequestHandler	methodHandler;
					if (it->_req.getPath().find(".php") == it->_req.getPath().size() - 4)
					{
						std::cout << "entering cgi" << std::endl;
						cgiresp = CgiExecutor::execute(&*it, servers[0], "/usr/bin/php");
						std::cout << "CGI resp is : " << cgiresp << std::endl;
					}
					it->_resp = methodHandler.handleRequest(&(it->_req));
					break;
				}
				case POST: 
				{
					PostRequestHandler	methodHandler;
					it->_resp = methodHandler.handleRequest(&(it->_req));
					break;
				}
				case DELETE: 
				{
					DeleteRequestHandler	methodHandler;
					it->_resp = methodHandler.handleRequest(&(it->_req));
					break;
				}
				case NONE:
					break;
			}
			std::cout << "REPONSE= " << it->_resp.getResp();
			write(it->_client_socket, (it->_resp.getResp()).c_str(), strlen((it->_resp.getResp().c_str())));
			std::cout << "Respond sended to Client " << it->_clientnumber << " on socket : " << it->_client_socket << std::endl;
			if (it->_req.keepAlive() == true)
			{
				it->_req.resetRequest();
				it->_req.setKeepAlive(true);
			}
			it->_buff.erase();
			it->_bytesrcv = 0;
		}
	}
}

std::string trimspace(std::string str)
{
	std::string res;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (isspace(str[i]) != true)
			res += str[i];
	}
	return res;
}

void	Socket::checkroute(Client *client, Socket *server)
{
	int i = 0;
	std::string filepath;
	DIR*	dir;
	while (server[i]._listening_socket != client->_serversocket)
		i++;
	for (std::map<std::string, Route>::iterator it = server[i]._route.begin(); it != server[i]._route.end(); it++)
	{
		if (client->_req.getPath().empty())
		{
			std::cout << "Path from request empty" <<std::endl;
			return ;
		}
		filepath = "." + it->second._path + client->_req.getPath().substr(1);
		while(filepath.find_first_of(" ") != std::string::npos)
			filepath.erase(filepath.find_first_of(" "), 1);
		if ((dir = opendir(filepath.c_str())) != NULL)
		{
			closedir(dir);
			client->_req.setDirectory(true);
			client->_req.setPathRelative(filepath);
			break;
		}
		else if (access(filepath.c_str(), F_OK | R_OK) == 0)
		{
			client->_req.setDirectory(false);
			client->_req.setPathRelative(filepath);
			break;
		}
		else
		{
			filepath.clear();
		}
	}
	// std::cout << "File found here " << filepath << std::endl << std::endl;
}
