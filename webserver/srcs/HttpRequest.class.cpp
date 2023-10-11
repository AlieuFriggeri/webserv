/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 15:09:14 by vgroux            #+#    #+#             */
/*   Updated: 2023/10/11 19:03:47 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.class.hpp"

HttpRequest::HttpRequest(void)
{
	// std::cout << "HttpRequest Default constructor called" << std::endl;
	_method_str[::GET] = "GET";
	_method_str[::POST] = "POST";
	_method_str[::DELETE] = "DELETE";
	_method_str[::NONE] = "NONE";
	return ;
}

HttpRequest::HttpRequest(const HttpRequest& src)
{
	// std::cout << "HttpRequest Copy constructor called" << std::endl;
	return ;
}

HttpRequest&	HttpRequest::operator=(const HttpRequest& src)
{
	// std::cout << "HttpRequest Assignement constructor called" << std::endl;
	return *this;
}

HttpRequest::~HttpRequest(void)
{
	// std::cout << "HttpRequest Destructor called" << std::endl;
}

HttpMethod	HttpRequest::getMethod(void) const
{
	return _method;
}

std::string	HttpRequest::getPath(void) const
{
	return _path;
}

bool    allowedCharURI(char c)
{
    if ((c >= '#' && c <= ';') || (c >= '?' && c <= '[') || (c >= 'a' && c <= 'z') ||
       c == '!' || c == '=' || c == ']' || c == '_' || c == '~')
        return (true);
    return (false);
}

bool	checkUriPath(std::string _path)
{
	char	*tmp;
	tmp = strtok((char *)_path.c_str(), "/");
	int		pos = 0;

	while (tmp != NULL)
	{
		if (!strcmp(tmp, ".."))
			pos--;
		else
			pos++;
		if (pos < 0)
			return (false);
		tmp = strtok(NULL, "/");
	}
	return true;
}

void	HttpRequest::parse(char *data, size_t len)
{
	char				c;
	short				mi = 1;
	std::stringstream	s;
	std::string			temp;

	for (size_t i = 0; i < len; i++)
	{
		c = data[i];
		switch(_state)
		{
			case REQUEST_LINE:
			{
				if (c == 'G')
					_method = GET;
				else if (c == 'P')
					_method = POST;
				else if (c == 'D')
					_method = DELETE;
				else
				{
					_err_code = 501;
					std::cerr << "Method Error in REQUEST_LINE. char is: " << c << std::endl;
					return ;
				}
				_state = REQUEST_LINE_METHOD;
				break ;
			}
			case REQUEST_LINE_METHOD:
			{
				if (c == _method_str[_method][mi])
					mi++;
				else
				{
					_err_code = 501;
					std::cerr << "Method Error in REQUEST_LINE_METHOD. char is: " << c << std::endl;
					return ;
				}

				if ((size_t)mi == _method_str[_method].length())
					_state = REQUEST_LINE_SPACE_BEFORE_URI;
				break ;
			}
			case REQUEST_LINE_SPACE_BEFORE_URI:
			{
				if (c != ' ')
				{
					_err_code = 400;
					std::cerr << "Bad Request (REQUEST_LINE_SPACE_BEFORE_URI)" << std::endl;
					return ;
				}
				_state = REQUEST_LINE_URI_SLASH;
				break ;
			}
			case REQUEST_LINE_URI_SLASH:
			{
				if (c == '/')
				{
					_state = REQUEST_LINE_URI;
					temp.clear();
				}
				else
				{
					_err_code = 400;
					std::cerr << "Bad request (REQUEST_LINE_URI_SLASH)" << std::endl;
				}
				break ;
			}
			case REQUEST_LINE_URI:
			{
				if (c == ' ')
				{
					_path.append(temp);
					temp.clear();
					_state = REQUEST_LINE_H;
					continue ;
				}
				else if (c == '?')
				{
					_path.append(temp);
					temp.clear();
					_state = REQUEST_LINE_URI_QUERY;
					continue ;
				}
				else if (c == '#')
				{
					_path.append(temp);
					temp.clear();
					_state = REQUEST_LINE_URI_FRAGMENT;
					continue ;
				}
				else if (!allowedCharURI(c))
				{
					_err_code = 400;
					std::cerr << "Bad request (REQUEST_LINE_URI)" << std::endl;
					return ;
				}
				else if (i > URI_MAX_LEN)
				{
					_err_code = 414;
					std::cerr << "URI Too Long (REQUEST_LINE_URI)" << std::endl;
					return ;
				}
				break ;
			}
			case REQUEST_LINE_URI_QUERY:
			{
				if (c == ' ')
				{
					_query.append(temp);
					temp.clear();
					_state = REQUEST_LINE_H;
					continue ;
				}
				else if (c == '#')
				{
					_query.append(temp);
					temp.clear();
					_state = REQUEST_LINE_URI_FRAGMENT;
					continue ;
				}
				else if (!allowedCharURI(c))
				{
					_err_code = 400;
					std::cerr << "Bad request (REQUEST_LINE_URI_QUERY)" << std::endl;
					return ;
				}
				else if (i > URI_MAX_LEN)
				{
					_err_code = 414;
					std::cerr << "URI Too Long (REQUEST_LINE_URI_QUERY)" << std::endl;
					return ;
				}
				break ;
			}
			case REQUEST_LINE_URI_FRAGMENT:
			{
				if (c == ' ')
				{
					_query.append(temp);
					temp.clear();
					_state = REQUEST_LINE_H ;
					continue ;
				}
				else if (!allowedCharURI(c))
				{
					_err_code = 400;
					std::cerr << "Bad request (REQUEST_LINE_URI_FRAGMENT)" << std::endl;
					return ;
				}
				else if (i > URI_MAX_LEN)
				{
					_err_code = 414;
					std::cerr << "URI Too Long (REQUEST_LINE_URI_FRAGMENT)" << std::endl;
					return ;
				}
				break ;
			}
			case REQUEST_LINE_SPACE_AFTER_URI:
			{
				if (c == ' ')
					_state = REQUEST_LINE_H;
				else
				{
					_err_code = 400;
					std::cerr << "Bad Request (REQUEST_LINE_SPACE_AFTER_URI" << std::endl;
					return ;
				}
			}
			case REQUEST_LINE_H:
			{
				if (c != 'H')
				{
					_err_code = 400;
					std::cerr << "Bad Request (REQUEST_LINE_H)" << std::endl;
					return ;
				}
				else if (!checkUriPath(_path))
				{
					_err_code = 400;
					std::cerr << "Request URI Error (REQUEST_LINE_H)" << std::endl;
					return ;
				}
				_state = REQUEST_LINE_HT;
				break;
			}
			case REQUEST_LINE_HT:
			{
				if (c != 'T')
				{
					_err_code = 400;
					std::cerr << "Bad Request (REQUEST_LINE_HT)" << std::endl;
					return ;
				}
				_state = REQUEST_LINE_HTT;
				break ;
			}
			case REQUEST_LINE_HTT:
			{
				if (c != 'T')
				{
					_err_code = 400;
					std::cerr << "Bad Request (REQUEST_LINE_HTT)" << std::endl;
					return ;
				}
				_state = REQUEST_LINE_HTTP;
				break ;
			}
			case REQUEST_LINE_HTTP:
			{
				if (c != 'P')
				{
					_err_code = 400;
					std::cerr << "Bad Request (REQUEST_LINE_HTTP)" << std::endl;
					return ;
				}
				_state = REQUEST_LINE_HTTP_SLASH;
				break ;
			}
			case REQUEST_LINE_HTTP_SLASH:
			{
				if (c != '/')
				{
					_err_code = 400;
					std::cerr << "Bad Request (REQUEST_LINE_HTTP_SLASH)" << std::endl;
					return ;
				}
				_state = REQUEST_LINE_MAJOR_DIGIT;
				break ;
			}
			case REQUEST_LINE_MAJOR_DIGIT:
			{
				if (!isdigit(c))
				{
					_err_code = 400;
					std::cerr << "Bad Request (REQUEST_LINE_MAJOR_DIGIT)" << std::endl;
					return ;
				}
				_ver_maj = c - '0';
				_state = REQUEST_LINE_DOT;
				break ;
			}
			case REQUEST_LINE_DOT:
			{
				if (c != '.')
				{
					_err_code = 400;
					std::cerr << "Bad Request (REQUEST_LINE_DOT)" << std::endl;
					return ;
				}
				_state = REQUEST_LINE_MINOR_DIGIT;
				break ;
			}
			case REQUEST_LINE_MINOR_DIGIT:
			{
				if (!isdigit(c))
				{
					_err_code = 400;
					std::cerr << "Bad Request (REQUEST_LINE_MINOR_DIGIT)" << std::endl;
					return ;
				}
				_ver_min = c - '0';
				_state = REQUEST_LINE_CR;
				break ;
			}
			case REQUEST_LINE_CR:
			{
				if (c != '\r')
				{
					_err_code = 400;
					std::cerr << "Bad Request (REQUEST_LINE_CR)";
					return ;
				}
				_state = REQUEST_LINE_LF;
				break ;
			}
			case REQUEST_LINE_LF:
			{
				if (c != '\n')
				{
					_err_code = 400;
					std::cerr << "Bad Request (REQUEST_LINE_LF)" << std::endl;
					return ;
				}
				_state = FIELDS_START;
				break ;
			}
			case FIELDS_START:
			{
				if (c == '\r')
					_state = FIELDS_END;
				else if (c )
				_state = FIELDS_KEY;
				std::cout << std::endl << temp << std::endl << std::endl;
				break ;
			}
			case FIELDS_KEY:
			{
				if (c == ':')
				{
					_tmp = temp; // _tmp devient la clé
					temp.clear();
					_state = FIELDS_VALUE;
					continue ;
				}
				break ;
			}
			case FIELDS_VALUE:
			{
				if (c == '\r')
				{
					setHeader(_tmp, temp); // _tmp = key | temp = value
					_tmp.clear();
					temp.clear();
					continue ;
				}
				break ;
			}
			case FIELDS_VALUE_END:
			{
				if (c == '\n')
				{
					_state = FIELDS_START;
					continue ;
				}
				else
				{
					_err_code = 400;
					std::cerr << "Bad Request (FIELDS_VALUE_END)" << std::endl;
					return ;
				}
				break ;
			}
			case FIELDS_END:
			{
				if (c == '\n')
				{
					temp.clear();


					
				}
				else
				{
					_err_code = 400;
					std::cerr << "Bad Request (FIELDS_END)" << std::endl;
					return ;
				}
				break ;
			}
		}
		temp += c;
		if (_state == PARSING_DONE)
			return ;
	}
}

void	HttpRequest::setHeader(std::string key, std::string value)
{
	// key to lower
	for (size_t i = 0; i < key.length(); i++)
		key[i] = std::tolower(key[i]);

	// trim spaces before and after the value
	value.erase(0, value.find_first_not_of(" \t"));
	value.erase(value.find_last_not_of(" \t") + 1);
	
	_headers[key] = value;
}
