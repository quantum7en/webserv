#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <dirent.h>
#include "Server.hpp"
#include <sstream>
#include <fstream>
#include <sys/stat.h>

class Request;

class Response
{
    private:
        bool 						_AUTOINDEX;
		std::string                 _zapros;
        std::string                 _answer;
        std::string                 _answer_body;
        std::string                 _content_type;
        std::string                 _status;
        int                         _code;
        std::map<int, std::string>	_errors;
        std::string                 _file_path;
        std::string                 _root;
        std::map<std::string, std::map<int, std::string> >		_default_errors;
        void                        find_root(Request &zapros);
    public:
	    std::map<std::string, std::string>						_hosts_and_root;
        void                        fill_hosts_and_root(std::vector<Server>& servers);
        void                        choose_method(Request &zapros, std::vector<Server>& servers);
        void                        make_get_response(Request zapros, std::vector<Server>& servers);
        void                        make_delete_response(Request &zapros, std::vector<Server>& servers);
        void                        make_headers(Request &zapros);
        void                        make_post_response(Request & zapros, std::vector<Server>& servers);
        void                        resetValues(Request & zapros);
        void                        setValues(Request &zapros);
        void                        setAnswer(const std::string  &answer) {this->_answer = answer;}
        std::string                 getAnswer() const {return this->_answer;}
        std::string		            getStatus(int code);
        std::string		            getFilePath() const {return this->_file_path;}
        void                        ErrorsValue();
        void 						autoindexOn(Request &zapros);
        std::string					error_404(std::string const &key);
		std::string					error_400(std::string const &key);
		std::string					error_403(std::string const &key);
		std::string					error_405(std::string const &key);
		void        				error_413();
        void                        check_location(Request zapros, std::vector<Server>& servers);
        Response() {}
        ~Response() {}
};

#endif