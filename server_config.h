#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include "config_parser.h"

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
        static_base_path_ = config->getConfigValue("static_base_path");
    }
    int getPort() {
        return port_;
    }
    std::string getStaticBasePath() {
        return static_base_path_;
    }
private:
    int port_;
    std::string static_base_path_;
};

#endif
