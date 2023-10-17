/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 16:26:45 by vgroux            #+#    #+#             */
/*   Updated: 2023/10/17 19:50:34 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.class.hpp"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	HttpRequest	test;
	std::string	str = "GET /index.html HTTP/1.1";

	test.parse((char *)str.c_str(), str.length());
	return 0;
}