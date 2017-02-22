#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

// Represents the parent of all request handlers. Implementations should expect to
// be long lived and created at server constrution.

// #include "request.h"
// #include "response.h"
#include "config_parser.h"
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <map>

// Represents an HTTP Request.
// 
// Usage:
// auto request = Request::Parse(raw_request);
class Request {
public:
    static std::unique_ptr<Request> Parse(const std::string& raw_request);

    std::string raw_request() const;
    std::string method() const;
    std::string uri() const;
    std::string version() const;
    std::string body() const;

    using Headers = std::vector<std::pair<std::string, std::string>>;
    Headers headers() const;
private:
    static std::vector<std::string> split_lines(std::string request);

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
        OK = 200,
        FILE_NOT_FOUND = 404
    };

    void SetStatus(const ResponseCode response_code);
    void AddHeader(const std::string& header_name, const std::string& header_value);
    void SetBody(const std::string& body);
    int GetStatus();

    std::string ToString();

private:
    std::string build_status_line();
    std::string build_header_string(const std::pair<std::string, std::string>& field);
    
    std::string response_body_;
    ResponseCode status_code_;
    
    using Headers = std::vector<std::pair<std::string, std::string>>;
    Headers headers_;
};

// Represents the parent of all request handlers. Implementations should expect to
// be long lived and created at server constrution.
class RequestHandler {
 public:
  RequestHandler();
  virtual ~RequestHandler();

  enum Status {
    OK = 0,
    FILE_NOT_FOUND = 404
  };
  
  // Initializes the handler. Returns a response code indicating success or
  // failure condition.
  // uri_prefix is the value in the config file that this handler will run for.
  // config is the contents of the child block for this handler ONLY.
  virtual Status Init(const std::string& uri_prefix, const NginxConfig& config) = 0;

  // Handles an HTTP request, and generates a response. Returns a response code
  // indicating success or failure condition. If ResponseCode is not OK, the
  // contents of the response object are undefined, and the server will return
  // HTTP code 500.
  virtual Status HandleRequest(const Request& request, Response* response) = 0;

  static std::unique_ptr<RequestHandler> CreateByName(const std::string type);
  virtual std::string GetName() = 0;
};

extern std::map<std::string, std::unique_ptr<RequestHandler> (*)(void)>* request_handler_builders;
template<typename T>
class RequestHandlerRegisterer {
 public:
  RequestHandlerRegisterer(const std::string& type) {
    if (request_handler_builders == nullptr) {
      request_handler_builders = new std::map<std::string, std::unique_ptr<RequestHandler> (*)(void)>;
    }
    (*request_handler_builders)[type] = RequestHandlerRegisterer::Create;
  }
  static std::unique_ptr<RequestHandler> Create() {
    return std::unique_ptr<RequestHandler>(new T);
  }
};

#define REGISTER_REQUEST_HANDLER(ClassName) \
   static RequestHandlerRegisterer<ClassName> ClassName##__registerer(#ClassName)

#endif
