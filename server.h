#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <boost/asio.hpp>
#include "config_parser.h"
#include "connection.h"
#include "server_config.h"

class Server {
public:
	Server(boost::asio::io_service& io_service, ServerConfig* server_config);
    virtual ~Server();
    bool handle_accept(Connection* new_connection, const boost::system::error_code& error);
private:
    void start_accept();
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
    ServerConfig* server_config_;
};

#endif
