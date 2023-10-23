/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRespond.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:55:44 by vgroux            #+#    #+#             */
/*   Updated: 2023/10/23 17:19:56 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPOND_CLASS_HPP
# define HTTP_RESPOND_CLASS_HPP

# include "HttpRequest.class.hpp"
# include <string.h>
# include <iostream>
# include <string>
# include <sstream>
# include <map>
class HttpRespond
{
	private:
		int									_status_code;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

	public:
		HttpRespond(void);
		~HttpRespond(void);
		
};

#endif
