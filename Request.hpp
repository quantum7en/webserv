#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>
#include "Server.hpp"
#include <dirent.h>
#include "Response.hpp"

class Request
{
    private:
        std::string                         _buf;
        std::string                         _method;
        std::string	                        _resource_name;
        std::string	                        _protocol_version;
        std::string	                        _body;
        std::string                         _content_type;
        std::string                         _content_length;
        std::string                         _status;
        std::string	                        _answer_body;
        size_t                              _len;
        int                                 _sl;
        int                                 _hd;
        int                                 _bd;
        int                                 _bd_flag;
        std::map<std::string, std::string>  _headers;
        std::string                         _location;
        Server                              _current_server;
    public:
        void        						parse_start_line();
        int         						parse_request(std::string str, Response & resp);
        void        						parse_headers();
        void        						parse_body();
        void        						cut_buf(int flag);
        void        						clean_request();
        void                                setStatus(const std::string  &status) {this->_status = status;}
        void                                setLocation(const std::string  &location) {this->_location = location;}
        void                                setAnswerBody(const std::string  &answer_body) {this->_answer_body = answer_body;}
        void                                setContentType(const std::string  &content_type) {this->_content_type = content_type;}
        std::string			                getResourseName() const {return this->_resource_name;}
        std::string			                getMethod() const {return this->_method;}
        std::string			                getContentType() const {return this->_content_type;}
        std::string			                getContentLength() const {return this->_content_length;}
        std::string			                getStatus() const {return this->_status;}
        std::string			                getLocation() const {return this->_location;}
        std::string			                getAnswerBody() const {return this->_answer_body;}
        std::map<std::string, std::string>  getHeaders() const {return this->_headers;}
        std::string                         getHeaderContentLength() const;
        std::string                         getHeaderContentType() const;
        std::string                         getHost() const {return _headers.find("Host")->second;}
        const std::string&                  getBody() const {return _body;}
        const Server&                       getCurrentServer() const {return _current_server; }
        int                                 check_request(Response & resp);
        Request(Server &server)
        {
            _answer_body = "";
            _current_server = server;
        }
        ~Request() {}
};

#endif