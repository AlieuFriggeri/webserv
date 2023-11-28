/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afrigger <afrigger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 18:07:46 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/28 13:18:55 by afrigger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
//#include "Socket.hpp"

int main( int ac,char **av)
{
	ServerConfig configuration;
	if (ac != 2)
	{
		std::cerr << "Usage: ./webserv CONFIG_FILE.UDC" << std::endl;
		exit(1);
	}
	std::list<Client> clientlist;
	clientlist.push_back(Client());
	Socket *serverarray = NULL;
	serverarray = ServerConfig::parsefile(av[1]);
	//Socket::setup(serverarray);
	for (int i = 0; i < serverarray[0].getTotalServ(); i++)
		serverarray[i].setListening(Socket::getListenSock(serverarray[i]._config));
	Socket::handleConnection(&clientlist, serverarray);
	delete [] serverarray;
	return 0;
}
