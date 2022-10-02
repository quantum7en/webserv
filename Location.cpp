//
// Created by Phylicia Melinda on 7/19/21.
//

#include "Location.hpp"

Location::Location() {
}

void Location::setPath(const std::string& path) {
	this->_path = path;
}

void Location::setRoot(const std::string& root) {
	this->_root = root;
}

std::vector<std::string>& Location::getAllowedMethods() {
	return this->_allowed_methods;
}

std::vector<std::string>& Location::getCgi() {
	return this->_cgi;
}

void Location::setCgiPath(const std::string& path) {
	this->_cgi_path = path;
}

void Location::setAutoIndex(bool autoindex) {
	this->_autoindex = autoindex;
}

std::vector<std::string>& Location::getIndex() {
	return this->_index;
}

std::map<int, std::string>& Location::getRedirects() {
	return this->_redirects;
}