#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server_config.h"

using boost::asio::ip::tcp;

class Connection {
public:
    Connection(boost::asio::io_service& io_service, ServerConfig* server_config);
    virtual ~Connection();
    tcp::socket& socket();
    void start();
    bool handle_read(const boost::system::error_code& error);
    bool close_socket(const boost::system::error_code& error);
private:
    static const int MAX_LENGTH = 2048;
    tcp::socket socket_;
    char data_[MAX_LENGTH];
    ServerConfig* server_config_;
};

#endif
