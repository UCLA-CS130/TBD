#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>
#include <vector>
#include <memory>
#include <utility>

// Represents an HTTP Request.
// 
// Usage:
// auto request = Request::Parse(raw_request);
class Request {
public:
    static unique_ptr<Request> Parse(const std::string& raw_request);

    std::string raw_request() const;
    std::string method() const;
    std::string uri() const;
    std::string version() const;

    using Headers = std::vector<std::pair<std::string, std::string>>;
    Headers headers() const;

    std::string body() const;

private:
    std::vector<std::string> split_lines(std::string request);

    std::string raw_request_;
    std::string method_;
    std::string uri_;
    std::string version_;
    Headers headers_;
    std::string body_;
};

// Represents an HTTP response.
// 
// Usage:
//     Response r;
//     r.SetStatus(RESPONSE_200);
//     r.SetBody(...);
//     return r.ToString();
// 
// Constructed by the RequestHandler, after which the server should call ToString
// to serialize.
class Response {
public:
    enum ResponseCode {
        RESPONSE_200 = 200;
        RESPONSE_404 = 404;
    };

    std::string to_string();
    void set_response_body(const std::string& response_body);
    void set_status_code(ResponseCode status_code);

    void set_mime_type(const std::string& mime_type);

private:
    std::string build_status_line();
    std::string build_header(std::string field, std::string value);
    
    std::string response_body_;
    std::string mime_type_ = "text/plain";
    int status_code_ = 404; // default to error
};

// Represents the parent of all request handlers. Implementations should expect to
// be long lived and created at server constrution.
class RequestHandler {
public:
    RequestHandler();
    virtual ~RequestHandler();
    virtual std::string build_response();
    static std::string build_status_line(int status_code);
    static std::string build_header(std::string field, std::string value);
};

#endif
