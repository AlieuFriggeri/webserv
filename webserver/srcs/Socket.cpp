#include "Socket.hpp"

Socket::Socket()
{

	FD_ZERO(&_write);
	FD_ZERO(&_read);
	FD_ZERO(&_except);
	_timeout.tv_sec = 1;
	_timeout.tv_usec = 0;
	bzero(&_svc, sizeof(_svc));
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

void Socket::handleConnection(int clientsocket)
{

	int bytesRcv;

	bzero(_buffer, sizeof(_buffer));
	bytesRcv = recv(clientsocket, _buffer, sizeof(_buffer), 0);

	if (bytesRcv < 0)
	{
		std::cerr << "Error while connecting" << std::endl;
		return;
	}

	if (bytesRcv == 0)
	{
		std::cout << "Client has disconnected" << std::endl;
		exit(0);
	}

	// afficher le message
	std::cout << "Message recu: " << _buffer << std::endl;

	send(clientsocket, _buffer, bytesRcv + 1, 0);

}

void Socket::prepareConnection(std::list<Client> clientlist)
{
	int max_sock = 0;
	long rcv = 0;
	// if (clientlist.size() == 1)
	// {
	// 	return;
	// }

	FD_ZERO(&_read);
	FD_ZERO(&_write);
	for (std::list<Client>::iterator it = clientlist.begin(); it != clientlist.end(); it++)
	{
		if(it->_client_socket != -1)
		{
			FD_SET(it->_client_socket, &_read);
			//FD_SET(clientsocket, &_write);
		}

	}
	bzero(_buffer, sizeof(_buffer));

	_timeout.tv_sec = 1;
	_timeout.tv_usec = 0;
	// FD_SET(_w, &_write);
	// FD_SET(_e, &_except);

	for (std::list<Client>::iterator it = clientlist.begin(); it != clientlist.end(); it++)
	{
		max_sock = std::max(max_sock, it->_client_socket);
	}
	
	//max_sock = std::max(max_sock, _e);

	rcv = select(max_sock + 1, &_read, NULL, NULL, &_timeout);

	if (rcv < 0)
	{
		perror("select");
		exit(1);
	}
	else if (rcv == 0)
	{
		std::cout << "No pending data" << std::endl;
	}
	else
	{
		// for (int i = 0; i < max_sock + 1; i++)
		// {
		// 	if (FD_ISSET(i, &_write))
		// 	{
		// 		rcv = send(i, _buffer, sizeof(_buffer), 0);
		// 		if (rcv < 0)
		// 		{
		// 			FD_CLR(i, &_write);
		// 			//FD_CLR(i, &_read);
		// 		}
		// 		else if (rcv == 0)
		// 		{
		// 			std::cout << "host terminated connection" << std::endl;
		// 			FD_CLR(i, &_write);
		// 		}
		// 		else
		// 			std::cout << "Sent: " << _buffer << std::endl;
		// 		rcv = 0;
		// 		break;
		// 	}
			
		// }
		
		// if (rcv)
		// 	std::cout << "Connection established" << std::endl;

		for (int i = 0; i < max_sock + 1; i++)
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
					std::cout << "host terminated connection" << std::endl;
					FD_CLR(i, &_read);
					for (std::list<Client>::iterator it = clientlist.begin(); it != clientlist.end(); it++)
					{
						if (it->_client_socket == i)
						{
							clientlist.erase(it);
							return;
						}
					}
					//close(i);
				}
				else	
					std::cout << "Received: " << _buffer << std::endl;
				//exit(1);
				rcv = 0;
				break;
			}
			// else if (FD_ISSET(_w, &_write))
			// {
			// 	std::cout << "Write ready" << std::endl;
			// }
			// else if (FD_ISSET(_e, &_except))
			// {
			// 	std::cerr << "Exception apparement" << std::endl;
			// 	exit(1217);
			// }
		}
		
	}
}