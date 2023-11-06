/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRespond.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:11:18 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/06 17:46:05 by vgroux           ###   ########.fr       */
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

bool	HttpRespond::addHeader(std::string key, std::string value)
{
	if (_headers.count(key) != 0)
		return (false);
	_headers[key] = value;
	return (true);
}

std::string	HttpRespond::generateRequestLine(void)
{
	std::string	resp;
	resp = "HTTP/1.1 " + toString(_status_code) + " " + getStatusStr(_status_code) + CRLN;
	return (resp);
}

std::string	HttpRespond::generateHeaders(void)
{
	std::string	heads;
	if (_body != "")
	{
		int	len = _body.length();
		heads += "Content-Length: " + toString(len) += CRLN;
	}
	for (std::map<std::string, std::string>::const_iterator i = _headers.begin(); i != _headers.end(); i++)
		heads += i->first + ":" + i->second + CRLN;
	heads += CRLN;
	return (heads);
}

bool HttpRespond::built(HttpRequest req)
{
	_isBuilt = false;
	_status_code = req.getErrorCode();
	if (_status_code != 0)
	{
		/*
		
		GERER LA REQUETE, L'APPEL ET RETOUR DU CGI, ETC...
		
		*/
		_resp = generateRequestLine();
		_resp += generateHeaders();
		_resp += _body + CRLN;
	}
	else
		_resp = generateRequestLine();
	_isBuilt = true;
	return (true);
}

bool HttpRespond::isBuilt(void)
{
	return (_isBuilt);
}

std::string	HttpRespond::getResp(void)
{
	if (_isBuilt == false)
		return ("");
	return (_resp);
}
