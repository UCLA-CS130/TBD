#include <iostream>
#include <string>
#include <cstring>
#include "server.h"

using boost::asio::ip::tcp;

bool Server::runServer(const char* filename) {
    try {
        configParser.Parse(filename, &config);
        // TODO: verify format of config file
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), std::stoi(config.statements_[0]->tokens_[1])));

        for (;;) {
          tcp::socket socket(io_service);
          acceptor.accept(socket);

          char request_buf[2048];
          boost::system::error_code error;
          std::size_t request_len = socket.read_some(boost::asio::buffer(request_buf), error);

          char response_buf[4096] = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";

          if (request_len > 0) {
            std::memcpy(&response_buf[42], request_buf, request_len);
            //std::cout << response_buf << std::endl;
          }
          
          boost::asio::write(socket, boost::asio::buffer(response_buf, 42 + request_len), error);
          //std::cout << "write finished" << std::endl;
        }
        return true;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return false;
}