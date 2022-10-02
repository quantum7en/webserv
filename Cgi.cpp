#include "Cgi.hpp"

int Cgi::write_to_buf(int fd)
{
    int r = 1;
    while (r > 0)
    {
        r = read(fd, _buf, CGI_BUFSIZE);
        if (r == -1)
            return -1;
    }
    return 0;
}

void Cgi::parseCGI(Request & zapros)
{
    int i;
    int j = 0;
    i = _answer.find("\r\n\r\n", 0);
    if (static_cast<unsigned long>(i) == std::string::npos)
        i = 0;
    j = _answer.find("Status: ", 0);
    if (j >= 0)
        zapros.setStatus(_answer.substr(j + 8, 3));
    j = _answer.find("Content-Type: ", 0);
    int f = _answer.find("\r\n", j + 14);
    if (j >= 0)
        zapros.setContentType(_answer.substr(j + 14, f - (j + 14)));
    if (i >= 0)
        zapros.setAnswerBody(_answer.substr(i + 4, _answer.size() - (i + 4)));
    if (zapros.getStatus() == "201"){
        size_t pos = _answer.find("Location: ") + 10;
        size_t end = _answer.find("\r\n", pos);
        zapros.setLocation(_answer.substr(pos, end));
    }
}  

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*nstr;
	int		i;
	int		j;
	int		len;

	if (!s1 || !s2)
		return (0);
	len = strlen(s1) + strlen(s2);
	if (!(nstr = (char*)malloc(sizeof(char) * len + 1)))
		return (NULL);
	j = 0;
	i = 0;
	while (s1[j])
		nstr[i++] = s1[j++];
	j = 0;
	while (s2[j])
		nstr[i++] = s2[j++];
	nstr[i] = '\0';
	return (nstr);
}

void    Cgi::free_memory()
{
    for(int i = 0; _env[i] != NULL; i++)
        free(_env[i]);
    delete(_env);
}

void Cgi::define_cgi_path(std::string& file_path)
{
    _extentionOfScript = 1;
	if (file_path.rfind(".py") != std::string::npos)
		_pathToHandler = "/usr/bin/python";
	else if (file_path.rfind(".php") != std::string::npos)
		_pathToHandler = "/usr/bin/php";
	else if (file_path.rfind(".perl") != std::string::npos || file_path.rfind(".pl") != std::string::npos)
		_pathToHandler = "/usr/bin/perl";
	else if (file_path.rfind(".cgi") != std::string::npos || file_path.rfind(".exe") != std::string::npos)
		_extentionOfScript = 0;
}

void Cgi::create2darray(Request zapros)
{
    _env = new char *[11 + zapros.getHeaders().size()];
    _env[0] = strdup("AUTH_TYPE=Anonymous");
    _env[1] = ft_strjoin("CONTENT_LENGTH=", (zapros.getHeaderContentLength().c_str()));
    _env[2] = ft_strjoin("CONTENT_TYPE=", (zapros.getHeaderContentType()).c_str());
    _env[3] = strdup("GATEWAY_INTERFACE=CGI/1.1");
    _env[4] = ft_strjoin("REQUEST_METHOD=", (zapros.getMethod().c_str()));
    _env[5] = ft_strjoin("SCRIPT_NAME=", zapros.getResourseName().c_str());
    _env[6] = ft_strjoin("SERVER_NAME=", zapros.getCurrentServer()._host.c_str());
    _env[7] = ft_strjoin("SERVER_PORT=", zapros.getCurrentServer()._port.c_str());
    _env[8] = strdup("SERVER_PROTOCOL=HTTP/1.1");
    _env[9] = strdup("SERVER_SOFTWARE=webserv");
    _env[10] = ft_strjoin("PATH_TRANSLATED=", zapros.getCurrentServer()._root.c_str());
    _env[11] = NULL;
}

int Cgi::execute_cgi(Request & zapros, std::string& file_path)
{
    int status;
    int fds[2];
    int e;
    const char *argv[3] = {zapros.getResourseName().c_str(), zapros.getBody().c_str(), NULL};
    const char *argv_interp[4] = {zapros.getResourseName().c_str(), zapros.getBody().c_str(), _pathToHandler.c_str(), NULL}; // s interpretatorom
    create2darray(zapros);
    define_cgi_path(file_path);
    if (pipe(fds) == -1)
        std::cout << "pipe error" << std::endl;
    pid_t child_id = fork();
    if (child_id < 0)
        std::cout << "fork failed" << std::endl;
    else if (child_id == 0)
    {
        close(fds[0]);
        dup2(fds[1], 1);
        if (_extentionOfScript == 1)
            e = execve(file_path.c_str(), (char *const *) argv_interp, _env);
        else 
            e = execve(file_path.c_str(), (char *const *) argv, _env);
        if(e == -1)
        {
            close(fds[1]);
            exit(-1);
        }
    }
    else 
    {
        waitpid(child_id, &status, 0);
        close(fds[1]);
        ssize_t bytes_read = read(fds[0], _buf, CGI_BUFSIZE);
        _answer = std::string(_buf);
        memset(_buf, 0, CGI_BUFSIZE);
        close(fds[0]);
        if (bytes_read != 0)
            parseCGI(zapros);
    }
    free_memory();
    return 0;  
}
