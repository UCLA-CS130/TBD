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
    auto request = Request::Parse(data_string);
    Response response;
    std::unique_ptr<RequestHandler> handler = find_handler(request.uri());
    Status status = handler->HandleRequest(request, &response);

    return response.ToString();
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

// Finds the correct handler using longest uri prefix matching
std::unique_ptr<RequestHandler> Server::find_handler(std::string uri) {
    std::string longest_prefix_match = "";
    for (auto it = handler_map_.begin(); it != handler_map_.end(); it++) {
        if (is_prefix(it->first, uri) &&
            (it->first).size() > longest_prefix_match.size()) {
            longest_prefix_match = it->first;
        }
    }
    return handler_map_[longest_prefix_match];
}

// Returns true if short_str is a prefix of long_str
bool HandlerFactory::is_prefix(std::string short_str, std::string long_str) {
    if (short_str.size() > long_str.size()) return false;

    if (long_str.substr(0, short_str.size()) == short_str) {
        return true;
    } else {
        return false;
    }
}
