#include "echo_handler.h"

EchoHandler::EchoHandler() {}

EchoHandler::~EchoHandler() {}

Status EchoHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    return Status::OK;
}

Status EchoHandler::HandleRequest(const Request& request, Response* response) {
    response.SetStatus(ResponseCode::OK);
    response.AddHeader("Content-Type", "text/plain");
    response.SetBody(request.raw_request());
    return Status::OK;
}
