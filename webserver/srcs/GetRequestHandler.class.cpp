/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequestHandler.class.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afrigger <afrigger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:08:04 by vgroux            #+#    #+#             */
/*   Updated: 2024/01/19 15:41:39 by afrigger         ###   ########.fr       */
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

HttpRespond	GetRequestHandler::handleRequest(HttpRequest *req, Client *clt, Socket srv)
{
	HttpRespond	resp;
	std::cout << "path IN GET METHOD AT THE START  = " << req->getPath() << std::endl;
	if (req->getPath().find("upload") != std::string::npos)
	{
		if (srv.getDownload() == "upload")
		{
			req->setPath(req->getPath().erase(1, 4));
			std::cout << req->getPath() << std::endl;
		}
		else
		{
			req->setPath(req->getPath().erase(0, 12));
			std::string finalpath = "./" + srv.getDownload() + req->getPath();
			req->setPath(finalpath);
			req->setPathRelative(finalpath);
			
		}
		std::cout << "PATH IN GET "<< req->getPath() << std::endl;
		std::cout << "PATH RELATIVE IN GET "<< req->getPathRelative() << std::endl;
	}
	if (req->isParsingDone() == false)
		std::cerr << "Le parsing de la requete a rencontre une erreur" << std::endl;
	else if (req->getPathRelative().empty())
	{
		std::cerr << "Le fichier/dossier n'existe pas ou les droits ne sont pas corrects" << std::endl;
		req->setErrorCode(404);
	}
	if (req->getErrorCode() == 0)
	{
		if (req->isDirectory() == false)
		{
			if (req->getPath().find(".php") == req->getPath().size() - 4)
			{
				std::string cgiresp;
				// std::cout << "entering cgi" << std::endl;
				cgiresp = CgiExecutor::execute(clt, srv, "/usr/bin/php");
				std::cout << "CGI resp is : " << std::endl << cgiresp << std::endl;
				if (cgiresp == "timeout")
				{
					resp.setStatus(502);
					resp.setBody(handleErrorPage(srv, resp.getStatus()));
				}
				else
				{
					resp.setBody(cgiresp);
					resp.setStatus(200);
				}
			}
			else if (req->getQuery().empty())
			{
				std::cout << " relative path is " << req->getPathRelative() << std::endl;
				resp.setBody(openReadFile(req->getPathRelative()));
				resp.setStatus(200);
			}
			else
			{
				std::string cgiresp;
				// std::cout << "entering cgi" << std::endl;
				cgiresp = CgiExecutor::execute(clt, srv, "/usr/bin/php");
				std::cout << "CGI resp is : " << std::endl << cgiresp << std::endl;
				if (cgiresp == "timeout")
				{
					resp.setStatus(502);
					resp.setBody(handleErrorPage(srv, req->getErrorCode()));
				}
				else
				{
					resp.setBody(cgiresp);
					resp.setStatus(200);
				}
			}
		}
		else
		{
			/* DIR */
			resp.setBody(openReadCloseDir(req->getPathRelative(), req->getPath()));
		}
	}
	else
	{
		/* ERREUR */
		resp.setStatus(req->getErrorCode());
		resp.setBody(handleErrorPage(srv, req->getErrorCode()));
	}
	resp.build(*req);
	return (resp);
}
