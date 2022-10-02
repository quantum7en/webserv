#include <iostream>
#include <string>
#include <fstream>

std::string extract_data(std::string key, std::string value_end, std::string &text)
{
    size_t pos = text.find(key) + key.length();
    size_t end = text.find(value_end);
    std::string result(text.substr(pos, end - pos));
    return result;
}

int check_if_file_already_exists(std::string filename){
    std::ifstream check(filename);
    if (check.is_open())
    {
        check.close();
        return -1;
    }
    return 0;
}

void    error_on_file_upload(std::string check){
    std::cout << "Status: 200\r\n"
                 "Content-Type: text/html\r\n\r\n"
                 "<!DOCTYPE html>\n"
                 " <head>\n"
                 "  <meta charset=\"utf-8\">\n"
                 "  <title>Failed to upload</title>\n"
                 " </head>\n"
                 " <body>\n"
                 "  <p>Failed to upload</p>\n"
                 " </body>\n"
                 "</html> ";
}

int main(int argc, char **argv, char **envp)
{
    std::string content_type(envp[2]);
    std::string root(envp[10]);
    std::string body(argv[1]);
    std::string host(envp[6]);
    std::string port(envp[7]);

    root.erase(0, 16);
    host.erase(0, 12);
    port.erase(0, 12);
    std::string delimetr(extract_data("boundary=", "\r\n", content_type));
    delimetr = "--" + delimetr;
    std::string filename(extract_data("filename=\"", "\"\r\n", body));
    if (check_if_file_already_exists(root + "/" + filename) == -1)
    {
        error_on_file_upload("");
        return 0;
    }
    size_t content_type_start = body.find("Content-Type", delimetr.length());
    size_t start_of_data = body.find("\r\n", content_type_start) + 4;
    size_t end_of_data = body.find(delimetr, start_of_data);
    std::ofstream new_file(root + "/" + filename);
    if (new_file.is_open())
    {
        new_file << body.substr(start_of_data, end_of_data - start_of_data);
        std::cout << "Status: 201\r\n"
                     "Content-Type: text/html\r\n"
                     "Location: http://" + host + ":" + port + "/" + filename + "\r\n\r\n" +
                     "<!DOCTYPE html>\n"
                     " <head>\n"
                     "  <meta charset=\"utf-8\">\n"
                     "  <title>Upload script</title>\n"
                     " </head>\n"
                     " <body>\n"
                     "  <p>File uploaded successfully</p>\n"
                     " </body>\n"
                     "</html> ";
        new_file.close();
        return 0;
    }
    error_on_file_upload("");
    return 0;
}

