/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.class.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:02:28 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/07 17:38:08 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.class.hpp"

RequestHandler::RequestHandler(void)
{
	// std::cout << "RequestHandler default constructor called" << std::endl;
}

// RequestHandler::RequestHandler(CONFIG SERVER)
// {
// 	// std::cout << "RequestHandler default constructor called" << std::endl;
// 	// _conf = CONFIG SERVER;
// }

RequestHandler::RequestHandler(const RequestHandler& src)
{
	// std::cout << "Copy RequestHandler constructor called" << std::endl;
	// _config = src._config;
	(void)src;
}

RequestHandler&	RequestHandler::operator=(const RequestHandler& src)
{
	// std::cout << "Assignement RequestHandler constructor called" << std::endl;
	// _config = src._config;
	(void)src;
	return (*this);
}

RequestHandler::~RequestHandler(void)
{
	// std::cout << "RequestHandler destructor called" << std::endl;
}
