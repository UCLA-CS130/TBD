#include "redirect_handler.h"

RedirectHandler::RedirectHandler() {}

RedirectHandler::~RedirectHandler() {}

RedirectHandler::Status RedirectHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    (void) uri_prefix;

    // expecting one statement in child block: url <url>;
    bool url_found = false;
    for (std::size_t i = 0; i < config.statements_.size(); i++) {
        if (config.statements_[i]->tokens_.size() >= 2 &&
            config.statements_[i]->tokens_[0] == "url") {
            url_ = config.statements_[i]->tokens_[1];
            url_found = true;
        }
    }
    if (!url_found) {
        return INTERNAL_ERROR;
    }

    return OK;
}

RedirectHandler::Status RedirectHandler::HandleRequest(const Request& request, Response* response) {
    (void) request;
    response->SetStatus(Response::ResponseCode::REDIRECT);
    response->AddHeader("Location", url_);
    return OK;
}

std::string RedirectHandler::GetName() {
    return "RedirectHandler";
}
