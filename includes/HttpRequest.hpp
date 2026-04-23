#pragma once

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <sstream>

#define print std::cout

class Request
{
    public:
        bool crlfCheck( std::vector<char> &rawData, bool two );
        void parseRequestLine();
        void parseHeaders();
        void reset();
        void extractBody( std::vector<char> &target );
        void feed( const char *chunk, size_t size );
        std::string getpath();
        Request();
        ~Request();

        class BadRequestException404 : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };

        class BadRequestException500 : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };

    private:
        enum states {
            READ_LINE,
            READ_HEADERS,
            READ_BODY,
            COMPLETE,
            READ_CHUNKED
        };

        bool chunkEndCheck;
        size_t _chunkSize;
        states _currentState;
        size_t readPos;
        size_t _bodyStartPos;
        size_t _lastPos;
        std::vector<char> raw_data; // container that contains the gathered data
                                    // collected from the previous part
        std::string _Request;       // done
        std::string _method;        // done
        std::string _path;          // done
        std::string _version;       // done                        // "POST"
        std::map<std::string, std::string> _headers;
        std::vector<char> _body; // The actual payload
        size_t _expectedSize;    // Taken from headers
};