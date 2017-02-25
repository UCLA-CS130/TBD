#include "reverse_proxy_handler.h"

ReverseProxyHandler::ReverseProxyHandler() {}

ReverseProxyHandler::~ReverseProxyHandler() {}

ReverseProxyHandler::Status ReverseProxyHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    // To suppress unused param warning
    (void) uri_prefix;
    (void) config;
    return OK;
}

ReverseProxyHandler::Status ReverseProxyHandler::HandleRequest(const Request& request, Response* response) {
    // TODO: Implement reverse proxy request
}

std::string ReverseProxyHandler::GetName() {
    return "ReverseProxyHandler";
}

