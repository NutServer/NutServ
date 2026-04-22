#pragma once 

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <cctype>
#include <sstream>
#include <sys/stat.h>

class Config{
public:
    std::string getroot();
    std::vector<std::string> getIndex();
    bool getAutoindex();
private:
    std::string root;
    std::vector<std::string> index;
    bool autoindex ; // A flag to decide if you should list folder contents if no index is found.
    std::map<int, std::string> error_pages; // Paths to custom files for 404, 500, etc.
    size_t client_max_body_size; // To prevent huge uploads
};