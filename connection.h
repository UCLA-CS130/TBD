#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

class Connection {
public:
    Connection(boost::asio::io_service& io_service) : socket_(io_service) {}
    virtual ~Connection();
    tcp::socket& socket();
    void start();
    bool handle_read(const boost::system::error_code& error, size_t bytes_transferred);
    bool closeSocket(const boost::system::error_code& error);
private:
    static const int MAX_LENGTH = 2048;
    void copyRequest(char* response, size_t bytes_transferred, size_t headerLength);
    tcp::socket socket_;
    char data_[MAX_LENGTH];
};

#endif