#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <poll.h>
#include <sys/event.h>

#define PORT 8080

int main( void ) {
	// initialisation de mypoll
	//int counter = 0;
	struct pollfd mypoll[2];
	bzero(&mypoll, sizeof(mypoll));
	// creation du socket
	int listening_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listening_socket < 0)
	{
		std::cerr << "Impossible de creer le socket" << std::endl;
		return -1;
	}
	// bind du socket a l'ip et au port
	sockaddr_in serveur;

	serveur.sin_family = AF_INET;
	serveur.sin_port = htons(PORT);
	inet_pton(AF_INET, "0.0.0.0", &serveur.sin_addr);

	if (bind(listening_socket, (sockaddr *)&serveur, sizeof(serveur)) < 0)
	{
		std::cerr << "Impossible de bind l'ip et / ou le port au socket" << std::endl;
		return (-2);
	}

//	mypoll[0].fd = listening_socket;
//	mypoll[0].events = POLLIN;

	// listen sur le socket
	if (listen(listening_socket, 128) < 0)
	{
		std::cerr << "Erreur lors du listen sur le socket" << std::endl;
		return (-3);
	}

	// accepter l'appel
	sockaddr_in client;
	socklen_t clientsize = sizeof(client);
	char host[NI_MAXHOST];
	char svc[NI_MAXSERV];

	//int clientsocket = accept(listening_socket, (sockaddr *)&client, &clientsize);
	for (size_t i = 0; i < 2; i++)
	{
		clientsize = sizeof(client);
		bzero(&client, sizeof(client));
		mypoll[i].fd = accept(listening_socket, (sockaddr *)&client, &clientsize);
		mypoll[i].events = POLLIN;
	}
	

	for(int i = 0; i < 2; i++)
	{
		if (mypoll[i].fd < 0)
		{
			std::cerr << "Probleme lors de la connexion au client" << std::endl;
			return (-4);
		}
	}

	// close le socket quon listen
	close(listening_socket);

	bzero(host, NI_MAXHOST);
	bzero(svc, NI_MAXSERV);

	int result = getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

	if (result)
	{
		std::cout << host << " - s'est connecte a : " << svc << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " - s'est connecte a : " << ntohs(client.sin_port) << std::endl;
	}

	// tant quon recoit des requete, afficher un message
	char buffer[4096];

	while (true)
	{

		poll(mypoll, 2, 100);

		// clear le buffer

		// attendre un message
		for ( int i = 0; i < 2; i++)
		{
		bzero(buffer, sizeof(buffer));
		int bytesRcv = recv(mypoll[i].fd, buffer, sizeof(buffer), 0);

		if (bytesRcv < 0)
		{
			std::cerr << "Probleme lors de la connexion" << std::endl;
			break;
		}

		if (bytesRcv == 0)
		{
			std::cout << "Le client s'est deconnecte" << std::endl;
			break;
		}

		// afficher le message
		std::cout << "Message recu: " << buffer << std::endl;

		// renvoyer un message
		send(mypoll[i].fd, buffer, bytesRcv + 1, 0);
		}
	}

	//std::cout << "Time in ms: " << counter * 100 << std::endl;

	// close le socket
	close(mypoll[0].fd);
	close(mypoll[1].fd);
	return 0;
}