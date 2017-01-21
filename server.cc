#include <iostream>
#include <string>
#include <cstring>
#include "server.h"

using boost::asio::ip::tcp;

Server::Server() {}

Server::~Server() {}

bool Server::runServer(const char* filename) {
    try {
        configParser.Parse(filename, &config);
        // TODO: verify format of config file
        int port = std::stoi(config.getConfigValue("port"));
        if (port < 1024) {
            std::cerr << "The web server port number must be greater than 1023." << std::endl;
            return false;
        }

        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

        for (;;) {
          tcp::socket socket(io_service);
          acceptor.accept(socket);

          char request_buf[2048];
          boost::system::error_code error;
          std::size_t request_len = socket.read_some(boost::asio::buffer(request_buf), error);

          char response_buf[4096] = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";

          if (request_len > 0) {
            std::memcpy(&response_buf[42], request_buf, request_len);
          }
          
          boost::asio::write(socket, boost::asio::buffer(response_buf, 42 + request_len), error);
        }
        return true;
    } catch (std::exception& e) {
        std::string errorMsg = "Error: ";
        if (std::string(e.what()) == "stoi")
            errorMsg += "The port must be a valid number greater than 1023";

        std::cerr << errorMsg << std::endl;
    }
    return false;
}