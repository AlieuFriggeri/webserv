/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 15:09:14 by vgroux            #+#    #+#             */
/*   Updated: 2023/09/26 15:59:39 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.class.hpp"

HttpRequest::HttpRequest(void)
{
	std::cout << "HttpRequest Default constructor called" << std::endl;
	return ;
}

HttpRequest::HttpRequest(const HttpRequest& src)
{
	std::cout << "HttpRequest Copy constructor called" << std::endl;
	return ;
}

HttpRequest&	HttpRequest::operator=(const HttpRequest& src)
{
	std::cout << "HttpRequest Assignement constructor called" << std::endl;
	return *this;
}

HttpRequest::~HttpRequest(void)
{
	std::cout << "HttpRequest Destructor called" << std::endl;
}

HttpMethod	HttpRequest::getMethod(void) const
{
	return _method;
}

std::string	HttpRequest::getPath(void) const
{
	return _path;
}

void	HttpRequest::parse(char *data, size_t len)
{

}
