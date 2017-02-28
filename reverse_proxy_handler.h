#ifndef REVERSE_PROXY_HANDLER_H
#define REVERSE_PROXY_HANDLER_H

#include "request_handler.h"

class ReverseProxyHandler : public RequestHandler {
public:
    ReverseProxyHandler();
    virtual ~ReverseProxyHandler();
    virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);
    virtual Status HandleRequest(const Request& request, Response* response);
    virtual std::string GetName();
private:
    std::string uri_prefix_;
    std::string remote_host_;
    int remote_port_;
};

REGISTER_REQUEST_HANDLER(ReverseProxyHandler);

#endif

