#include "Config.hpp"

Config::Config(const char *config_path) {
	this->config_path = config_path;
}

Config::~Config() {
}

void Config::parseConfig() {
	std::ifstream configFile;
	configFile.open(config_path);
	if (!configFile) {
		throw new std::runtime_error("File " + config_path + " not found");
	}
	std::vector<std::string> lines;

	while (!configFile.eof()) {
		std::string currLine;
		getline(configFile, currLine);
		if (!skipSpaces(currLine).empty()) {
			lines.push_back(currLine);
		}
	}

	for (unsigned long i = 0; i < lines.size(); i++) {
		std::vector<std::string> tokens = split(lines[i], " ");
		for (unsigned long i1 = 0; i1 < tokens.size(); i1++) {
			tokens[i1] = trim(tokens[i1]);
		}
		uint32_t endBlockPos = i + 1;
		this->_servers.push_back(Server());
		parseServer(lines, &endBlockPos);
		i = endBlockPos;
	}
}

void Config::parseServer(std::vector<std::string> lines, uint32_t* endBlockPos) {
	while (*endBlockPos < lines.size()) {
		std::vector<std::string> lineItems = split(lines[*endBlockPos], " ");
		if (lineItems[0] == "location") {
			this->_servers.back().getLocations().push_back(Location());
			parseLocation(lines, endBlockPos);
		}
		else if (lineItems[0] == "}") {
			break;
		}
		else {
			checkServerInstructions(lineItems);
		}
		(*endBlockPos)++;
	}
}

void Config::checkServerInstructions(std::vector<std::string> line) {

	if (line[0] == "server_name") {
		this->_servers.back().setServerName(line[1]);
	} else if (line[0] == "port") {
		this->_servers.back().setPortInt(atoi(line[1].c_str()));
		this->_servers.back().setPort(line[1]);
	} else if (line[0] == "host") {
		this->_servers.back().setHost(line[1]);
	} else if (line[0] == "default_error_page") {
		this->_servers.back().getDefaultPages()[atoi(line[1].c_str())] = line[2];
	} else if (line[0] == "limit_size") {
		this->_servers.back().setMaxBodySize(atoi(line[1].c_str()));
	} else if (line[0] == "root") {
		this->_servers.back().setRoot(line[1]);
	}
}

void Config::parseLocation(std::vector<std::string> lines, uint32_t* endBlockPos) {
	std::vector<std::string> locationLine = split(lines[*endBlockPos], " ");
	this->_servers.back().getLocations().back().setPath(locationLine[1]);
	(*endBlockPos)++;
	while (*endBlockPos < lines.size()) {
		std::vector<std::string> lineItems = split(lines[*endBlockPos], " ");

		if (lineItems[0] == "}") {
			break ;
		}
		else {
			checkLocationInstructions(lineItems, *endBlockPos);
		}
		(*endBlockPos)++;
	}
}

void Config::checkLocationInstructions(std::vector<std::string> line, uint32_t endBlockPos) {

	endBlockPos = 0;

	if (line[0] == "root") {
		this->_servers.back().getLocations().back().setRoot(line[1]);
	} else if (line[0] == "allow_method") {
		size_t size = line.size();
		for (unsigned long i = 1; i < size; i++) {
			this->_servers.back().getLocations().back().getAllowedMethods().push_back(line[i]);
		}
	} else if (line[0] == "cgi") {
		for (unsigned long i = 1; i < line.size(); i++) {
			this->_servers.back().getLocations().back().getCgi().push_back(line[i]);
		}
	} else if (line[0] == "cgi_path") {
		this->_servers.back().getLocations().back().setCgiPath(line[1]);
	} else if (line[0] == "autoindex") {
		this->_servers.back().getLocations().back().setAutoIndex(line[1] == "on");
	} else if (line[0] == "index") {
		for (unsigned long i = 1; i < line.size(); i++) {
			this->_servers.back().getLocations().back().getIndex().push_back(line[i]);
		}
	} else if (line[0] == "redirect") {
		this->_servers.back().getLocations().back().getRedirects()[atoi(line[1].c_str())] = line[2];
	}
}