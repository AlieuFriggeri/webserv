/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRespond.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:11:18 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/22 14:54:45 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRespond.class.hpp"

HttpRespond::HttpRespond(void)
{
	_status_code = 0;
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
	resp = "HTTP/1.1 " + toString(_status_code) + " " + getStatusStr(_status_code) + "\r\n";
	return (resp);
}

std::string	HttpRespond::generateHeaders(void)
{
	std::string	heads;
	
	setHeader("Content-Length", toString(_body.length() + 2));
	if (_body.length() != 0)
	{
		setHeader("Content-Type", "text/html; charset=UTF-8");
	}
	for (std::map<std::string, std::string>::const_iterator i = _headers.begin(); i != _headers.end(); i++)
		heads += i->first + ": " + i->second + "\r\n";
	heads += "\r\n";
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
	setHeader("Date", getDate());
	setHeader("Connection", req.getHeader("connection"));
	if (req.getErrorCode() == 0)
		_status_code = 200;
	else
	{
		_status_code = req.getErrorCode();
		if (_body.empty())
			_body = getStatusStr(_status_code);
	}
	_body += "\r\n";
	_resp = generateStatusLine();
	_resp += generateHeaders();
	_resp += "\r\n" + _body;
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
