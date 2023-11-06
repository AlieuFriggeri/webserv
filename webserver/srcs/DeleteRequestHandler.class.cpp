/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteRequestHandler.class.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:06:16 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/06 17:44:29 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DeleteRequestHandler.class.hpp"

DeleteRequestHandler::DeleteRequestHandler(void)
{
	// std::cout << "DeleteRequestHandler default constructor called" << std::endl;
}

DeleteRequestHandler::~DeleteRequestHandler(void)
{
	// std::cout << "DeleteRequestHandler destructor called" << std::endl;
}

HttpRespond	DeleteRequestHandler::handleRequest(HttpRequest req)
{
	HttpRespond	resp;

	if (req.isParsingDone() == false)
		std::cerr << "Le parsing de la requete a rencontre une erreur" << std::endl;
	
	resp.built(req);
	return (resp);
}