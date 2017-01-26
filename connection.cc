#include <iostream>
#include <cstring>
#include "connection.h"

using boost::asio::ip::tcp;

Connection::~Connection() {}

tcp::socket& Connection::socket() {
    return socket_;
}

// start a connection by listening to web requests
void Connection::start() {
    socket_.async_read_some(boost::asio::buffer(data_, MAX_LENGTH),
        boost::bind(&Connection::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

// handler for received requests
// echo back request with status and content-type
bool Connection::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        char response[2048] = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";
        size_t headerLength = std::strlen(response);
        copyRequest(response, bytes_transferred, headerLength);
        boost::asio::async_write(socket_,
            boost::asio::buffer(response, bytes_transferred + headerLength),
            boost::bind(&Connection::closeSocket, this,
            boost::asio::placeholders::error));
        return true;
    } else {
        delete this;
        return false;
    }
}

// shutdown and close socket after responding to request
bool Connection::closeSocket(const boost::system::error_code& error) {
    if (!error) {
        socket_.close();
        return true;
    } else {
        delete this;
        return false;
    }
}

// construct response by placing request after headers
void Connection::copyRequest(char* response, size_t bytes_transferred, size_t headerLength) {
    std::memcpy(&response[headerLength], data_, bytes_transferred);
}