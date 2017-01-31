#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H
#include "connection.h"

class RequestHandler {
public:
    RequestHandler() {};
    virtual ~RequestHandler() {};
    static std::string build_status_line(int status_code);
    static std::string build_header(std::string field, std::string value);
};

#endif
