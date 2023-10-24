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

void ServerConfig::parsefile(std::string filename)
{
	std::ifstream file;
	std::string content;
	std::string line;
	file.open(filename);

	if (!file.is_open())
	{
		std::cerr << "cannot open file " << filename << std::endl;
		exit(1);
	}
	if (filename.find(".genial"))

	while (std::getline(file, line))
	{
		size_t comm_pos = line.find("$");
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
	if (content.substr(0, 8) != "servers:")
	{
		std::cerr << "invalid config file: servers keyword missing on first line" << std::endl;
		exit(1);
	}
	content = content.erase(0, 8);

	//std::cout << content << std::endl;

}

void ServerConfig::configservers(std::string content)
{
	std::list<std::string> configs;

	while (!content.empty())
	{
		configs.push_back(splitserv(content));
	}
}


std::string ServerConfig::splitserv(std::string content)
{
	std::string res;
	int servpos;

	servpos = content.find('%');
}

static void openfile_error(std::ifstream &file, const std::string &filename)
{
	file.open(filename.c_str());
	if (!file.good() || !file.is_open())
	{
		std::cerr << "Cannot open file " << filename << std::endl;
		exit(1);
	}
}

static void trim_left(std::string &str)
{
	str = str.erase(str.find_last_not_of(" \n") + 1);
}

static bool iswhitespace(std::string str)
{
	return str.find_first_not_of(" \n") == std::string::npos;
}

static std::string getcontentnocomm(std::ifstream file)
{
	std::string line;
	std::string content;

	while (std::getline(file, line))
	{
		size_t comm_pos = line.find("$");
		if (comm_pos != std::string::npos)
		{
			line = line.substr(0, comm_pos);
			trim_left(line);
		}
		if (!line.empty() || iswhitespace(line))
			continue;
		content += line + '\n';
	}
	return content;
}

static void checkinvalidchar(std::string content)
{
	if (content.find_first_of("'\"\t\r") != std::string::npos)
	{
		std::cerr << "Invalid character found in file" << std::endl;
		exit(1);
	}
}

static void checkinvalidkeyword(std::string content)
{
	std::string keyword[] = {
			"server",
			"servers",
			"port",
			"error_pages",
			"server_name",
			"methods",
			"root",
			"routes",
			"index",
			"redirection",
			"repertory_listing",
			"cgi",
			"upload",
			"client_max_body_size",
	};

	std::istringstream iss(content);
	for (std::string line; std::getline(iss, line);)
	{
		size_t start = line.find_first_not_of(" \n");
		if (start % 2 != 0)
		{
			std::cerr << "Config file: bad indentation" << std::endl;
			exit(1);
		}
		std::vector<std::string> splitted = split(line.substr(start), ':');
		if (splitted.empty())
		{
			std::cerr << "invalid config file" << std::endl;
			exit(1);
		}
		if (splitted.size() > 2)
		{
			std::cerr << "invalid config file, too many ':'" << std::endl;
			exit(1);
		}
		bool found = false;
		for(std::size_t i = 0; i < sizeof(keyword) / sizeof(keyword[0]); ++i)
		{
			if (splitted[0].substr(0, 2) == "- " || splitted[0] == keyword[i])
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			std::cerr << "Config file: unknown keyword " << splitted[0] << std::endl;
			exit(1);
		}
		if ("- " != splitted[0].substr(0, 2) && splitted[0].find_first_of(" \n") != std::string::npos)
		{
			std::cerr << "Config file: need space after ':' or '-'" << std::endl;
			exit(1);
		}
		if (splitted.size() == 2)
		{
			if (splitted[1][0] != ' ')
			{
				std::cerr << "Config file: need space after ':'" << std::endl;
				exit(1);
			}
			if (splitted[1][0] == ' ')
			{
				std::cerr << "Config file: need only one space after ':'" << std::endl;
				exit(1);
			}
		}
	}
}

