#include "status_handler.h"
#include "status_counter.h"

StatusHandler::StatusHandler() {}

StatusHandler::~StatusHandler() {}

StatusHandler::Status StatusHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    // To suppress unused param warning
    (void) uri_prefix;
    (void) config;
    return OK;
}

StatusHandler::Status StatusHandler::HandleRequest(const Request& request, Response* response) {
    (void) request;
    
    response->SetStatus(Response::ResponseCode::OK);
    response->AddHeader("Content-Type", "text/plain");
    response->SetBody(std::to_string(StatusCounter::get_instance().request_count_));
    return OK;
}
