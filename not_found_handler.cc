#include "not_found_handler.h"

NotFoundHandler::NotFoundHandler() {}

NotFoundHandler::~NotFoundHandler() {}

NotFoundHandler::Status NotFoundHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    (void) uri_prefix;
    (void) config;
    return OK;
}

NotFoundHandler::Status NotFoundHandler::HandleRequest(const Request& request, Response* response) {
    (void) request;
    response->SetStatus(Response::ResponseCode::FILE_NOT_FOUND);
    response->AddHeader("Content-Type", "text/plain");

    std::string body = "404 File Not Found";
    response->SetBody(body);
    return FILE_NOT_FOUND;
}

std::string NotFoundHandler::GetName() {
    return "NotFoundHandler";
}
