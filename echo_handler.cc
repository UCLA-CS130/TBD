#include "echo_handler.h"
#include "compressor.h"

EchoHandler::EchoHandler() {}

EchoHandler::~EchoHandler() {}

EchoHandler::Status EchoHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    // To suppress unused param warning
    (void) uri_prefix;
    (void) config;
    return OK;
}

EchoHandler::Status EchoHandler::HandleRequest(const Request& request, Response* response) {
    response->SetStatus(Response::ResponseCode::OK);
    response->AddHeader("Content-Type", "text/plain");

    Compressor compressor;
    std::string compressed_string = compressor.compress(request.raw_request());
    
    response->AddHeader("Content-Encoding", "gzip");
    response->SetBody(compressed_string);
    return Status::OK;
}

std::string EchoHandler::GetName() {
    return "EchoHandler";
}
