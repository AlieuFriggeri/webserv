/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostRequestHandler.class.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:10:16 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/06 17:44:36 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostRequestHandler.class.hpp"

PostRequestHandler::PostRequestHandler(void)
{
	// std::cout << "PostRequestHandler default constructor called" << std::endl;
}

PostRequestHandler::~PostRequestHandler(void)
{
	// std::cout << "PostRequestHandler destructor called" << std::endl;
}

HttpRespond	PostRequestHandler::handleRequest(HttpRequest req)
{
	HttpRespond	resp;

	if (req.isParsingDone() == false)
		std::cerr << "Le parsing de la requete a rencontre une erreur" << std::endl;
	
	resp.built(req);
	return (resp);
}
