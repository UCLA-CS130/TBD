#include <iostream>
#include "server.h"
#include "config_parser.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: ./server <path to config file>\n");
        return 1;
    }

    try {
        // parse config file input to get port
        NginxConfigParser parser;
        NginxConfig config;
        parser.Parse(argv[1], &config);

        // start server and wait for requests
        boost::asio::io_service io_service;
        Server server(io_service, std::stoi(config.getConfigValue("listen")));
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}