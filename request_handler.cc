#include "iostream"
#include "sstream"
#include "request_handler.h"

// Helper Function(s)

static std::vector<std::string> split_lines(std::string str) {
    std::stringstream ss(str);
    std::string line;
    std::vector<std::string> lines;

    // tokenize each line by newline
    while(std::getline(ss,line,'\n')) {
        // remove carriage return from line
        if (line[line.size()-1] == '\r') {
            lines.push_back(line.substr(0, line.size()-1));
        } else {
            lines.push_back(line.substr(0, line.size()));
        }
    }
    return lines;
}


// Request Implementations

std::unique_ptr<Request> Request::Parse(const std::string & raw_request) {
    std::unique_ptr<Request> new_request(new Request());
    new_request->raw_request_ = raw_request;
    
    std::vector<std::string> lines = split_lines(raw_request);
    for (size_t i = 0; i < lines.size(); i++) {
        if (i == 0) {
        // parse first line of the request
            auto first_space = lines[i].find(" ");
            new_request->method_ = lines[i].substr(0, first_space);
            std::string temp = lines[i].substr(first_space + 1);
            auto second_space = temp.find(" ");
            new_request->uri_ = temp.substr(0, second_space);
            new_request->version_ = temp.substr(second_space + 1);
        } else if (!lines[i].empty()) {
        // parse header fields of the request
            auto first_space = lines[i].find(" ");
            std::string field = lines[i].substr(0, first_space - 1);
            std::string value = lines[i].substr(first_space + 1);
            new_request->headers_.push_back(std::make_pair(field, value));
        }
    }
    
    return new_request;
}

std::string Request::raw_request() const {
    return raw_request_;
}

std::string Request::method() const {
    return method_;
}

std::string Request::uri() const {
    return uri_;
}

std::string Request::version() const {
    return version_;
}

std::string Request::body() const {
    return body_;
}

using Headers = std::vector<std::pair<std::string, std::string>>;
Headers Request::headers() const {
    return headers_;
}


// Response Implementations

std::unique_ptr<Response> Response::Parse(const std::string& raw_response) {
    // std::cout << "Raw response: " << raw_response << std::endl;
    std::unique_ptr<Response> new_response(new Response());
    std::vector<std::string> lines = split_lines(raw_response);
    // std::cout << "Splitted lines" << std::endl;

    // parse first line of the response
    if (lines.size() > 0) {
        auto first_space = lines[0].find(" ");
        std::string temp = lines[0].substr(first_space + 1);
        auto second_space = temp.find(" ");
        std::string status_code = temp.substr(0, second_space);
        try {
            new_response->status_code_ = std::stoi(status_code);
        } catch (...) {
            return std::unique_ptr<Response>();
        }
    }
    // std::cout << "Parsed first line" << std::endl;
    // parse header fields of the response
    unsigned int i = 1;
    while (i < lines.size() && !lines[i].empty()) {
        auto first_space = lines[i].find(" ");
        std::string field = lines[i].substr(0, first_space - 1);
        std::string value = lines[i].substr(first_space + 1);
        new_response->headers_.push_back(std::make_pair(field, value));
        i++;
    }
    // std::cout << "Parsed header fields" << std::endl;

    // parse body of the response
    i++;
    while (i < lines.size()) {
        new_response->response_body_ += lines[i];
        if (i < lines.size()-1) {
            new_response->response_body_ += "\n"; // avoid putting new line at end
        } 
        i++;
    }
    
    // std::cout << "Returning new response!" << std::endl;
    return new_response;
}

std::string Response::ToString() {
    std::string response = build_status_line();
    for (auto const& header: headers_)
        response += build_header_string(header);
    response += "\r\n" + response_body_;
    return response;
}

std::string Response::build_status_line() {
    std::string status_line = "HTTP/1.1";

    if (status_code_ == ResponseCode::OK) {
        status_line += " 200 OK";
    } else if (status_code_ == ResponseCode::FILE_NOT_FOUND) {
        status_line += " 404 Not Found";

    } else if (status_code_ == ResponseCode::FOUND) {
        status_line += " 302 Found";
    } else {
        status_line += " " + std::to_string(status_code_) + " Proxy";
    }

    return status_line + "\r\n";
}

std::string Response::build_header_string(const std::pair<std::string, std::string>& field) {
    return field.first + ": " + field.second + "\r\n";
}

void Response::SetBody(const std::string& body) {
    response_body_ = body;
}

void Response::SetStatus(int response_code) {
    status_code_ = response_code;
}

void Response::AddHeader(const std::string& header_name, const std::string& header_value) {
    headers_.push_back(std::make_pair(header_name, header_value));
}

int Response::GetStatus() {
    return status_code_;
}

std::string Response::body() const {
    return response_body_;
}

using Headers = std::vector<std::pair<std::string, std::string>>;
Headers Response::headers() const {
    return headers_;
}


// RequestHandler Implementations

RequestHandler::RequestHandler() {}

RequestHandler::~RequestHandler() {}


// RequestHandler Registerer

std::map<std::string, std::unique_ptr<RequestHandler> (*)(void)>* request_handler_builders = nullptr;

std::unique_ptr<RequestHandler> RequestHandler::CreateByName(const std::string type) {
  const auto type_and_builder = request_handler_builders->find(type);
  if (type_and_builder == request_handler_builders->end()) {
    return nullptr;
  }
  return (*type_and_builder->second)();
}

