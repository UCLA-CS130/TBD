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
    response->SetBody(CreateResponseBody());
    return OK;
}

std::string StatusHandler::GetName() {
    return "StatusHandler";
}

std::string StatusHandler::CreateResponseBody() {
    std::string response_body = "Total number of requests: " +
                                std::to_string(StatusCounter::get_instance().request_count_) + "\n\n";
    for (auto it = StatusCounter::get_instance().handler_name_map_.begin();
            it != StatusCounter::get_instance().handler_name_map_.end(); it++) {
        response_body += it->first + " " + it->second + "\n\n";
    }

    response_body += "Response code count: \n";
    for (auto it = StatusCounter::get_instance().status_code_map_.begin();
            it != StatusCounter::get_instance().status_code_map_.end(); it++) {
        response_body += it->first + "\n";

        for (auto i = it->second.begin(); i != it->second.end(); i++) {
            response_body += "\t" + std::to_string(i->first) + ": " + std::to_string(i->second) + "\n";
        }
        response_body += "\n";
    }

    return response_body;
}
