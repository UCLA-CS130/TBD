#include "reverse_proxy_handler.h"

#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

ReverseProxyHandler::ReverseProxyHandler() {}

ReverseProxyHandler::~ReverseProxyHandler() {}

ReverseProxyHandler::Status ReverseProxyHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    uri_prefix_ = uri_prefix;

    // expecting one statement in child block: remote_host <remote_host>;
    bool remote_host_found = false;
    for (std::size_t i = 0; i < config.statements_.size(); i++) {
        if (config.statements_[i]->tokens_.size() >= 2 &&
            config.statements_[i]->tokens_[0] == "remote_host") {
            remote_host_ = config.statements_[i]->tokens_[1];
            remote_host_found = true;
        }
    }  
    if (!remote_host_found) {
        return INTERNAL_ERROR;
    }

    // remove any http protocol statements from start remote host
    if (remote_host_.size() > 7 &&
        remote_host_.substr(0, 7) == "http://") {
        remote_host_ = remote_host_.substr(7);
    }

    // separate the host and port from the uri part of the url
    std::size_t slash_location = remote_host_.find('/');
    if (slash_location != std::string::npos) { // if slash exists
        remote_uri_ = remote_host_.substr(remote_host_.find('/'));
        remote_host_ = remote_host_.substr(0, remote_host_.find('/'));
    } else {
        remote_uri_ = "";
    }

    // separate the hostname and port from the host
    std::size_t port_location = remote_host_.find(':');
    if (port_location != std::string::npos) {
        remote_port_ = remote_host_.substr(port_location+1);
        remote_host_ = remote_host_.substr(0, port_location);
    } else {
        remote_port_ = "80"; // default port number
    }

    return OK;
}

ReverseProxyHandler::Status ReverseProxyHandler::HandleRequest(const Request& request, Response* response) {
    using Headers = std::vector<std::pair<std::string, std::string>>;

    // copies made in case we need to redirect which server we are talking to
    std::string remote_host = remote_host_;
    std::string remote_uri  = remote_uri_;
    std::string remote_port = remote_port_;
    std::string request_uri;

    // check whether the request contains a referer header
    bool referer_flag = false;
    Headers req_headers = request.headers();
    for (std::size_t i = 0; i < req_headers.size(); i++) {
        if (req_headers[i].first == "Referer") {
            referer_flag = true;
            break;
        }
    }

    // get rid of the prefix if referer header is not contained in request
    if (!referer_flag) {
        request_uri = request.uri().substr(uri_prefix_.size());
    } else {
        request_uri = request.uri();
    }

    // keep looping while we find new hosts to connect to
    response->SetStatus(Response::REDIRECT);
    while (response->GetStatus() == Response::REDIRECT) {
        // connect to the new remote host
        boost::asio::io_service io_service;
        tcp::socket s(io_service);
        tcp::resolver resolver(io_service);
        boost::asio::connect(s, resolver.resolve({remote_host, remote_port}));

        // ensure that our uri is not blank so the request will be valid
        std::string uri = remote_uri + request_uri;
        if (uri == "") {
            uri = "/";
        }

        // construct the new request for the host by modifying the old one
        std::string new_req = request.method() + " " + uri + " " + request.version() + "\r\n";
        new_req += "Host: " + remote_host + ": " + remote_port + "\r\n";
        for (std::size_t i = 0; i < req_headers.size(); i++) {
            if (req_headers[i].first != "Host") {
                new_req += req_headers[i].first + ": " + req_headers[i].second + "\r\n";
            }
        }
        new_req += "\r\n";

        // send the new request to the host
        s.write_some(boost::asio::buffer(new_req, new_req.size()));

        // gather the response from the host
        std::string remote_response;
        while (1) {
            boost::array<char, 4096> buf;
            boost::system::error_code ec;
            size_t len = s.read_some(boost::asio::buffer(buf), ec);
            if (ec == boost::asio::error::eof) {
                break; // connection closed
            } else if (ec) {
                std::cout << "Error during read from remote web server\n";
                return Status::INTERNAL_ERROR;
            }
            remote_response += std::string(buf.data(), len);
        }

        // parse the remote response into the actual response data structure
        *response = *(Response::Parse(remote_response).get());

        // check if the remote host responded with a redirect
        if (response->GetStatus() == Response::REDIRECT ||
            response->GetStatus() == 300 ||
            response->GetStatus() == 301) {
            // find the location header indicating the new host
            remote_host = "";
            Headers res_headers = response->headers();
            for (std::size_t i = 0; i < res_headers.size(); i++) {
                if (res_headers[i].first == "Location") {
                    remote_host = res_headers[i].second;
                }
            }
            if (remote_host.empty()) {
                std::cout << "Received response code 302 without location\n";
                return Status::INTERNAL_ERROR;
            }

            // remove protocol from the start of remote host
            const std::string http_prefix = "http://";
            if (remote_host.substr(0, http_prefix.size()) != http_prefix) {
                std::cout << "Received a location that was not for http\n";
                return Status::INTERNAL_ERROR;
            } else {
                remote_host = remote_host.substr(http_prefix.size());
            }

            // separate the hostname and port from the host
            std::size_t port_location = remote_host.find(':');
            if (port_location != std::string::npos) {
                remote_port = remote_host.substr(port_location+1);
                remote_host = remote_host.substr(0, port_location);
            } else {
                remote_port = "80"; // default port number
            }

            // All these status responses redirect
            response->SetStatus(Response::REDIRECT);
        }
    }

    return Status::OK;
}

std::string ReverseProxyHandler::GetName() {
    return "ReverseProxyHandler";
}

