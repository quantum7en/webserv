#include "Sockets.hpp"

Sockets::Sockets(std::vector<Server> &servers)
{
    fill_hints();
    std::vector<Server>::iterator it = servers.begin();
    while (it != servers.end()){
        listen_socket_setup(*it);
        it++;
    }
}

int Sockets::listen_all(fd_set &master){
    std::map<int, Server>::iterator it = listen_sockets.begin();
    while (it != listen_sockets.end())
    {
        listen(it->first, 128);
        FD_SET(it->first, &master);
        it++;
    }
    return ((--it)->first); //Возвращаем значение для fdmax (последний из прослушивающих сокетов)
}

void Sockets::fill_hints()
{
    memset(&_hints, 0, sizeof(_hints));
    _hints.ai_family = AF_UNSPEC; // указывает, что будет IPv4
    _hints.ai_socktype = SOCK_STREAM; //указывает, что сокет будет потоковый (TCP)
    _hints.ai_flags = AI_PASSIVE; //указывает, что структурам сокета нужно назначить адрес локального хоста
}

void Sockets::reuse_address(int listen_sock, struct addrinfo *servinfo)
{
    int yes = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (char *) &yes, sizeof(int)) == -1)
    {
        fprintf(stderr, "Error: Can't reuse address\n");
        freeaddrinfo(servinfo); // all done with this structure
        close_all();
        exit(1);
    }
}

void Sockets::reuse_port(int listen_sock, struct addrinfo *servinfo)
{
    int yes = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) == -1)
    {
        fprintf(stderr, "Error: Can't reuse port\n");
        freeaddrinfo(servinfo); // all done with this structure
        close_all();
        exit(1);
    }
}

void Sockets::listen_socket_setup(Server &server)
{
    struct addrinfo *p;
    struct addrinfo *servinfo;
    int status;

    if ((status = getaddrinfo(NULL, server.getPort().c_str(), &_hints, &servinfo)) != 0)
    {
        std::cout << "ERROR GETADDRINFO " << gai_strerror(status) << std::endl;
        exit(1);
    }
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        int listen_sock = -1;
        listen_sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listen_sock == -1)
            continue;
        reuse_address(listen_sock, servinfo);
        reuse_port(listen_sock, servinfo);
        if (bind(listen_sock, p->ai_addr, p->ai_addrlen) == -1)
        {
            perror("Cannot bind socket to address");
            close(listen_sock);
            continue;
        }
        listen_sockets.insert(std::make_pair(listen_sock, server));
        break;
    }
    freeaddrinfo(servinfo);
}

void    Sockets::close_all()
{
    std::map<int, Server>::iterator it = listen_sockets.begin();
    while (it != listen_sockets.end())
    {
        close(it->first);
        it++;
    }
}

int Sockets::accept_connection(int i, fd_set &master, int &fdmax)
{
    struct sockaddr_storage client_addr;
    socklen_t s_size = sizeof(client_addr);
    std::map<int, Server>::iterator it = listen_sockets.find(i);
    if (it != listen_sockets.end())
    {
        int sock = accept(it->first, (struct sockaddr *) &client_addr, &s_size);
        if (sock < 0)
        {
            perror("accept");
            exit(3);
        }
        FD_SET(sock, &master);
        connection_sockets.insert(std::make_pair(sock, it->second));
        if (sock > fdmax)
            fdmax = sock;
        fcntl(sock, F_SETFL, O_NONBLOCK);
        return 1;
    }
    else
        return 0; //Возвращаем ноль и сигнализируем, что это не прослушивающий сокет, а сокет соединения уже
}

void Sockets::remove_connection(int socket)
{
    std::map<int, Server>::iterator  it = connection_sockets.find(socket);
    if (it != connection_sockets.end()) //проверить
        connection_sockets.erase(it);
}