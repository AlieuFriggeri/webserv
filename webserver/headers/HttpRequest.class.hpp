/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 15:09:38 by vgroux            #+#    #+#             */
/*   Updated: 2023/09/26 17:04:17 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_CLASS_HPP
# define HTTPREQUEST_CALL_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <map>

enum HttpMethod
{
	GET,
	POST,
	DELETE,
	NONE
};

enum ParsingState
{
	REQUEST_LINE,
	REQUEST_LINE_METHOD,
	REQUEST_LINE_FIRST_SPACE,
	REQUEST_LINE_URI,
	PARSING_DONE
};

class HttpRequest
{
	private:
		HttpMethod					_method;
		std::map<int, std::string>	_method_str;
		std::string					_path;
		std::string					_host;
		std::string					_user_agent;
		std::string					_accept;
		std::string					_acc_lang;
		std::string					_acc_encode;
		std::string					_conn;
		std::string					_up_ins_req;
		int							_err_code;
		ParsingState				_state;

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
