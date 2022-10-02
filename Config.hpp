//
// Created by Phylicia Melinda on 7/9/21.
//

#ifndef WEBSERV_CONFIG_HPP
#define WEBSERV_CONFIG_HPP

#include "Server.hpp"
#include "Location.hpp"
#include "Utils.hpp"

#include <vector>
#include <iostream>
#include <fstream>

class Server;

class Location;

class Config {
private:
	// Coplien
	Config();
	Config (const Config& other);
	Config& operator=(const Config& rhs);

	std::vector<Server> _servers;
	std::string config_path;

	void parseServer(std::vector<std::string> lines, uint32_t* endBlockPos);
	void parseLocation(std::vector<std::string> lines, uint32_t* endBlockPos);

	void checkServerInstructions(std::vector<std::string> line);
	void checkLocationInstructions(std::vector<std::string> line, uint32_t endBlockPos);
public:
	Config(const char *config_path);
	virtual ~Config();

//	ConfigErrors configErrors;

	void parseConfig(void);
	std::vector<Server>& getServers() { return _servers; }   

};


#endif //WEBSERV_CONFIG_HPP
