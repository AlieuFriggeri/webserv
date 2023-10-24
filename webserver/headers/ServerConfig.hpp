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

	void parsefile(std::string filename);
	void erasewhitespaceleft(std::string str);
	void configservers(std::string file);
	std::string splitserv(std::string content);
	~ServerConfig();
	ServerConfig();



	private:

	std::map<std::string, std::string> _configmap;
};



#endif