#include <sstream>
#include <iostream>
#include "http_request.h"

HttpRequest::HttpRequest(std::string request)
    : raw_request_string_(request) {
    parse_request(request);
}

void HttpRequest::parse_request(std::string request) {
    std::vector<std::string> lines = split_lines(request);
    for (size_t i = 0; i < lines.size() - 1; i++) {
        // parse first line of request
        if (i == 0) {
            size_t first_space = lines[i].find(" ");
            std::string temp = lines[i].substr(first_space + 1);
            size_t second_space = temp.find(" ");
            request_path_ = temp.substr(0, second_space);
        }
    }
}

std::vector<std::string> HttpRequest::split_lines(std::string request) {
    std::stringstream ss(request);
    std::string line;
    std::vector<std::string> lines;

    if (!request.empty()) {
        // tokenize each line by newline
        while(std::getline(ss,line,'\n')){
            // remove carriage return from line
            lines.push_back(line.substr(0, line.size()-1));
        }
    }

    return lines;
}
