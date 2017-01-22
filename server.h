#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <boost/asio.hpp>
#include "config_parser.h"
#include "connection.h"

class Server {
public:
	Server(boost::asio::io_service& io_service, int port);
    virtual ~Server();
private:
    void start_accept();
    void handle_accept(Connection* new_connection, const boost::system::error_code& error);
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
};

#endif