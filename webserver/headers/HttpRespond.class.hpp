/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRespond.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:55:44 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/06 17:30:07 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPOND_CLASS_HPP
# define HTTP_RESPOND_CLASS_HPP

# include "HttpRequest.class.hpp"
# include "utils.hpp"
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
		std::string							_resp;
		bool								_isBuilt;

		std::string	generateRequestLine(void);
		std::string	generateHeaders(void);

	public:
		HttpRespond(void);
		~HttpRespond(void);

		bool	addHeader(std::string key, std::string value);
		bool	built(HttpRequest req);
		bool	isBuilt(void);
		
		std::string	getResp(void);
};

#endif
