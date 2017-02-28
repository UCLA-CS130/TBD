#ifndef REDIRECT_HANDLER_H
#define REDIRECT_HANDLER_H

#include "request_handler.h"

class RedirectHandler : public RequestHandler {
public:
    RedirectHandler();
    virtual ~RedirectHandler();
    virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);
    virtual Status HandleRequest(const Request& request, Response* response);
    virtual std::string GetName();

private:
    std::string url_; // url that the server will redirect the user to

};

REGISTER_REQUEST_HANDLER(RedirectHandler);

#endif

