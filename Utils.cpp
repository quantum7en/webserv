#include "Utils.hpp"

std::string skipSpaces(std::string &str)
{
	size_t begin = 0;
	size_t end = str.size() - 1;

	while (begin < str.size()) {
		if (str[begin] == ' ' || (str[begin] >= 9 && str[begin] <= 13))
			begin++;
		else
			break ;
	}
	while (end > 0) {
		if (str[end] == ' ' || (str[end] >= 9 && str[end] <= 13))
			end--;
		else
			break ;
	}
	return (str.substr(begin, end - begin + 1));
}

std::vector<std::string> split(const std::string& str,const  std::string& delimeter)
{
	std::vector<std::string> strVector;
	size_t prev = 0;
	size_t pos = str.find(delimeter, prev);

	while ((pos < str.length()) && (prev < str.length())) {
		if (pos == std::string::npos)
			pos = str.length();
		std::string item = str.substr(prev, pos - prev);
		if (!item.empty())
			strVector.push_back(item);
		prev = pos + delimeter.length();
		pos = str.find(delimeter, prev);
	}
	if (str.size() != prev)
		strVector.push_back(str.substr(prev, pos - prev));
	return (strVector);
}

std::string trim(std::string& s) {
	size_t pos;
	size_t len;
	pos = 0;
	while (pos < s.length() && isspace(s[pos]))
		++pos;
	if (pos == s.length())
		return std::string();
	len = s.length() - 1;
	while (len > pos && isspace(s[len]))
		--len;
	if (len == pos && isspace(s[pos]))
		return std::string();
	else
		++len;
	len -= pos;
	return s.substr(pos, len);
}