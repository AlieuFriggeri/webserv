/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostRequestHandler.class.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:10:16 by vgroux            #+#    #+#             */
/*   Updated: 2023/12/01 16:05:16 by vgroux           ###   ########.fr       */
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

void	PostRequestHandler::handlePostFile(HttpRequest *req)
{
	(void)req;
}

HttpRespond	PostRequestHandler::handleRequest(HttpRequest *req, Client *clt, Socket srv)
{
	(void)clt;
	(void)srv;
	HttpRespond	resp;
	if (req->isParsingDone() == false)
		std::cerr << "Le parsing de la requete a rencontre une erreur" << std::endl;
	else if (req->getPathRelative().empty())
	{
		std::cerr << "Le fichier/dossier n'existe pas ou les droits ne sont pas corrects" << std::endl;
		req->setErrorCode(404);
	}
	std::cout << "Status code = " << req->getErrorCode() << std::endl;
	if (req->getErrorCode() == 0)
	{
		/**
		 * si upload fichier ->
		 * 				read requete
		 * 				open fichier
		 * 				write le content read depuis la requete du Client
		 * si upload un form ->
		 * 				envoyer le contenu de la requet au cgi
		 * 				ajouter une ligne dans user data.txt
		 * 				actualiser la page
		*/  
		if (req->getPath().find(".php") == req->getPath().size() - 4)
		{
			std::string cgiresp;
			std::cout << "entering cgi" << std::endl;
			cgiresp = CgiExecutor::execute(clt, srv, "/usr/bin/php");
			std::cout << "CGI resp is : " << std::endl << cgiresp << std::endl;
			resp.setBody(cgiresp);
			resp.setStatus(200);
		}
		else 
		{
			if (req->isMultiform())
			{
				// for (std::vector<Boundary>::iterator it = req->getBounded().begin(); it != req->getBounded().end(); it++)
				// {
				// 	;
				// }
			}
			else
			{
				// traitement "classique"
			}
		}	
	}
	else
	{
		/* GESTION ERREUR */
		resp.setStatus(req->getErrorCode());
		resp.setBody(handleErrorPage(srv, req->getErrorCode()));
	}
	resp.build(*req);
	return (resp);
}
