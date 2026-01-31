#ifndef __SUTILS_HH
#define __SUTILS_HH

#include <string>
#include <vector>

using string = std::string;

std::vector<string> split_string_into_vec(const string &str, char delimiter);
string get_token_behind_last_delimiter(const string &str,
                                       const string &delimiter);
string get_token_before_delimiter(const string &str, const string &delimiter);

#endif // __SUTILS_HH
