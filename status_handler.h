#ifndef STATUS_HANDLER_H
#define STATUS_HANDLER_H

#include "request_handler.h"

class StatusHandler : public RequestHandler {
public:
    StatusHandler();
    virtual ~StatusHandler();
    virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);
    virtual Status HandleRequest(const Request& request, Response* response);
    virtual std::string GetName();
private:
    std::string CreateResponseBody();
};

REGISTER_REQUEST_HANDLER(StatusHandler);

#endif
