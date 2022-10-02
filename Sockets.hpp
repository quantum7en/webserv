#ifndef WEBSERV_SOCKETS_HPP
#define WEBSERV_SOCKETS_HPP
#include <sys/select.h>
#include <netdb.h>
#include <set>
#include "Server.hpp"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

class Sockets
{

    public:
        Sockets(std::vector<Server> &servers);
        int listen_all(fd_set &master);
        std::map<int, Server> listen_sockets;
        std::map<int, Server> connection_sockets;
        int accept_connection(int i, fd_set &master, int &fdmax);
        void    remove_connection(int socket);
    private:
        struct addrinfo _hints;
        void    fill_hints();
        void    close_all();
        void    listen_socket_setup(Server &server);
        void    reuse_address(int listen_sock, struct addrinfo *servinfo);
        void    reuse_port(int listen_sock, struct addrinfo *servinfo);
};

#endif //WEBSERV_SOCKETS_HPP
