#include "Request.hpp"
#include <cstring>
#include <string>

class Response;

int Request::check_request(Response & resp)
{
    int i = -1;
    int len = _protocol_version.length();
    const char *str = _protocol_version.c_str();
    i = _protocol_version.find("HTTP/", 0);
    if(i != 0)
    {
        resp.setAnswer(resp.error_400(getCurrentServer()._host + ":" + getCurrentServer().getPort()));
        return (-1);
    }
    for(int i = 5; i < len; i++)
    {
        if((!(isdigit(str[i]))) && str[i] != '.')
        {
            resp.setAnswer(resp.error_400(getCurrentServer()._host + ":" + getCurrentServer().getPort()));
            return (-1);
        }
    }
    return 0;
}

void Request::clean_request()
{
    _sl = 0;
    _hd = 0;
    _bd = 0;
    _len = 0;
}

void Request::parse_start_line()
{
    int flag = 0;
    size_t pos = 0;
    for(size_t i = 0; i < _len; i++)
    {
        if(_buf[i] == ' ' && flag == 0)
        {
            _method = _buf.substr(0, i);
            pos = i + 1;
            flag = 1;
        }
        else if(_buf[i] == ' ' && flag == 1) 
        {
            _resource_name = _buf.substr(pos, (i - pos));
            pos = i + 1;
            flag = 2;
        }
        else if (_buf[i] == '\r' && _buf[i + 1] == '\n' && flag == 2)
        {
            _protocol_version = _buf.substr(pos, (i - pos));
            pos = i + 1;
            flag = 3;
            break;
        }
    }
}

void Request::cut_buf(int flag)
{
    std::string temp;
    size_t i = 0;
    i = _buf.find("\r\n\r\n");
    if (_hd == 1 && i != std::string::npos && flag == 0)
    {
        i += 4;
        temp = _buf.substr(i, _len);
        _buf = temp;
        _len -= i;
        return;
    }
    i = 0;
    for(; i < _len; i++)
    {
        if (_buf[i] == '\r' && _buf[i + 1] == '\n')
        {
            i += 2;
            _len -= i;
            break;
        }
    }
    temp = _buf.substr(i, _len);
    _buf = temp;
}

void Request::parse_headers()
{
    std::string key;
    std::string val;
    int flag = 0;
    size_t pos = 0;
    for(size_t i = 0; i < _len; i++)
    {
        if(_buf[i] == ':' && _buf[i + 1] == ' ')
        {
            key = _buf.substr(pos, (i - pos));
            flag = 1;
            pos = i + 2;
        }
        else if(_buf[i] == '\r' && _buf[i + 1] == '\n')
        {
            val = _buf.substr(pos, (i - pos));
            _headers.insert(make_pair(key, val));
            pos = i + 2;   
        }
    }
}

void Request::parse_body()
{
    int body_len = -1;
    if(_headers.count("Content-Length"))
    {
        body_len = std::strtol(_headers["Content-Length"].c_str(), 0, 10);
        _body.append(_buf, 0, body_len);
    }
    else if(_headers.count("Transfer-Encoding"))
    {
        std::string tmp;
        int pos = 0;
		_bd_flag = 0;
        for(size_t i = 0; i < _len; i++)
        {
            if (_buf[i] == '\r' && _buf[i + 1] == '\n')
            {
                tmp = _buf.substr(pos, i - pos);
                body_len = std::strtol(tmp.c_str(), 0, 16);
				pos = i + 2;
            }
			if (body_len == 0)
				break;
            if (body_len != -1 && _buf[i] == '\r' && _buf[i + 1] == '\n')
            {
                _body.append(_buf, pos, body_len);
				_bd_flag = 1;
				i += body_len + 2;
				pos += body_len + 2;
            }
        }
    }
}

int Request::parse_request(std::string str, Response & resp)
{
    _buf += str;
    _len = _buf.length();
    int found = _buf.find("\r\n\r\n");
    if(_sl == 0 && _buf.find("\r\n"))
    {
        parse_start_line();
        _sl = 1;
        cut_buf(0);
    }
    if (_sl == 1 && _hd == 0 && found != -1)
    {
        parse_headers();
        _hd = 1;
        cut_buf(0);
    }
    if ( _len != 0 && _sl == 1 && _hd == 1 && _bd == 0)
    {
        parse_body();
        cut_buf(1);
        _bd = 1;
    }
    if (_sl == 1 && _hd == 1 && _bd == 1)
    {
        clean_request();
    }
    return(check_request(resp));
}

std::string Request::getHeaderContentLength() const
{
    return _headers.find("Content-Length")->second;
}

std::string Request::getHeaderContentType() const
{
    return _headers.find("Content-Type")->second;
}
