#include "main.hpp"

int    check_for_limit_size_body(std::string &str_request, Server &server, Response &response)
{
    size_t pos = 0;
    pos = str_request.find("Content-Length: ");
    if (pos != std::string::npos)
    {
        size_t end_of_length = str_request.find("\r\n", pos);
        int length = atoi(str_request.substr(pos + 16, end_of_length - (pos + 16)).c_str());
        if (server._max_body_size != 0 && server._max_body_size < static_cast<size_t>(length))
        {
            response.error_413();
            return 1;
        }
    }
    return 0;
}

int main(int , char **argv) {


	 if (!argv[1] || strcmp(argv[1], "test.conf") != 0){
	 	std::cout << "Webserver requires a valid config" << std::endl;
	 	return 1;
	 }
    Config config(argv[1]);
    config.parseConfig();

    char buf[1000000];
    std::vector<Server> servers = config.getServers();
    memset(&buf, 0, sizeof(buf));
    Sockets sockets(servers);
    fd_set master;
    fd_set readset;
    fd_set writeset;
    std::map<int, Response> responses;
    std::set<int> need_to_close;
    FD_ZERO(&master);
    FD_ZERO(&readset);
    FD_ZERO(&writeset);
    int fdmax = sockets.listen_all(master);
    int save_socket = -1;
    std::string save_buf;
    while (1) {
        readset = master;
        writeset = master;
        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        if(select(fdmax + 1, &readset, &writeset, NULL, &timeout) == -1)
        {
            perror("select");
            exit(3);
        }
        for (int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &readset)) {
                if (sockets.accept_connection(i, master, fdmax) == 1)
                    continue;
                else {
                    ssize_t bytes_read = 0;
                    bytes_read = recv(i, buf, sizeof(buf) - 1, MSG_PEEK);
                    memset(&buf, 0, sizeof(buf));
                    bytes_read = recv(i, buf, bytes_read, MSG_WAITALL);
                    std::string buffer(buf);
                    memset(&buf, 0, sizeof(buf));
                    if (save_socket == i)
                    {
                        save_buf.append(buffer);
                        buffer = save_buf;
                        save_buf.clear();
                        save_socket = -1;
                    }
                    else if (buffer.find("multipart/form-data") != std::string::npos)
                    {
                        save_buf = buffer;
                        memset(&buf, 0, sizeof(buf));
                        save_socket = i;
                        continue;
                    }
                    if (bytes_read <= 0) {
                        close(i);
                        FD_CLR(i, &master);
                        sockets.remove_connection(i);
                    }
                    else {
                        Response resp;
                        if (check_for_limit_size_body(buffer, sockets.connection_sockets.find(i)->second, resp) == 1){
                            responses.insert(std::make_pair(i, resp));
                        }
                        else
                        {
                            Request zapros(sockets.connection_sockets.find(i)->second);
                            zapros.clean_request();
                            if (!zapros.parse_request(buffer.c_str(), resp))
                            {
                                resp.fill_hosts_and_root(servers);
                                resp.choose_method(zapros, servers);
                            }
                            responses.insert(std::make_pair(i, resp));
                            std::map<std::string, std::string>::iterator it = zapros.getHeaders().find("Connection");
                            if (it != zapros.getHeaders().end())
                            {
                                if (it->second == "close")
                                    need_to_close.insert(i);
                            }
                        }
                    }
                }
            }
            if (FD_ISSET(i, &writeset)){
                std::map<int, Response>::iterator it = responses.find(i);
                if (it != responses.end())
                {
                    ssize_t res = send(i, it->second.getAnswer().c_str(), it->second.getAnswer().length(), 0);
                    if (res <= 0)
                    {
                        close(i);
                        FD_CLR(i, &master);
                        sockets.remove_connection(i);
                        responses.erase(it);
                    }
                    else
                    {
                        if (it->second.getAnswer().find("Connection: close\r\n") != std::string::npos ||
                            need_to_close.find(i) != need_to_close.end())
                        {
                            // std::cout << "Closing connection" << std::endl;
                            close(i);
                            FD_CLR(i, &master);
                            sockets.remove_connection(i);
                        }
                        responses.erase(it);
                    }
                }
            }
        }
    }
    return 0;
}