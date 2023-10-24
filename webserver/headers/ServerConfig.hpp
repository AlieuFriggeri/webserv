#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "../headers/Socket.hpp"
#include <map>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>

class ServerConfig{
	public:

	static std::map<std::string, std::map<std::string, std::string> > parsefile(std::string filename);
	static void erasewhitespaceleft(std::string str);
	//void configservers(std::string file);
	static std::vector<std::string> splitserv(std::string content);
	static std::map<std::string, std::map<std::string, std::string> > setupmap(std::vector<std::string> configs);
	~ServerConfig();
	ServerConfig();



	private:

	std::map<std::string, std::string> _configmap;
};



#endif