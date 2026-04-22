#pragma once

#include "HttpRequest.hpp"
#include "Config.hpp"
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <cctype>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>

class HttpRespons {
    private:
        int         _statusCode;
        std::string _statusMessage;
        std::string _filePath;      // The physical path we found
        std::string _contentType;   // e.g., "text/html"
        std::vector<char> _body;    // The actual file content
        
        // Helper maps (MIME and Status Codes)
        static std::map<int, std::string> _statusMap;
        static std::map<std::string, std::string> _mimeMap;

    public:
        void mimefillingmap();
        HttpRespons(Request &req, Config &conf); // The "Assembly Line" starts here
        std::string getFullResponse();           // Combines headers + body
        ~HttpRespons();
        // ... getters and setters
};