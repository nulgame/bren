#include "string-utils.hh"
#include <sstream>

std::vector<string> split_string_into_vec(const string &str, char delimiter)
{
    std::vector<string> result;
    std::stringstream ss(str);
    string item;

    while (getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

std::string get_token_behind_last_delimiter(const string &str,
                                            const string &delimiter)
{
    auto start = 0U;
    auto end = str.find(delimiter);
    while (end != string::npos) {
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    return str.substr(start, end);
}

std::string get_token_before_delimiter(const string &str,
                                       const string &delimiter)
{
    int pos = str.find(delimiter);
    return str.substr(0, pos);
}
