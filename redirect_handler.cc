#include "redirect_handler.h"

RedirectHandler::RedirectHandler() {}

RedirectHandler::~RedirectHandler() {}

RedirectHandler::Status RedirectHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    (void) uri_prefix;
    
    bool url_found = false;
    // expecting one statement in child block: url <url>;                                        
    if (config.statements_.size() == 1) {
        for (std::size_t i = 0; i < config.statements_.size(); i++) {
            if (config.statements_[i]->tokens_.size() != 2) {
                return INTERNAL_ERROR;
            } else if (config.statements_[i]->tokens_[0] == "url") {
                url_ = config.statements_[i]->tokens_[1];
                url_found = true;
            } else {
                return INTERNAL_ERROR;
            }
        }
    } else {
        return INTERNAL_ERROR;
    }
    if (!url_found) {
        return INTERNAL_ERROR;
    }

    return OK;
}

RedirectHandler::Status RedirectHandler::HandleRequest(const Request& request, Response* response) {
    (void) request;
    response->SetStatus(Response::ResponseCode::FOUND);
    response->AddHeader("Location", url_);
    std::cout << response->ToString() << std::endl;
    return FOUND;
}

std::string RedirectHandler::GetName() {
    return "RedirectHandler";
}
