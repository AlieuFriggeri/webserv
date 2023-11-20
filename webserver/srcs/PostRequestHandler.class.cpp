/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostRequestHandler.class.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:10:16 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/20 18:39:58 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostRequestHandler.class.hpp"

PostRequestHandler::PostRequestHandler(void)
{
	// std::cout << "PostRequestHandler default constructor called" << std::endl;
}

// PostRequestHandler::PostRequestHandler( CONFIG DU SERVER )
// {
// 	// std::cout << "PostRequestHandler default constructor called" << std::endl;
// 	// _conf = src._conf;
// }

PostRequestHandler::PostRequestHandler(const PostRequestHandler &src): RequestHandler(src)
{
	// std::cout << "Copy PostRequestHandler constructor called" << std::endl;
	if (this != &src)
	{
		// _conf = src._conf;
	}
}

PostRequestHandler&	PostRequestHandler::operator=(const PostRequestHandler &src)
{
	// std::cout << "Assignement PostRequestHandler constructor called" << std::endl;
	// _conf = src._conf;
	(void)src;
	return (*this);
}

PostRequestHandler::~PostRequestHandler(void)
{
	// std::cout << "PostRequestHandler destructor called" << std::endl;
}

HttpRespond	PostRequestHandler::handleRequest(HttpRequest *req, Client clt, Socket srv)
{
	HttpRespond	resp;

	if (req->isParsingDone() == false)
		std::cerr << "Le parsing de la requete a rencontre une erreur" << std::endl;
	
	resp.build(*req);
	return (resp);
}
