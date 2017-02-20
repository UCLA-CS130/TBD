#ifndef NOT_FOUND_HANDLER_H
#define NOT_FOUND_HANDLER_H

#include "request_handler.h"

class NotFoundHandler : public RequestHandler {
public:
    NotFoundHandler();
    virtual ~NotFoundHandler();
    virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);
    virtual Status HandleRequest(const Request& request, Response* response);
};

//REGISTER_REQUEST_HANDLER(NotFoundHandler);

#endif
