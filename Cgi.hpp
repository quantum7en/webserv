#ifndef CGI_HPP
#define CGI_HPP
#include "main.hpp"

#define CGI_BUFSIZE 65536

class Cgi 
{
    private:
        std::string _answer;
        std::string _answer_body;
        std::string _pathToHandler;
        int         _extentionOfScript;
        char        **_env;
        char        _buf[CGI_BUFSIZE];
    public:
        int         execute_cgi(Request & zapros, std::string& root);
        int         write_to_buf(int fd);
        void        parseCGI(Request & zapros);
        void        create2darray(Request zapros);
        void        define_cgi_path(std::string& file_path);
        void        free_memory();
        Cgi() {}
        ~Cgi() {}

};

#endif
