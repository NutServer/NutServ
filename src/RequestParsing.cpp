#include "../includes/HttpRequest.hpp"
#include "../includes/HttpRespons.hpp"

Request::Request()
{

    _currentState = READ_LINE;
    readPos       = 0;
    _lastPos      = 0;
    _bodyStartPos = 0;
    _expectedSize = 0;
    chunkEndCheck = false;
}

bool Request::crlfCheck( std::vector<char> &rawData, bool two )
{

    const char *crlf[] = { "\r\n", "\r\n\r\n" };
    std::vector<char>::iterator it_crlf;
    std::vector<char>::iterator searchStart = rawData.begin() + readPos;

    if ( two ) {
        it_crlf =
            std::search( searchStart, rawData.end(), crlf[ 1 ], crlf[ 1 ] + 4 );
        if ( it_crlf != rawData.end() )
            return true;
    } else {
        it_crlf =
            std::search( searchStart, rawData.end(), crlf[ 0 ], crlf[ 0 ] + 2 );
        if ( it_crlf != rawData.end() ) {
            readPos = std::distance( rawData.begin(), it_crlf ) + 2;
            return true;
        }
    }
    return false;
}

void Request::parseRequestLine()
{

    std::string del = "\r\n";
    static std::set<std::string> methods;
    if ( methods.empty() ) {
        methods.insert( "GET" );
        methods.insert( "POST" );
        methods.insert( "DELETE" );
        methods.insert( "PATCH" );
    }
    _Request = std::string( raw_data.begin() + _lastPos,
                            raw_data.begin() + ( readPos - 2 ) );
    std::stringstream ss( _Request );

    ss >> _method >> _path >> _version;
    if ( ss.fail() ) { // cehck if the last extraction failed (eg, the request
                       // was GET / or GET /upload )
        throw Request::BadRequestException404();
    }
    // check version + methode
    if ( ( _version != "HTTP/1.1" ) || !methods.count( _method ) ) {
        // STATUS ERROR (404, 500...)
        throw Request::BadRequestException500();
    }
    _lastPos = readPos;
}

static void lowering( std::string &key )
{
    for ( size_t i = 0; i < key.length(); ++i ) {
        key[ i ] = std::tolower( static_cast<unsigned char>( key[ i ] ) );
    }
}

void Request::parseHeaders()
{

    size_t pos;

    while ( crlfCheck( raw_data, false ) ) {
        std::string line = std::string( raw_data.begin() + _lastPos,
                                        raw_data.begin() + ( readPos - 2 ) );
        if ( line.empty() ) {
            if ( _headers.count( "transfer-encoding" ) ) {
                if ( _headers[ "transfer-encoding" ] == "chunked" ||
                     _headers[ "transfer-encoding" ] == "Chunked" ) {
                    _currentState = READ_CHUNKED;
                    _bodyStartPos = readPos;
                }
            } else if ( _headers.count( "content-length" ) ) {
                std::stringstream ss( _headers[ "content-length" ] );
                ss >> _expectedSize;
                _bodyStartPos = readPos;
                _currentState = READ_BODY;
            } else {
                _expectedSize = 0;
                _currentState = READ_BODY;
            }
            _bodyStartPos = readPos;
            break;
        }
        pos = line.find( ':' );
        if ( pos == std::string::npos )
            throw Request::BadRequestException404();
        std::string key   = line.substr( 0, pos );
        std::string value = line.substr( pos + 1 );
        size_t first      = value.find_first_not_of( " \t" );
        if ( first != std::string::npos ) {
            size_t last = value.find_last_not_of( " \t" );
            value       = value.substr( first, ( last - first + 1 ) );
        }
        _lastPos = readPos;
        lowering( key );
        // lowering(value);
        _headers[ key ] = value;
    }
}

void Request::reset()
{
    raw_data.erase( raw_data.begin(), raw_data.begin() + _lastPos );
    _currentState = READ_LINE;
    readPos       = 0;
    _lastPos      = 0;
    _bodyStartPos = 0;
    _expectedSize = 0;
    chunkEndCheck = false;
}

void Request::feed( const char *chunk, size_t size )
{
    // 1. Add new data to my internal "long-term" buffer
    raw_data.insert( raw_data.end(), chunk, chunk + size );

    // 2. Run the State Machine
    while ( true ) {
        switch ( _currentState ) {
        case READ_LINE:
            if ( crlfCheck( raw_data, false ) ) {
                parseRequestLine(); // Extracts POST, /path, etc.
                _currentState = READ_HEADERS;
            } else
                return; // Need more data!
            break;

        case READ_HEADERS:
            if ( crlfCheck( raw_data, true ) ) {
                parseHeaders(); // Extracts Content-Length
                if ( _currentState == READ_HEADERS )
                    return;
            } else
                return; // Need more data!
            break;

        case READ_BODY:
            if ( raw_data.size() - readPos >= _expectedSize ) {
                _currentState = COMPLETE;
                _body.assign( raw_data.begin() + readPos,
                              raw_data.begin() + readPos + _expectedSize );
                _lastPos = readPos + _expectedSize;
                return;
            } else
                return; // Need more data!
            break;
        case READ_CHUNKED:
            if ( chunkEndCheck ) {
                if ( crlfCheck( raw_data, false ) ) {
                    _lastPos      = readPos;
                    _currentState = COMPLETE;
                }
                return;
            } else if ( _chunkSize == 0 ) {
                if ( crlfCheck( raw_data, false ) ) {
                    std::string sizeline =
                        std::string( raw_data.begin() + _lastPos,
                                     raw_data.begin() + ( readPos - 2 ) );
                    std::stringstream ss;
                    ss << std::hex << sizeline;
                    if ( ( ss >> _chunkSize ) ) {
                        _lastPos = readPos;
                        if ( !_chunkSize ) {
                            if ( !crlfCheck( raw_data, false ) ) {
                                chunkEndCheck = true;
                                return;
                            }
                            _lastPos      = readPos;
                            _currentState = COMPLETE;
                            return;
                        }
                    } else
                        throw Request::BadRequestException404();
                } else
                    return;
            } else if ( _chunkSize ) {
                if ( raw_data.size() - readPos >= _chunkSize + 2 ) {
                    _body.insert( _body.end(), raw_data.begin() + _lastPos,
                                  raw_data.begin() + _lastPos + _chunkSize );
                    readPos    += _chunkSize + 2;
                    _lastPos    = readPos;
                    _chunkSize  = 0;
                } else
                    return;
            }
            break;

        case COMPLETE:
            return;
        }
    }
}

std::string Request::getpath() { return _path; }

void Request::extractBody( std::vector<char> &target )
{
    target.swap( this->_body );
}

const char *Request::BadRequestException404::what() const throw()
{
    return "Bad Request 404";
}

const char *Request::BadRequestException500::what() const throw()
{
    return "Bad Request 500";
}

Request::~Request() {}