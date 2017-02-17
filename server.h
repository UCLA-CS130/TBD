#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <boost/asio.hpp>
#include "config_parser.h"
#include "server_config.h"

class Server {
public:
	Server(boost::asio::io_service& io_service, ServerConfig* server_config);
    virtual ~Server();
    void run();
    std::string handle_read(const char* data);
private:
    static const int MAX_LENGTH = 2048;
    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    ServerConfig* server_config_;
};

#endif
