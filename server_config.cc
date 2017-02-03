#include "server_config.h"

ServerConfig::ServerConfig(NginxConfig* config) {
    std::string port_string = config->getConfigValue("listen");
    if (port_string != "") {
        port_ = std::stoi(config->getConfigValue("listen"));
        if (port_ < 1024 || port_ > 65535) {
            std::cerr << "The port value must be between 1024 and 65535. Defaulting to 8080" << std::endl;
        }
    } else {
        std::cerr << "The port must be a valid number. Defaulting to 8080" << std::endl;
    }

    // If port is missing, default to 8080
    if (port_ == 0) port_ = 8080;

    path_map_ = config->extract_path_map();
    // If path mapping is empty, default to 'root' -> './'
    if (path_map_.empty()) path_map_["root"] = "./";
}

int ServerConfig::get_port() {
    return port_;
}

std::unordered_map<std::string, std::string> ServerConfig::get_path_map() {
    return path_map_;
}
