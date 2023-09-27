/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 15:09:38 by vgroux            #+#    #+#             */
/*   Updated: 2023/09/27 16:58:05 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_CLASS_HPP
# define HTTPREQUEST_CLASS_HPP

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
	REQUEST_LINE_SPACE_BEFORE_URI,
	REQUEST_LINE_URI_SLASH,
	REQUEST_LINE_URI,
	REQUEST_LINE_SPACE_AFTER_URI,
	REQUEST_LINE_H,
	REQUEST_LINE_HT,
	REQUEST_LINE_HTT,
	REQUEST_LINE_HTTP,
	REQUEST_LINE_MAJOR_DIGIT,
	REQUEST_LINE_DOT,
	REQUEST_LINE_MINOR_DIGIT,
	HOST,
	HOST_DOUBLE_DOT,
	HOST_PORT,
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
