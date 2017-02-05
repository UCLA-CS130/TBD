#ifndef STATIC_FILE_HANDLER_H
#define STATIC_FILE_HANDLER_H

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "request_handler.h"
#include "http_request.h"

class StaticFileHandler : public RequestHandler {
public:
    StaticFileHandler(HttpRequest* http_request);
    virtual ~StaticFileHandler();
    virtual std::string build_response();
    virtual std::string get_mime_type();
    virtual std::string read_file();
private:
    std::string path_;
};

#endif