#ifndef ROUTE_HPP
#define ROUTE_HPP
#include "ServerConfig.hpp"


class Route{
public:
	Route();
	~Route();

	std::string _methods;
	std::string _root;
	bool _listing;
	std::string _index;
	std::string _cgi;
private:

};

#endif