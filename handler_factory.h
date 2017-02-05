#ifndef HANDLER_FACTORY_H
#define HANDLER_FACTORY_H

#include "server_config.h"
#include "http_request.h"
#include "request_handler.h"

class HandlerFactory {
public:
    HandlerFactory(ServerConfig* server_config, HttpRequest* http_request);
    std::unique_ptr<RequestHandler> create_handler();
    std::string transform_path();
    bool is_prefix(std::string short_str, std::string long_str);
private:
    ServerConfig* server_config_;
    HttpRequest* http_request_; 
};

#endif
