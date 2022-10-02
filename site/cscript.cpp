#include <iostream>

int main(int argc, char **argv){
    std::cout << "Status: 200"
                 "Content-type: text/html\r\n\r\n"
                 "<!DOCTYPE html>\n"
                 "<html lang=\"en\">\n"
                 "<head>\n"
                 "    <meta charset=\"UTF-8\">\n"
                 "    <title>C++ SCRIPT</title>\n"
                 "</head>\n"
                 "<body>\n";
    if (argc >= 2)
    {
        std::string values(argv[1]);
        int start = values.find("first_name") + 11;
        int len = values.find("&") - start;
        std::string first_name = values.substr(start, len);
        std::cout << "<p>Your first name: " << first_name << "</p>";
        start = values.find("last_name") + 10;
        len = values.find("&") - start;
        std::string last_name = values.substr(start, len);
        std::cout << "<p>Your last name: " << last_name << "</p>";
        std::cout << "<a href=\"/index.html\">\n"
                     "HOME\n"
                     "</a></body></html>";
    }
    else
    {
        std::cout << "<p>Wrong arguments</p></body></html>";
    }
}

