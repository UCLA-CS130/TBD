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
        char response[2048] = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";
        copyRequest(response, bytes_transferred);
        boost::asio::async_write(socket_,
            boost::asio::buffer(response, bytes_transferred + 42),
            boost::bind(&Connection::closeSocket, this,
            boost::asio::placeholders::error));
    } else {
        delete this;
    }
}

void Connection::closeSocket(const boost::system::error_code& error) {
    if (!error) {
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
        socket_.close();
    } else {
        delete this;
    }
}

void Connection::copyRequest(char* response, size_t bytes_transferred) {
    std::memcpy(&response[42], data_, bytes_transferred);
}