#pragma once

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>

class Config
{
    public:
        Config();
        std::string getroot();
        std::vector<std::string> getIndex();
        bool getAutoindex();

    private:
        std::string root;
        std::vector<std::string> index;
        bool autoindex; // A flag to decide if you should list folder contents
                        // if no index is found.
        std::map<int, std::string>
            error_pages;             // Paths to custom files for 404, 500, etc.
        size_t client_max_body_size; // To prevent huge uploads
};