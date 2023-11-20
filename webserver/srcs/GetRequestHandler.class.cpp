/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequestHandler.class.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:08:04 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/20 18:39:15 by vgroux           ###   ########.fr       */
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

GetRequestHandler::GetRequestHandler(const GetRequestHandler &src): RequestHandler(src)
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

HttpRespond	GetRequestHandler::handleRequest(HttpRequest *req, Client clt, Socket srv)
{
	// req->printMessage();
	HttpRespond	resp;

	if (req->isParsingDone() == false)
		std::cerr << "Le parsing de la requete a rencontre une erreur" << std::endl;
	else if (req->getPathRelative().empty())
	{
		std::cerr << "Le fichier/dossier n'existe pas ou les droits ne sont pas corrects" << std::endl;
		req->setErrorCode(404);
	}
	else
	{
		if (req->isDirectory() == false)
		{
			if (req->getQuery().empty())
			{
				resp.setBody(openReadFile(req->getPathRelative()));
				resp.setStatus(200);
			}
			else
			{
				/* THIS REQUEST WILL BE HANDLE BY THE CGI 		*/
				/*		This is because there are some queries	*/
				if (req->getPath().find(".php") == req->getPath().size() - 4)
				{
					std::string cgiresp;
					std::cout << "entering cgi" << std::endl;
					cgiresp = CgiExecutor::execute(clt, srv, "/usr/bin/php");
					std::cout << "CGI resp is : " << std::endl << cgiresp << std::endl;
				}
			}
		}
		else
		{
			/* THIS WILL BE FOR DIRECTORY */
			resp.setBody(openReadCloseDir(req->getPathRelative(), req->getPath()));
		}
	}
	resp.build(*req);
	return (resp);
}
