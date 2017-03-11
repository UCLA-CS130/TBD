#ifndef STATIC_FILE_HANDLER_H
#define STATIC_FILE_HANDLER_H

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "request_handler.h"

class StaticFileHandler : public RequestHandler {
public:
    StaticFileHandler();
    virtual ~StaticFileHandler();
    std::string GetMimeType(const std::string& file_path, bool &isMarkdown);
    bool ReadFile(const std::string& file_path, std::string& file_content);
    virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);
    virtual Status HandleRequest(const Request& request, Response* response);
    std::string ProcessMarkdown(const std::string& content);
    virtual std::string GetName();
private:
    std::string uri_prefix_;
    std::string root_path_;
};

REGISTER_REQUEST_HANDLER(StaticFileHandler);

#endif
