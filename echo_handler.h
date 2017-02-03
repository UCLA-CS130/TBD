#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H
#include "request_handler.h"
#include "connection.h"

class EchoHandler : public RequestHandler {
public:
    EchoHandler(std::string request);
    virtual ~EchoHandler();
    virtual std::string build_response();
private:
    std::string request_;
};

#endif
