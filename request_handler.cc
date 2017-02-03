#include "request_handler.h"

RequestHandler::RequestHandler() {}

RequestHandler::~RequestHandler() {}

std::string RequestHandler::build_status_line(int status_code) {
    std::string status_line = "HTTP/1.1";

    if (status_code == 200) {
        status_line += " 200 OK";
    } else if (status_code == 404) {
        status_line += " 404 Not Found";
    }

    return status_line + "\r\n";
}

std::string RequestHandler::build_header(std::string field, std::string value) {
    return field + ": " + value + "\r\n";
}
