#include "handler_factory.h"
#include <unordered_map>
#include <memory>
#include "echo_handler.h"
#include "static_file_handler.h"

HandlerFactory::HandlerFactory(ServerConfig* server_config, HttpRequest* http_request)
    : server_config_(server_config),
      http_request_(http_request) {}

std::unique_ptr<RequestHandler> HandlerFactory::create_handler() {
    std::unordered_map<std::string, std::string> path_map = server_config_->get_path_map();
    std::unique_ptr<RequestHandler> handler = nullptr;

    if (http_request_->request_path_ == path_map["echo"]) {
        handler = std::unique_ptr<RequestHandler>(new EchoHandler(http_request_->raw_request_string_));
    } else {
        handler = std::unique_ptr<RequestHandler>(new StaticFileHandler(http_request_));
    }

    return handler;
}
