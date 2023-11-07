/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequestHandler.class.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:08:04 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/07 17:42:40 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetRequestHandler.class.hpp"

GetRequestHandler::GetRequestHandler(void)
{
	// std::cout << "GetRequestHandler default constructor called" << std::endl;
}

// GetRequestHandler::GetRequestHandler( CONFIG DU SERVER )
// {
// 	// std::cout << "GetRequestHandler constructor called" < <std::endl;
// 	// _conf = config;
// }

GetRequestHandler::GetRequestHandler(const GetRequestHandler &src)
{
	// std::cout << "Copy GetRequestHandler constructor called" << std::endl;
	if (this != &src)
	{
		// _conf = src._conf;
	}
}

GetRequestHandler&	GetRequestHandler::operator=(const GetRequestHandler &src)
{
	// std::cout << "Assignement GetRequestHandler constructor called" << std::endl;
	// _conf = src._conf;
	(void)src;
	return (*this);
}


GetRequestHandler::~GetRequestHandler(void)
{
	// std::cout << "GetRequestHandler destructor called" << std::endl;
}

HttpRespond	GetRequestHandler::handleRequest(HttpRequest req)
{
	HttpRespond	resp;

	if (req.isParsingDone() == false)
		std::cerr << "Le parsing de la requete a rencontre une erreur" << std::endl;
	else
	{
		if (req.getQuery() != "" && req.getFragment() != "")
		{
			resp.setBody(""/* CGI */);
		}

	}
	resp.build(req);
	return (resp);
}
