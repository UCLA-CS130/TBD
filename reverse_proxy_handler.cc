#include "reverse_proxy_handler.h"

ReverseProxyHandler::ReverseProxyHandler() {}

ReverseProxyHandler::~ReverseProxyHandler() {}

ReverseProxyHandler::Status ReverseProxyHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    uri_prefix_ = uri_prefix;

    bool remote_host_found = false;
    bool remote_port_found = false;

    // expecting two statements in child block: remote_host <remote_host>;
    //                                          remote_port <remote_port>;
    if (config.statements_.size() == 2) {
        for (std::size_t i = 0; i < config.statements_.size(); i++) {
            if (config.statements_[i]->tokens_.size() != 2) {
                return INTERNAL_ERROR;
            } else if (config.statements_[i]->tokens_[0] == "remote_host") {
                remote_host_ = config.statements_[i]->tokens_[1];
                remote_host_found = true;
            } else if (config.statements_[i]->tokens_[0] == "remote_port") {
                remote_port_ = std::stoi(config.statements_[i]->tokens_[1];
                remote_port_found = true;
            } else {
                return INTERNAL_ERROR;
            }
        }
    } else {
        return INTERNAL_ERROR;
    }    

    if (remote_host_found && remote_port_found) {
        return OK;
    } else {
        return INTERNAL_ERROR;
    }
}

ReverseProxyHandler::Status ReverseProxyHandler::HandleRequest(const Request& request, Response* response) {
    // TODO: Implement reverse proxy request
}

std::string ReverseProxyHandler::GetName() {
    return "ReverseProxyHandler";
}

