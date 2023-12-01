/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRespond.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:11:18 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/30 18:02:37 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRespond.class.hpp"

HttpRespond::HttpRespond(void)
{
	_status_code = 0;
	(void)_headers;
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
	
	setHeader("Content-Length", toString(_body.size() + 2));
	for (std::map<std::string, std::string>::const_iterator i = _headers.begin(); i != _headers.end(); i++)
		heads += i->first + ": " + i->second + "\r\n";
	heads += "\r\n\r\n";
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
	std::string	path = req.getPath();
	_isBuilt = false;
	setHeader("Date", getDate());
	setHeader("Connection", req.getHeader("connection"));
	if (path.find(".png") != std::string::npos)
		setHeader("Content-Type", "image/png");
	else if (path.find(".jpg") != std::string::npos || path.find(".jpeg") != std::string::npos)
		setHeader("Content-Type", "image/jpeg");
	else if (_body.size() != 0)
		setHeader("Content-Type", "text/html; charset=UTF-8");
	if (req.getErrorCode() == 0)
		_status_code = 200;
	else if (req.getErrorCode() == 408)
		setHeader("Connection", "close");
	else
	{
		_status_code = req.getErrorCode();
		std::string	statusLine = getStatusStr(_status_code);
		if (_body.empty())
			_body.insert(_body.end(), statusLine.begin(), statusLine.end());
	}
	_body.push_back('\r');
	_body.push_back('\n');

	std::string statusLine = generateStatusLine();
	_resp.insert(_resp.end(), statusLine.begin(), statusLine.end());

	std::string head = generateHeaders();
	_resp.insert(_resp.end(), head.begin(), head.end());

	_resp.insert(_resp.end(), _body.begin(), _body.end());
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

std::vector<char>	HttpRespond::getBody(void)
{
	return (_body);
}

std::vector<char>	HttpRespond::getResp(void)
{
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

void	HttpRespond::setBody(std::vector<char> body)
{
	_body = body;
}

void	HttpRespond::appendBody(std::vector<char> newBody)
{
	_body.insert(_body.end(), newBody.begin(), newBody.end());
}
