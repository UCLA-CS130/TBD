#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include "config_parser.h"

// TODO: add tests for server config
class ServerConfig {
public:
    ServerConfig(NginxConfig* config) {
        std::string port_string = config->getConfigValue("listen");
        if (port_string != "") {
            port_ = std::stoi(config->getConfigValue("listen"));
            if (port_ < 1024 || port_ > 65535) {
                std::cerr << "The port value must be between 1024 and 65535." << std::endl;
                exit(1);
            }
        } else {
            std::cerr << "The port must be a valid number." << std::endl;
            exit(1);
        }

        path_map = config->extract_path_map();
    }

    int getPort() {
        return port_;
    }

    std::unordered_map<std::string, std::string> get_path_map() {
        return path_map;
    }

private:
    int port_;
    std::unordered_map<std::string, std::string> path_map;
};

#endif
