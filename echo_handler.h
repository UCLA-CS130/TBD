#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H
#include "request_handler.h"

class EchoHandler : public RequestHandler {
public:
    EchoHandler();
    virtual ~EchoHandler();
private:
};

#endif