/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRespond.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:11:18 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/06 18:26:39 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRespond.class.hpp"

HttpRespond::HttpRespond(void)
{
	(void)_status_code;
	(void)_headers;
	_body = "";
	_isBuilt = false;
}

HttpRespond::~HttpRespond(void)
{

}

std::string	HttpRespond::generateStatusLine(void)
{
	std::string	resp;
	resp = "HTTP/1.1 " + toString(_status_code) + " " + getStatusStr(_status_code) + CRLN;
	return (resp);
}

std::string	HttpRespond::generateHeaders(void)
{
	std::string	heads;
	
	if (_body.length() != 0)
	{
		setHeader("Content-Lenght", toString(_body.length()));
		//setHeader("Content-Type", "QQCH");
	}
	for (std::map<std::string, std::string>::const_iterator i = _headers.begin(); i != _headers.end(); i++)
		heads += i->first + ": " + i->second + CRLN;
	heads += CRLN;
	return (heads);
}

std::string	getDate(void)
{
	char buffer[1000];
    time_t now = time(0);
    struct tm *timeinfo = gmtime(&now);

    strftime(buffer, 1000, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    return buffer;
}

bool HttpRespond::build(HttpRequest req)
{
	_isBuilt = false;
	_status_code = req.getErrorCode();
	setHeader("Date", getDate());
	setHeader("Connection", req.getHeader("connection"));
	if (_status_code == 0)
	{
		setHeader("Server", req.getServerName());
		
		/*
		
		GERER LA REQUETE, L'APPEL ET RETOUR DU CGI, ETC...
		
		*/
		_resp = generateStatusLine();
		_resp += generateHeaders();
		_resp += _body + CRLN;
	}
	else // Erreur lors du parsing (status == 400 / 501)
		_resp = generateStatusLine() + generateHeaders() + CRLN;
	_isBuilt = true;
	return (true);
}

bool HttpRespond::isBuild(void)
{
	return (_isBuilt);
}

int	HttpRespond::getStatus(void)
{
	return (_status_code);
}

std::string	HttpRespond::getHeader(std::string key)
{
	return (_headers.at(key));
}

std::map<std::string, std::string>	HttpRespond::getHeaders(void)
{
	return (_headers);
}

std::string	HttpRespond::getBody(void)
{
	return (_body);
}

std::string	HttpRespond::getResp(void)
{
	if (_isBuilt == false)
		return ("");
	return (_resp);
}

void	HttpRespond::setStatus(int status)
{
	_status_code = status;
}

void	HttpRespond::setHeader(std::string key, std::string value)
{
	_headers[key] = value;
}

void	HttpRespond::setBody(std::string body)
{
	_body = body;
}
