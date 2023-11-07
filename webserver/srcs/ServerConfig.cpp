#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
{
	_configmap["port"] = "default";
	_configmap["servername"] = "default";
	_configmap["maxbodysize"] = "default";
	_configmap["root"] = "default";
}

ServerConfig::~ServerConfig()
{

}

void ServerConfig::erasewhitespaceleft(std::string str)
{
	while (isspace(str[0]))
		str = str.erase(0);
}

Socket *ServerConfig::parsefile(std::string filename)
{
	std::ifstream file;
	std::string content;
	std::string line;
	std::vector<std::string> configs;
	std::vector<std::map<std::string, std::string> > res;
	std::vector<std::map<std::string, Route> >routes;

	file.open(filename);

	if (!file.is_open())
	{
		std::cerr << "cannot open file " << filename << std::endl;
		exit(1);
	}
	while (std::getline(file, line))
	{
		size_t comm_pos = line.find("#");
		if (comm_pos != std::string::npos)
		{
			line = line.substr(0, comm_pos);
			erasewhitespaceleft(line);
		}
		if (!line.empty())
			content += line;
		content += '\n';
	}
	file.close();

	if (filename.find(".UDC") != filename.size() - 4)
	{
		std::cerr << "Bad file extension" << std::endl;
		exit(1);
	}
	if (content.substr(0, 9) != "servers:\n")
	{
		std::cerr << "invalid config file: 'servers' keyword missing on first line" << std::endl;
		exit(1);
	}

	content = content.erase(0, 9);

	configs = splitserv(content);
	int nbserv = configs.size();
	routes = setuproutes(configs);
	// for (std::vector<std::string>::iterator i = configs.begin(); i != configs.end(); i++)
	// {
	// 	std::cout << *i << std::endl;
	// 	std::cout << "-------------------------" << std::endl;
	// }

	res = setupmap(configs);
	Socket *serverarray = new Socket[nbserv];
	configservers(res, routes, serverarray);
	serverarray[0]._totalserv = nbserv;
	//exit(1);
	return serverarray;
}

std::string removespace(std::string key)
{
	size_t i = 0;
	std::string res;
	while (i < key.size())
	{
		if (isspace(key[i]) == false)
			res += key[i];
		i++;
	}
	return res;
}

void ServerConfig::configservers(std::vector<std::map<std::string, std::string> > configs, std::vector<std::map<std::string, Route> > routes, Socket *serverarray)
{
	size_t i = 0;
	for (std::vector<std::map<std::string, std::string> >::iterator it = configs.begin(); it != configs.end(); it++)
	{
		
		std::map<std::string, std::string> maptmp = *it;
		serverarray[i]._port = atoi(maptmp["port"].c_str());
		serverarray[i]._maxbodysize = atoi(maptmp["clientbody"].c_str());
		serverarray[i]._servername = maptmp["server_name"];
		i++;
		
	}
	i = 0;
	for (std::vector<std::map<std::string, Route> >::iterator it = routes.begin(); it != routes.end() && i < 2; it++)
	{
		std::map<std::string, Route> maptmp = *it;
		std::string key;
		for (std::map<std::string, Route>::iterator it2 = maptmp.begin(); it2 != maptmp.end(); it2++)
		{
			if (it2->first == "NEXT" && it2->second._path == "true")
			{
				i++;
				break;
			}
			key = it2->first;
			key = removespace(key);
			serverarray[i]._route[key] = it2->second;
		}
	}
	i = 0;
	// while (i < configs.size())
	// {
	// 	std::cout << "PORT: "<< serverarray[i]._port << std::endl;
	// 	std::cout << "SERVERNAME: "<< serverarray[i]._servername << std::endl;
	// 	std::cout << "MAXBODYSIZE: " << serverarray[i]._maxbodysize << std::endl;
	// 	std::cout << "---------------------ROUTE FOR SERVER " << i << "-----------------------" << std::endl;
	// 	for ( std::map<std::string, Route>::iterator it = serverarray[i]._route.begin(); it != serverarray[i]._route.end(); it++)
	// 	{
	// 		std::cout << it->first << " cgi = " << it->second._cgi << std::endl;
	// 		std::cout << it->first << " index = " << it->second._index << std::endl;
	// 		std::cout << it->first << " listing = " << it->second._listing << std::endl;
	// 		std::cout << it->first << " methods = " << it->second._methods << std::endl;
	// 		std::cout << it->first << " path = " << it->second._path << std::endl;
	// 		std::cout << it->first << " root = " << it->second._root << std::endl;
	// 	}
	// 	std::cout << "-------------------------------------------------------------------------" << std::endl;
	// 	i++;
	// }
}


std::vector<std::string> ServerConfig::splitserv(std::string content)
{
	std::string res;
	std::vector<std::string> vector;
	int servstart;
	//int servend;

	while (content.size() > 0)
	{
		if (content.find("@")  == std::string::npos)
		{
			content.clear();
			return vector;
		}
		servstart = content.find("@");
		if (content.substr(servstart, servstart + 7) != "@server[\n")
		{
			std::cerr << "Bad syntax, example: @server[...]" << std::endl;
			exit(1);
		}
		content = content.substr(servstart + 9, content.size());
		res = content.substr(0, content.find_first_of(']'));
		content = content.substr(content.find_first_of("]") + 1, content.size());
		vector.push_back(res);
		if (content.find('\n') != 0)
		{
			std::cerr << "Bad syntax, missing newline after server" << std::endl;
			exit(1);
		}
		while(content[0] == '\n')
			content.erase(0, 1);
		//std::cout << content << std::endl;
		//std::cout << res << std::endl;
	}
	return vector;
}

std::vector<std::map<std::string, std::string> > ServerConfig::setupmap(std::vector<std::string> configs)
{
	std::vector<std::map<std::string, std::string> > res;
	std::map<std::string, std::string> maptmp;
	std::string tmp;
	size_t pos;
	std::string i = "1";
	for (std::vector<std::string>::iterator it = configs.begin(); it != configs.end(); it++)
	{
		pos = it->find("- server_name =");
		if (pos == std::string::npos)
		{
			std::cerr << "Config file: server name not found, default name given" << std::endl;
			tmp  = "default " + i;
			std::cout << "default:" << tmp << std::endl;
			maptmp["server_name"] = tmp;
			i[0]++;
		}
		else
		{
			tmp = it->substr(pos + 16, it->find(";") - 2);
			if (tmp.find_first_of('\n') != tmp.find_last_of("\n") || tmp.find_first_of(";") != tmp.find_last_of(";"))
			{
				std::cerr << "Config file: server_name: bad syntax" << std::endl;
				exit(1);
			}
			tmp.erase(tmp.find("\n"));
			tmp.erase(tmp.find(";"));
			if (checkdouble(tmp, "server_name", res) != 0)
			{
				std::cerr << "Config file: two server cannot have the same name" << std::endl;
				exit(1);
			}
			maptmp["server_name"] = tmp;
		}
		pos = it->find("- listen =");
		if (pos == std::string::npos)
		{
			std::cerr << "Config file: listening port not found, syntax is: - listen = [...];" << std::endl;
			exit(1);
		}
		tmp = it->substr(pos + 11, it->find(";"));
		if (tmp.find_first_of('\n') != tmp.find_last_of("\n") || tmp.find_first_of(";") != tmp.find_last_of(";"))
		{
			std::cerr << "Config file: port: bad syntax" << std::endl;
			exit(1);
		}
		tmp.erase(tmp.find("\n"));
		tmp.erase(tmp.find(";"));
		if (checkdouble(tmp, "port", res) || atoi(tmp.c_str()) == 0)
		{
			std::cerr << "Config file: two server cannot listen on the same port" << std::endl;
			exit(1);
		}
		maptmp["port"] = tmp;
		pos = it->find("- clientbody =");
		if (pos == std::string::npos)
		{
			std::cerr << "Config file: client max body size not found, syntax is: - clientbody = [...];" << std::endl;
			exit(1);
		}
		tmp = it->substr(pos + 14, it->find(";"));
		tmp.erase(tmp.find("\n"));
		tmp.erase(tmp.find(";"));
		maptmp["clientbody"] = tmp;
		//std::cout << "tmp = " << tmp << std::endl;
		res.push_back(maptmp);
		maptmp.clear();
	}
	return res;
}

std::vector<std::map<std::string, Route> > ServerConfig::setuproutes(std::vector<std::string> configs)
{
	size_t pos = 0;
	std::string tmp;
	std::string tmp2;
	std::string route;
	std::map<std::string, Route> maptmp;
	std::vector<std::map<std::string, Route> > res;
	for (std::vector<std::string>::iterator it = configs.begin(); it != configs.end(); it++)
	{
		pos = it->find("- route{");
		if (pos == std::string::npos)
		{
			std::cerr << "Config file: route not found" << std::endl;
			exit(1);
		}
		//std::cout << "it = " << *it << std::endl;
		tmp = it->substr(pos + 8, it->find_first_of("}"));
		while (tmp.size() != 0)
		{
			if (tmp.find("-") == std::string::npos || tmp.find("(") == std::string::npos)
			{
				std::cerr << "Config file: bad route syntax" << std::endl;
				exit(1);
			}
			tmp2 = tmp.substr(tmp.find("-") + 1, tmp.find("("));
			tmp2 = tmp2.substr(0, tmp2.find("("));
			maptmp[tmp2]._path = tmp2;
			route = tmp.substr(0, tmp.find_first_of(")"));
			if (route.find("methods =") == std::string::npos)
			{
				std::cerr << "Config file: route methods not found" << std::endl;
				exit(1);
			}
			maptmp[tmp2]._methods = route.substr(route.find("methods =") + 9, route.find(";", route.find("methods =")) - route.find("methods ="));
			maptmp[tmp2]._methods.erase(maptmp[tmp2]._methods.find(";"));
			if (route.find("root =") == std::string::npos)
			{
				std::cerr << "Config file: route root not found" << std::endl;
				exit(1);
			}
			maptmp[tmp2]._root = route.substr(route.find("root =") + 6, route.find(";", route.find("root =")) - route.find("root ="));
			maptmp[tmp2]._root.erase(maptmp[tmp2]._root.find(";"));
			if (route.find("listing = false;") == std::string::npos && route.find("listing = true;") == std::string::npos)
			{
				std::cerr << "Config file: listing not found" << std::endl;
				exit(1);
			}
			if (route.find("listing = true;") != std::string::npos)
				maptmp[tmp2]._listing = true;
			else
				maptmp[tmp2]._listing = false;
			if (route.find("index =") == std::string::npos)
			{
				std::cerr << "Config file: index not found" << std::endl;
				exit(1);
			}
			maptmp[tmp2]._index = route.substr(route.find("index =") + 7, route.find(";", route.find("index =")) - route.find("index ="));
			maptmp[tmp2]._index.erase(maptmp[tmp2]._index.find(";"));
			if (route.find("cgi =") != std::string::npos)
			{
				//std::cerr << "Config file: cgi not found" << std::endl;
				maptmp[tmp2]._cgi = route.substr(route.find("cgi =") + 5, route.find(";", route.find("cgi =") - route.find("cgi =")));
				maptmp[tmp2]._cgi.erase(maptmp[tmp2]._cgi.find(";"));
				//exit(1);
			}
			// RAJOUTER LES METHODE DANS HTTPMETHODE DE LA CLASSE ROUTE
			// std::cout << maptmp[tmp2]._methods << std::endl;
			// std::cout << maptmp[tmp2]._root << std::endl;
			// std::cout << maptmp[tmp2]._index << std::endl;
			// std::cout << maptmp[tmp2]._listing << std::endl;
			// std::cout << maptmp[tmp2]._cgi << std::endl;
			// std::cout << "---------------------------" << std::endl;

			tmp.erase(0, tmp.find_first_of(")") + 1);
			res.push_back(maptmp);
			maptmp.clear();
			if (tmp.find_first_of(")") == std::string::npos)
			{
				tmp.clear();
				break;
			}
		}
		maptmp["NEXT"]._path = "true";
		res.push_back(maptmp);
		maptmp.clear();
	}
	return res;
}

int ServerConfig::checkdouble(std::string str, std::string tosearch, std::vector<std::map<std::string, std::string> > configs)
{
	for (std::vector<std::map<std::string, std::string> >::iterator it = configs.begin(); it != configs.end(); it++)
	{
		if (it->at(tosearch) == str)
			return 1;
	}
	return 0;
}
// static void openfile_error(std::ifstream &file, const std::string &filename)
// {
// 	file.open(filename.c_str());
// 	if (!file.good() || !file.is_open())
// 	{
// 		std::cerr << "Cannot open file " << filename << std::endl;
// 		exit(1);
// 	}
// }

// static void trim_left(std::string &str)
// {
// 	str = str.erase(str.find_last_not_of(" \n") + 1);
// }

// static bool iswhitespace(std::string str)
// {
// 	return str.find_first_not_of(" \n") == std::string::npos;
// }

// static std::string getcontentnocomm(std::ifstream file)
// {
// 	std::string line;
// 	std::string content;

// 	while (std::getline(file, line))
// 	{
// 		size_t comm_pos = line.find("$");
// 		if (comm_pos != std::string::npos)
// 		{
// 			line = line.substr(0, comm_pos);
// 			trim_left(line);
// 		}
// 		if (!line.empty() || iswhitespace(line))
// 			continue;
// 		content += line + '\n';
// 	}
// 	return content;
// }

// static void checkinvalidchar(std::string content)
// {
// 	if (content.find_first_of("'\"\t\r") != std::string::npos)
// 	{
// 		std::cerr << "Invalid character found in file" << std::endl;
// 		exit(1);
// 	}
// }

// static void checkinvalidkeyword(std::string content)
// {
// 	std::string keyword[] = {
// 			"server",
// 			"servers",
// 			"port",
// 			"error_pages",
// 			"server_name",
// 			"methods",
// 			"root",
// 			"routes",
// 			"index",
// 			"redirection",
// 			"repertory_listing",
// 			"cgi",
// 			"upload",
// 			"client_max_body_size",
// 	};

// 	std::istringstream iss(content);
// 	for (std::string line; std::getline(iss, line);)
// 	{
// 		size_t start = line.find_first_not_of(" \n");
// 		if (start % 2 != 0)
// 		{
// 			std::cerr << "Config file: bad indentation" << std::endl;
// 			exit(1);
// 		}
// 		std::vector<std::string> splitted = split(line.substr(start), ':');
// 		if (splitted.empty())
// 		{
// 			std::cerr << "invalid config file" << std::endl;
// 			exit(1);
// 		}
// 		if (splitted.size() > 2)
// 		{
// 			std::cerr << "invalid config file, too many ':'" << std::endl;
// 			exit(1);
// 		}
// 		bool found = false;
// 		for(std::size_t i = 0; i < sizeof(keyword) / sizeof(keyword[0]); ++i)
// 		{
// 			if (splitted[0].substr(0, 2) == "- " || splitted[0] == keyword[i])
// 			{
// 				found = true;
// 				break;
// 			}
// 		}
// 		if (!found)
// 		{
// 			std::cerr << "Config file: unknown keyword " << splitted[0] << std::endl;
// 			exit(1);
// 		}
// 		if ("- " != splitted[0].substr(0, 2) && splitted[0].find_first_of(" \n") != std::string::npos)
// 		{
// 			std::cerr << "Config file: need space after ':' or '-'" << std::endl;
// 			exit(1);
// 		}
// 		if (splitted.size() == 2)
// 		{
// 			if (splitted[1][0] != ' ')
// 			{
// 				std::cerr << "Config file: need space after ':'" << std::endl;
// 				exit(1);
// 			}
// 			if (splitted[1][0] == ' ')
// 			{
// 				std::cerr << "Config file: need only one space after ':'" << std::endl;
// 				exit(1);
// 			}
// 		}
// 	}
// }

// static std::size_t find_next_non_whitespace_line_start(const std::string &str)
// {
// 	std::size_t pos = 0;

// 	while (true)
// 	{
// 		if (str[pos] != ' ' && str[pos] != '\n')
// 			return pos;
// 		pos = str.substr(pos).find('\n') + pos;
// 		if (pos == std::string::npos)
// 			return std::string::npos;
// 		pos++;
// 	}
// }

// void Config::parse_servers(const std::string &path, std::vector<Config> &configs)
// {
// 	/// Check file extension and path validity
// 	if (path.empty())
// 		throw std::runtime_error("Empty config file path");
// 	std::size_t ext_pos = path.find(".yaml");
// 	if (ext_pos == std::string::npos)
// 		throw std::runtime_error("Invalid config file extension");
// 	if (ext_pos != path.size() - 5)
// 		throw std::runtime_error("Invalid config file extension");

// 	/// Get the file content without comments
// 	std::ifstream file;
// 	open_file_or_throw(file, path);
// 	std::string file_content = get_file_content_without_comments(file);
// 	file.close();

// 	/// Check file validity
// 	if (file_content.empty())
// 		throw std::runtime_error("Empty config file");
// 	throw_if_any_invalid_char(file_content);
// 	throw_if_invalid_keyword(file_content);
// 	throw_if_invalid_tiret(file_content);

// 	/// Check if the file starts with "servers"
// 	if ("servers" != file_content.substr(0, 7))
// 		throw std::runtime_error("Invalid config file, missing 'servers' keyword at the beginning");
// 	file_content = file_content.erase(0, file_content.find_first_of('\n') + 1);

// 	/// Split the file content into servers
// 	std::vector<std::string> servers;
// 	file_content = unpad_from_left(file_content, 2, true);
// 	file_content = unpad_from_left(file_content, 2, false); // don't check because tiret
// 	while (true)
// 	{
// 		if (file_content.empty())
// 			break;
// 		std::string first_line = file_content.substr(0, file_content.find_first_of('\n'));
// 		std::vector<std::string> splitted = split(first_line, ':');
// 		if ("server" != splitted[0])
// 			throw std::runtime_error("Invalid config file, '" + splitted[0] + "' is not a valid keyword");
// 		if (splitted.size() != 1)
// 			throw std::runtime_error("Invalid config file, '" + splitted[0] + "' keyword must be alone on its line");
// 		file_content.erase(0, file_content.find_first_of('\n') + 1);
// 		std::size_t pos = find_next_non_whitespace_line_start(file_content);
// 		std::string server_content = file_content.substr(0, pos);
// 		server_content = unpad_from_left(server_content, 2, true);
// 		servers.push_back(server_content);
// 		file_content = file_content.erase(0, pos);
// 	}

// 	/// Parse each server
// 	std::set<int> ports;
// 	for (std::vector<std::string>::iterator it = servers.begin(); it != servers.end(); ++it)
// 	{
// 		Config config;
// 		parse(*it, config);
// 		if (ports.count(config.port) != 0)
// 			throw std::runtime_error("Invalid config file, two servers have the same port");
// 		ports.insert(config.port);
// 		configs.push_back(config);
// 	}
// }

// static Option<std::string> get_list_content(std::string &str, const std::string &key)
// {
// 	std::string lines;
// 	bool found = false;
// 	std::istringstream iss(str);
// 	for (std::string line; std::getline(iss, line);)
// 	{
// 		if (found)
// 		{
// 			if (line.substr(0, 2) == "  ")
// 			{
// 				str.erase(str.find(line), line.size() + 1);
// 				lines += line + "\n";
// 			}
// 			else
// 				break;
// 		}
// 		else if (line.substr(0, key.size()) == key)
// 		{
// 			found = true;
// 			str.erase(str.find(line), line.size() + 1);
// 		}
// 	}
// 	if (!found)
// 		return Option<std::string>::None();
// 	lines = unpad_from_left(lines, 2, true);
// 	return Option<std::string>::Some(lines);
// }


// static void replacePWD(std::string &str)
// {
// 	std::string pwd = get_cwd();
// 	std::string::size_type pos = 0;
// 	while ((pos = str.find("PWD", pos)) != std::string::npos)
// 	{
// 		str.replace(pos, 3, pwd);
// 		pos += pwd.size();
// 	}
// }

// static std::map<int, std::string> parse_error_pages(std::string &str)
// {
// 	std::map<int, std::string> map;
// 	Option<std::string> m_lines = get_list_content(str, "error_pages");
// 	if (m_lines.isNone())
// 		return map;
// 	std::string lines = m_lines.unwrap();
// 	std::vector<std::string> error_pages_lines = split(lines, '\n');
// 	for (std::vector<std::string>::iterator it = error_pages_lines.begin(); it != error_pages_lines.end(); ++it)
// 	{
// 		if ("- " != it->substr(0, 2))
// 			throw std::runtime_error("Invalid config file, an error_page must be in the form '- <error_code> <path>'");
// 		std::string line = it->substr(2);
// 		std::vector<std::string> words = split(line, ':');
// 		if (words.size() != 2)
// 			throw std::runtime_error("Invalid config file, an error_page must be in the form '- <error_code> <path>'");
// 		if (words[0].size() != 3)
// 			throw std::runtime_error("Invalid config file, an error_code must be 3 digits long");
// 		if (words[1][0] != ' ')
// 			throw std::runtime_error("Invalid config file, an error_page must be in the form '- <error_code> <path>'");
// 		map[std::atoi(words[0].c_str())] = words[1].substr(1);
// 	}
// 	for (std::map<int, std::string>::iterator it = map.begin(); it != map.end(); ++it)
// 		replacePWD(it->second);
// 	return map;
// }

// static void parse_routes(std::string &str, std::map<std::string, Route> &map)
// {
// 	Option<std::string> m_lines = get_list_content(str, "routes");
// 	if (m_lines.isNone())
// 		return;
// 	std::string lines = m_lines.unwrap();
// 	std::istringstream iss(lines);
// 	bool is_in_new_route;
// 	bool is_first = true;
// 	std::string route_path;
// 	std::string route_infos;

// 	for (std::string line; std::getline(iss, line);)
// 	{
// 		is_in_new_route = line.substr(0, 2) == "- ";
// 		if (is_first && !is_in_new_route)
// 			throw std::runtime_error("Invalid config file, a route must start with '- '");
// 		if (!is_first && is_in_new_route)
// 		{
// 			if (!map.count(route_path))
// 			{
// 				map.insert(std::pair<std::string, Route>(route_path, Route(route_infos, route_path)));
// 				route_infos.clear();
// 			}
// 		}
// 		if (is_in_new_route)
// 		{
// 			is_first = false;
// 			std::string route_line = line.substr(2);
// 			std::size_t pos = route_line.find(':');
// 			if (pos == std::string::npos)
// 				throw std::runtime_error("Invalid config file, missing ':'");
// 			if (route_line.size() != pos + 1)
// 				throw std::runtime_error(
// 						"Invalid config file, there should be no character after ':' in a list of routes");
// 			route_path = route_line.substr(0, pos);
// 		}
// 		else
// 		{
// 			route_infos += line + "\n";
// 		}
// 	}
// 	if (!map.count(route_path))
// 		map.insert(std::pair<std::string, Route>(route_path, Route(route_infos, route_path)));

// 	for (std::map<std::string, Route>::iterator it = map.begin(); it != map.end(); ++it)
// 	{
// 		replacePWD(it->second.root);
// 		if (it->second.index.isSome())
// 			replacePWD(it->second.index.unwrap());
// 		if (it->second.cgi.isSome())
// 			replacePWD(it->second.cgi.unwrap().cgi_path);
// 	}

// 	for (std::map<std::string, Route>::iterator it = map.begin(); it != map.end(); ++it)
// 	{
// 		if (it->second.repertory_listing && it->second.cgi.isSome())
// 			throw std::runtime_error("Invalid config file, a route cannot have both repertory_listing and cgi");
// 		if (it->second.repertory_listing && it->second.index.isSome())
// 			throw std::runtime_error("Invalid config file, a route cannot have both repertory_listing and index");
// 		char last_char = *(it->first.end() - 1);
// 		if (last_char != '/')
// 			throw std::runtime_error(
// 					"Invalid config file, a route with repertory_listing must end with '/' (" + it->first + ")");
// 	}
// }

// void ServerConfig::parse(std::string server_config, ServerConfig config)
// {
// 	{
		
// 	}
// }