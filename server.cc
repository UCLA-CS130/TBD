#include <iostream>
#include <string>
#include <cstring>
#include <boost/asio.hpp>
#include "config_parser.h"
#include "server.h"

using boost::asio::ip::tcp;

bool Server::runServer(const char* filename) {
    try {
        NginxConfigParser configParser;
        NginxConfig config;
        configParser.Parse(filename, &config);

        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), std::stoi(config.statements_[0]->tokens_[1])));
        for (;;) {
          tcp::socket socket(io_service);
          acceptor.accept(socket);

          char buf[1024];
          boost::system::error_code error;
          std::size_t len = socket.read_some(boost::asio::buffer(buf), error);

          char response[1024] = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";

          if (len > 0) {
            std::memcpy(&response[43], buf, len);
            //std::cout << response << std::endl;
          }
          
          boost::asio::write(socket, boost::asio::buffer(response, len), error);
          //std::cout << "write finished" << std::endl;
          
          return true;
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return false;
}