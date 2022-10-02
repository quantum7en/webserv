//
// Created by Phylicia Melinda on 7/19/21.
//

#ifndef OURWEBSERV_SERVER_HPP
#define OURWEBSERV_SERVER_HPP

#include "Location.hpp"
#include <string>
#include <map>
#include <vector>

class Location;


class Server {

public:
	std::string _host;  //ip
	// int 		_hostInt; //ip
	std::string _port;
	int			_portInt;
	std::string _path;
	std::string _server_name;
	size_t		_max_body_size;
	std::string _root;
	std::map<int, std::string> _default_error_page;
	std::map<std::string, Location> _location_config;
	std::vector<Location> _locations;
	int 		_socketFd;
	Server();
	virtual ~Server();
    Server& operator=(const Server& rhs);

	// Server Getters
	const std::string& getHost() const;
//	const int getHostInt() const;
	const std::string& getPort() const;
	const std::string& getRoot() const;
	int getPortInt() const;
	const std::string& getPath() const;
//	int getIndex() const;
	std::map<int, std::string>& getDefaultPages();
	std::vector<Location>& getLocations();


	int getSocketFd() const;
	void setSocketFd(int fd);
	void setServerName(const std::string& name);
	void setPortInt(int port);
    void setPort(const std::string& port);
	void setHost(const std::string& host);
	void setMaxBodySize(size_t size);
	void setRoot(const std::string& root);
};


#endif //OURWEBSERV_SERVER_HPP
