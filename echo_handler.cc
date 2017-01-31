#include "echo_handler.h"

EchoHandler::EchoHandler(char* request) {
    request_ = std::string(request);
}

EchoHandler::~EchoHandler() {}

std::string EchoHandler::build_response() {
    std::string response = build_status_line(200);
    response += build_header("Content-Type", "text/plain");
    response += "\r\n\r\n" + request_;
    return response;
}
