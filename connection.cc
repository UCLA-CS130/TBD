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
        char response[MAX_LENGTH] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
        size_t header_length = std::strlen(response);
        copy_request(response, bytes_transferred, header_length);
        boost::asio::async_write(socket_,
            boost::asio::buffer(response, bytes_transferred + header_length),
            boost::bind(&Connection::close_socket, this,
            boost::asio::placeholders::error));
        return true;
    } else {
        delete this;
        return false;
    }
}

// shutdown and close socket after responding to request
bool Connection::close_socket(const boost::system::error_code& error) {
    if (!error) {
        socket_.close();
        return true;
    } else {
        delete this;
        return false;
    }
}

// construct response by placing request after headers
void Connection::copy_request(char* response, size_t bytes_transferred, size_t header_length) {
    std::memcpy(&response[header_length], data_, bytes_transferred);
}