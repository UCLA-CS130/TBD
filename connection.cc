#include <iostream>
#include "connection.h"

using boost::asio::ip::tcp;

tcp::socket& Connection::socket() {
    return socket_;
}

void Connection::start() {
    socket_.async_read_some(boost::asio::buffer(data_, MAX_LENGTH),
        boost::bind(&Connection::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void Connection::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        std::cout << data_ << std::endl;
        boost::asio::async_write(socket_,
            boost::asio::buffer(data_, bytes_transferred),
            boost::bind(&Connection::handle_write, this,
            boost::asio::placeholders::error));
    } else {
        delete this;
    }
}

void Connection::handle_write(const boost::system::error_code& error) {
    if (!error) {
        socket_.async_read_some(boost::asio::buffer(data_, MAX_LENGTH),
            boost::bind(&Connection::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    } else {
        delete this;
    }
}