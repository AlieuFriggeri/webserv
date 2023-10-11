/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 15:09:38 by vgroux            #+#    #+#             */
/*   Updated: 2023/10/11 18:53:11 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_CLASS_HPP
# define HTTPREQUEST_CLASS_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <map>

# define URI_MAX_LEN 4096

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
	REQUEST_LINE_URI_QUERY,
	REQUEST_LINE_URI_FRAGMENT,
	REQUEST_LINE_SPACE_AFTER_URI,
	REQUEST_LINE_H,
	REQUEST_LINE_HT,
	REQUEST_LINE_HTT,
	REQUEST_LINE_HTTP,
	REQUEST_LINE_HTTP_SLASH,
	REQUEST_LINE_MAJOR_DIGIT,
	REQUEST_LINE_DOT,
	REQUEST_LINE_MINOR_DIGIT,
	REQUEST_LINE_CR,
	REQUEST_LINE_LF,
	FIELDS_START,
	FIELDS_KEY,
	FIELDS_VALUE,
	FIELDS_VALUE_END,
	FIELDS_END,
	PARSING_DONE
};

class HttpRequest
{
	private:
		HttpMethod							_method;
		std::map<int, std::string>			_method_str;
		std::map<std::string, std::string>	_headers;
		std::string							_path;
		std::string							_query;
		std::string							_fragment;
		std::string							_host;
		std::string							_user_agent;
		std::string							_accept;
		std::string							_acc_lang;
		std::string							_acc_encode;
		std::string							_conn;
		std::string							_up_ins_req;
		int									_err_code;
		int									_ver_maj;
		int									_ver_min;
		std::string							_tmp;
		ParsingState						_state;

	public:
		HttpRequest();
		HttpRequest(const HttpRequest& src);
		HttpRequest& operator=(const HttpRequest& src);
		~HttpRequest();

		HttpMethod	getMethod(void) const;
		std::string	getPath(void) const;

		void		parse(char *data, size_t len);
		void		setHeader(std::string key, std::string value);
};

#endif
