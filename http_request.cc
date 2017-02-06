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
            std::cout << "before temp" << std::endl;
            std::string temp = lines[i].substr(first_space + 1);
            std::cout << "after temp" << std::endl;
            size_t second_space = temp.find(" ");
            std::cout << "before request_path_" << std::endl;
            request_path_ = temp.substr(0, second_space);
            std::cout << "after request_path_" << std::endl;
        } 
        // else {
        //     size_t pos = lines[i].find(": ");
        //     std::cout << "before header_name" << std::endl;
        //     std::string header_name = lines[i].substr(0, pos);
        //     std::cout << "before header_value" << std::endl;
        //     std::cout << pos << std::endl;
        //     std::cout << lines[i] << std::endl;
        //     std::string header_value = lines[i].substr(pos + 2);
        //     std::cout << "after header_value" << std::endl;
        //     header_map_[header_name] = header_value;
        // }
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
            std::cout << "before push_back" << std::endl;
            lines.push_back(line.substr(0, line.size()-1));
            std::cout << "after push back" << std::endl;
        }
    }

    return lines;
}
