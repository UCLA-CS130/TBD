#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H
#include "request_handler.h"
#include "connection.h"

class EchoHandler : public RequestHandler {
public:
    EchoHandler(char* request);
    virtual ~EchoHandler();
    virtual std::string constructResponse();
private:
    char* request_;
};

#endif
