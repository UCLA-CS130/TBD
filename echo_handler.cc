#include "echo_handler.h"

EchoHandler::EchoHandler(char* request) {
    request_ = request;
}

EchoHandler::~EchoHandler() {}

std::string EchoHandler::constructResponse() {
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    response += std::string(request_);
    return response;
}
