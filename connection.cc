/* Source:
    http://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/example/cpp03/echo/async_tcp_echo_server.cpp
*/

#include <iostream>
#include <cstring>
#include "connection.h"
#include "echo_handler.h"
#include "static_file_handler.h"
#include "http_request.h"
#include "handler_factory.h"

using boost::asio::ip::tcp;

Connection::Connection(boost::asio::io_service& io_service, ServerConfig* server_config)
    : socket_(io_service),
      server_config_(server_config) {}

Connection::~Connection() {}

tcp::socket& Connection::socket() {
    return socket_;
}

// start a connection by listening to web requests
void Connection::start() {
    socket_.async_read_some(boost::asio::buffer(data_, MAX_LENGTH),
        boost::bind(&Connection::handle_read, this,
        boost::asio::placeholders::error));
}

// handler for received requests
// echo back request with status and content-type
bool Connection::handle_read(const boost::system::error_code& error) {
    if (!error) {
        std::string data_string = std::string(data_);

        HttpRequest http_request(data_string);
        HandlerFactory handler_factory(server_config_, &http_request);
        // StaticFileHandler static_file_handler(&http_request);
        //EchoHandler echo_handler(data_string);
        // std::string response = static_file_handler.build_response();
        std::unique_ptr<RequestHandler> handler = handler_factory.create_handler();
        std::string response = handler->build_response();

        size_t response_length = response.size();
        boost::asio::async_write(socket_,
            boost::asio::buffer(response, response_length),
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
