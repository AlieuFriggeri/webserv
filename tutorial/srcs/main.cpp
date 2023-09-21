#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#define PORT 669

int main( void ) {
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

	int clientsocket = accept(listening_socket, (sockaddr *)&client, &clientsize);

	if (clientsocket < 0)
	{
		std::cerr << "Probleme lors de la connexion au client" << std::endl;
		return (-4);
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
		// clear le buffer
		bzero(buffer, sizeof(buffer));

		// attendre un message
		int bytesRcv = recv(clientsocket, buffer, sizeof(buffer), 0);

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
		send(clientsocket, buffer, bytesRcv + 1, 0);

	}
	// close le socket
	close(clientsocket);
	return 0;
}