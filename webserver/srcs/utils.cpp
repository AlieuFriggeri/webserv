/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afrigger <afrigger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:45:38 by vgroux            #+#    #+#             */
/*   Updated: 2024/01/09 17:26:25 by afrigger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string	getStatusStr(int statusCode)
{
	switch (statusCode)
	{
		case 100:
			return "Continue";
		case 101:
			return "Switching Protocol";
		case 200:
			return "OK";
		case 201:
			return "Created";
		case 202:
			return "Accepted";
		case 203:
			return "Non-Authoritative Information";
		case 204:
			return "No Content";
		case 205:
			return "Reset Content";
		case 206:
			return "Partial Content";
		case 300:
			return "Multiple Choice";
		case 301:
			return "Moved Permanently";
		case 302:
			return "Moved Temporarily";
		case 303:
			return "See Other";
		case 304:
			return "Not Modified";
		case 307:
			return "Temporary Redirect";
		case 308:
			return "Permanent Redirect";
		case 400:
			return "Bad Request";
		case 401:
			return "Unauthorized";
		case 403:
			return "Forbidden";
		case 404:
			return "Not Found";
		case 405:
			return "Method Not Allowed";
		case 406:
			return "Not Acceptable";
		case 407:
			return "Proxy Authentication Required";
		case 408:
			return "Request Timeout";
		case 409:
			return "Conflict";
		case 410:
			return "Gone";
		case 411:
			return "Length Required";
		case 412:
			return "Precondition Failed";
		case 413:
			return "Payload Too Large";
		case 414:
			return "URI Too Long";
		case 415:
			return "Unsupported Media Type";
		case 416:
			return "Requested Range Not Satisfiable";
		case 417:
			return "Expectation Failed";
		case 418:
			return "I'm a teapot";
		case 421:
			return "Misdirected Request";
		case 425:
			return "Too Early";
		case 426:
			return "Upgrade Required";
		case 428:
			return "Precondition Required";
		case 429:
			return "Too Many Requests";
		case 431:
			return "Request Header Fields Too Large";
		case 451:
			return "Unavailable for Legal Reasons";
		case 500:
			return "Internal Server Error";
		case 501:
			return "Not Implemented";
		case 502:
			return "Bad Gateway";
		case 503:
			return "Service Unavailable";
		case 504:
			return "Gateway Timeout";
		case 505:
			return "HTTP Version Not Supported";
		case 506:
			return "Variant Also Negotiates";
		case 507:
			return "Insufficient Storage";
		case 510:
			return "Not Extended";
		case 511:
			return "Network Authentication Required";
		default:
			return "Undefined";
	}
}

std::string	toString(int i)
{
	std::stringstream ss;
    ss << i;
    return (ss.str());
}

std::string openReadFile(std::string relative_path)
{
	std::ifstream	file;
	std::string		result;
	std::ostringstream content;

	file.open(relative_path);
	if (file.is_open())
	{
		content << file.rdbuf();
		result = content.str();

		file.close();
	}
	// std::string		line;


	// result.clear();
	// if (relative_path.c_str()[0] == '.' && relative_path.c_str()[1] == '/')
	// 	relative_path.erase(0, 2);
	// file.open(relative_path);
	// if (file.is_open())
	// {
	// 	while (std::getline(file, line))
	// 	{
	// 		result += line;
	// 		result += "\n";
	// 	}
	// 	file.close();
	// }
	else
		std::cerr << "Erreur lors de l'ouverture du fichier" << std::endl;
	return result;
}

std::string	openReadCloseDir(std::string path, std::string uri)
{
	std::vector<std::string>	files;
	std::string					html;
	struct dirent	*ent;

	DIR *dir = opendir(path.c_str());
	if (dir != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
			files.push_back(ent->d_name);
		closedir(dir);

		// Supprimme le ./www/ au debut des url afin que les links fonctionnent.
		if (uri.find("./www/") != std::string::npos)
			uri.erase(uri.find("./www/"), uri.find("./www/") + 5);
		html = "<html><head><title>Index of " + path + "</title></head><body><h1>Index of " + uri + "</h1><hr>\n";
		for (std::vector<std::string>::iterator it = files.begin(); it < files.end(); it++)
		{
			struct stat s;

			if (stat(it->c_str(), &s))
			{
				if (s.st_mode & S_IFDIR)
				{	// is a directory
					std::cout << *it << " is a directory" << std::endl;
				}
				else if (s.st_mode & S_IFREG)
				{	// is a file
					std::cout << *it << " is a file" << std::endl;
				}
				if (uri.rfind('/') == uri.length() - 1)
					html += "<a href=\"" + uri + *it + "\">" + *it + "</a><br>\n";
				else
					html += "<a href=\"" + uri + "/" + *it + "\">" + *it + "</a><br>\n";
			}
		}
		html += "</hr>\n</body>\r</html>";
	}
	return (html);
}
