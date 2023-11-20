/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 18:07:46 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/20 18:08:30 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

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
	Socket::setup(serverarray);
	Socket::handleConnection(&clientlist, serverarray);
	delete [] serverarray;
	return 0;
}
