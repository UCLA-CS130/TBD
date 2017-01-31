#include "echo_handler.h"

EchoHandler::EchoHandler(std::string request) {
    request_ = request;
}

EchoHandler::~EchoHandler() {}

std::string EchoHandler::build_response() {
    std::string response = build_status_line(200);
    response += build_header("Content-Type", "text/plain");
    response += "\r\n" + request_;
    return response;
}
