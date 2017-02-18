#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include "request_handler.h"

class EchoHandler : public RequestHandler {
public:
    EchoHandler();
    virtual ~EchoHandler();
    virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);
    virtual Status HandleRequest(const Request& request, Response* response);
};

//REGISTER_REQUEST_HANDLER(EchoHandler);

#endif
