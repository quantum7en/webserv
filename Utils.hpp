#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

std::string skipSpaces(std::string &str);
std::vector<std::string> split(const std::string& str,const  std::string& delimeter);
std::string trim(std::string& s);

#endif