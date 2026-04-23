#include "../includes/HttpRequest.hpp"
#include "../includes/HttpRespons.hpp"

int main()
{

    Request R;

    R.feed( "POST /upload HTTP/1.1\r\nHost: localhost\r\nContent-Length: "
            "12\r\n\r\nHello World!",
            82 );
    try {
        // R.feed();
    } catch ( std::exception &e ) {
        std::cerr << "Error " << e.what() << std::endl;
    }
}