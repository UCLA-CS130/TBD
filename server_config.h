#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include "config_parser.h"

// TODO: add tests for server config
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
