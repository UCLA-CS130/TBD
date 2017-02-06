#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <unordered_map>
#include <vector>

class HttpRequest {
public:
    HttpRequest(std::string request);
    void parse_request(std::string request);
    std::vector<std::string> split_lines(std::string request);
    
    std::string request_path_;
    std::string raw_request_string_;
};

#endif
