#include "handler_factory.h"
#include <unordered_map>
#include <memory>
#include "echo_handler.h"
#include "static_file_handler.h"

HandlerFactory::HandlerFactory(ServerConfig* server_config, HttpRequest* http_request)
    : server_config_(server_config),
      http_request_(http_request) {}

RequestHandler* HandlerFactory::create_handler() {
    std::unordered_map<std::string, std::string> path_map = server_config_->get_path_map();
    RequestHandler* handler = nullptr;

    if (http_request_->request_path_ == path_map["/echo/"]) {
        handler = new EchoHandler(http_request_->raw_request_string_);
    } else {
        std::string file_path = transform_path();
        handler = new StaticFileHandler(file_path);
    }

    return handler;
}

// TODO: handle mutliple matches
std::string HandlerFactory::transform_path() {
    // TODO: handle favicon, dont send it as binary
    if (http_request_->request_path_ == "/favicon.ico") return "favicon.png";

    std::unordered_map<std::string, std::string> path_map = server_config_->get_path_map();
    std::string request_path = http_request_->request_path_;

    for (auto it = path_map.begin(); it != path_map.end(); it++) {
        if (is_prefix(it->first, request_path)) {
            std::string right = request_path.substr((it->first).size());
            return it->second + right;
        }
    }

    return "";
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
