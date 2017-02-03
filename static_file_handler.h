#ifndef STATIC_FILE_HANDLER_H
#define STATIC_FILE_HANDLER_H
#include "request_handler.h"

class StaticFileHandler : public RequestHandler {
public:
    StaticFileHandler();
    virtual ~StaticFileHandler();
private:
};

#endif