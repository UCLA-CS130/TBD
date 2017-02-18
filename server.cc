/* Source:
    http://www.boost.org/doc/libs/1_53_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_server.cpp
*/

#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include "server.h"
#include "echo_handler.h"
#include "static_file_handler.h"
#include "http_request.h"
#include "handler_factory.h"

using boost::asio::ip::tcp;

Server::Server(boost::asio::io_service& io_service, NginxConfig* config)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), std::stoi(config->getConfigValue("port")))),
      config_(config) {
    handler_map_ = create_handler_map(config);
}

Server::~Server() {}

// Start accepting requests
void Server::run() {
    while (true) {
        tcp::socket socket(io_service_);
        acceptor_.accept(socket);

        char data[MAX_LENGTH];
        memset(data, 0, MAX_LENGTH);
        boost::system::error_code error;
        socket.read_some(boost::asio::buffer(data), error);

        if (error) {
            std::cout << "Error reading request" << std::endl;
        } else {
            std::string message = handle_read(data);
            boost::asio::write(socket, boost::asio::buffer(message, message.size()));
        }
        socket.close();
    }
}

std::string Server::handle_read(const char* data) {
    std::string data_string = std::string(data);
    HttpRequest http_request(data_string);
    HandlerFactory handler_factory(server_config_, &http_request);
    std::unique_ptr<RequestHandler> handler = handler_factory.create_handler();
    std::string response = handler->build_response();

    return response;
}

std::unordered_map<std::string, std::unique_ptr<RequestHandler> > Server::create_handler_map(NginxConfig* config) {
    std::unordered_map<std::string, std::unique_ptr<RequestHandler> > handler_map;
    std::vector<std::shared_ptr<NginxConfigStatement>> statements = config->statements_;
    
    for (size_t i = 0; i < statements.size(); i++) {
        if (statements[i]->tokens_[0] == "path") {
            std::unique_ptr<RequestHandler> handler = nullptr;
    
            if (statements[i]->tokens_[2] == "EchoHandler") {
                handler = std::unique_ptr<RequestHandler>(new EchoHandler());
            } else if (statements[i]->tokens_[2] == "StaticHandler") {
                handler = std::unique_ptr<RequestHandler>(new StaticFileHandler());
            } else {
                continue;
            }

            std::string uri_prefix = statements[i]->tokens_[1];
            handler->Init(uri_prefix, statements[i]->child_block_);
            handler_map[uri_prefix] = handler;
        }
    }

    return handler_map;
}
