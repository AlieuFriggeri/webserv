/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 15:09:38 by vgroux            #+#    #+#             */
/*   Updated: 2023/09/26 16:02:17 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_CLASS_HPP
# define HTTPREQUEST_CALL_HPP

# include <iostream>
# include <string>

enum HttpMethod
{
	GET,
	POST,
	DELETE,
	NONE
};

enum ParsingState
{

};

class HttpRequest
{
	private:
		HttpMethod		_method;
		std::string		_path;
		std::string		_host;
		std::string		_user_agent;
		std::string		_accept;
		std::string		_acc_lang;
		std::string		_acc_encode;
		std::string		_conn;
		std::string		_up_ins_req;
		ParsingState	_state;

	public:
		HttpRequest();
		HttpRequest(const HttpRequest& src);
		HttpRequest& operator=(const HttpRequest& src);
		~HttpRequest();

		HttpMethod	getMethod(void) const;
		std::string	getPath(void) const;

		void		parse(char *data, size_t len);
};

#endif
