//
// Created by Phylicia Melinda on 7/19/21.
//

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <map>
#include <vector>

class Location {
private:
	// Coplien form
	Location& operator=(const Location& rhs);

public:
	std::string _root;
	std::vector<std::string> _allowed_methods;
	std::vector<std::string> _cgi;
	std::string _cgi_path;
	bool _autoindex;
	std::vector<std::string> _index;
	std::string _path;
	std::map<int, std::string> _redirects;
	Location();
	void setPath(const std::string& path);
	void setRoot(const std::string& root);
	std::vector<std::string>& getAllowedMethods();
	std::vector<std::string>& getCgi();
	void setCgiPath(const std::string& path);
	void setAutoIndex(bool autoindex);
	std::vector<std::string>& getIndex();
	std::map<int, std::string>& getRedirects();
};


#endif //OURWEBSERV_LOCATION_HPP
