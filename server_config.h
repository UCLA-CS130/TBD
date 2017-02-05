#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include "config_parser.h"

class ServerConfig {
public:
    ServerConfig(NginxConfig* config);
    int get_port();
    std::unordered_map<std::string, std::string> get_path_map();

private:
    int port_;
    std::unordered_map<std::string, std::string> path_map_;
};

#endif
