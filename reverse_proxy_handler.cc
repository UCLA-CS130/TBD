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

    bool remote_host_found = false;

    // expecting one statement in child block: remote_host <remote_host>;                                        
    if (config.statements_.size() == 1) {
        for (std::size_t i = 0; i < config.statements_.size(); i++) {
            if (config.statements_[i]->tokens_.size() != 2) {
                return INTERNAL_ERROR;
            } else if (config.statements_[i]->tokens_[0] == "remote_host") {
                remote_host_ = config.statements_[i]->tokens_[1];
                remote_host_found = true;
            } else {
                return INTERNAL_ERROR;
            }
        }
    } else {
        return INTERNAL_ERROR;
    }    

    if (remote_host_found) {
        std::size_t slash_location = remote_host_.find('/'); // if slash exists
        if (slash_location != std::string::npos) {
            remote_uri_ = remote_host_.substr(remote_host_.find('/'));
            remote_host_ = remote_host_.substr(0, remote_host_.find('/'));
        } else {
            remote_uri_ = "";
        }   
        
        std::size_t port_location = remote_host_.find(':');
        if (port_location != std::string::npos) {
            remote_port_ = remote_host_.substr(port_location+1);
            remote_host_ = remote_host_.substr(0, port_location);
        } else {
            remote_port_ = "80"; // default port number
        }
        std::cout << "Reverse Proxy Handler's settings:" << std::endl;
        std::cout << "\tRemote host: " << remote_host_ << std::endl;
        std::cout << "\tRemote uri: " << remote_uri_ << std::endl;
        std::cout << "\tRemote port: " << remote_port_ << std::endl;
        return OK;
    } else {
        return INTERNAL_ERROR;
    }
}

ReverseProxyHandler::Status ReverseProxyHandler::HandleRequest(const Request& request, Response* response) {
    

    // copies made in case we need to handle a 302 and change which server we are talking to
    std::string remote_host = remote_host_;
    std::string remote_uri = remote_uri_;
    std::string remote_port = remote_port_;
    std::string request_uri = request.uri();
    std::string request_uri_suffix = request_uri.substr(uri_prefix_.size());

    response->SetStatus(Response::FOUND);

    while(response->GetStatus() == Response::FOUND) {
        std::cout << "Starting iteration" << std::endl;

        std::cout << "\tRemote host: " << remote_host << std::endl;
        std::cout << "\tRemote uri: " << remote_uri << std::endl;
        std::cout << "\tRemote port: " << remote_port << std::endl;

        
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(remote_host, remote_port);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator iter = endpoint_iterator;
        tcp::resolver::iterator end; // End marker.
        while (iter != end)
        {
          tcp::endpoint endpoint = *iter++;
          std::cout << endpoint << std::endl;
        }

        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        std::cout << "Successfully connected!\n";

        std::string uri = remote_uri + request_uri_suffix;
        if (uri == "") {
            uri = "/";
        }

        std::string request_string = std::string("GET ") + uri + " HTTP/1.1\r\n" +
                                     "Host: " + remote_host + ":" + remote_port;

        request_string += std::string("\r\n") +
                          "Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n" +
                          "Accept-Encoding: gzip, deflate, sdch\r\n" +
                          "Accept-Language: en-US,en;q=0.8\r\n" +
                          "\r\n";
                               
                              


         // Send the response back to the client and then we're done
        socket.write_some(boost::asio::buffer(request_string, request_string.size()));

        std::string remote_response;

        while(1) {
          
            boost::array<char, 4096> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            //std::cout << "Got here!\n";
            if (error == boost::asio::error::eof) {
                std::cout <<"Exited cleanly\n";
                break; // Connection closed cleanly by peer.
            }
            else if (error)
                throw boost::system::system_error(error); // Some other error.


            
            //remote_response += std::string(std::begin(buf), std::end(buf));
            //std::cout.write(buf.data(), len);
            remote_response += std::string(buf.data(), len);

            
        }

        std::cout << "Got here!" << std::endl;
        std::cout << "Remote response size: " << remote_response << std::endl;
        *response = *(Response::Parse(remote_response).get());
        std::cout << "Got here too!" << std::endl;

        std::string content_length;
        using Headers = std::vector<std::pair<std::string, std::string>>;
        Headers headers = response->headers();
        for (std::size_t i = 0; i < headers.size(); i++) {
            std::cout << headers[i].first << ": " << headers[i].second << std::endl;
        }

        
        std::cout << "Actual Content-Length: " << response->body().size() << std::endl;

        if(response->GetStatus() == Response::FOUND) {

            // TODO: encapsulate in function so its not repeated
            for (std::size_t i = 0; i < headers.size(); i++) {
                if(headers[i].first == "Location") {
                    remote_host = headers[i].second;
                }
            }

            // Location will have http::// appended to front, need to remove
            const std::string http_prefix = "http://";
            if (remote_host.substr(0, http_prefix.size()) != http_prefix) {
                // TODO: error
            } else {
                remote_host = remote_host.substr(http_prefix.size());
            }

            std::size_t slash_location = remote_host.find('/'); // if slash exists
            if (slash_location != std::string::npos) {
                remote_uri = remote_host.substr(remote_host.find('/'));
                remote_host = remote_host.substr(0, remote_host.find('/'));
            } else {
                remote_uri = "";
            }   
            
            std::size_t port_location = remote_host.find(':');
            if (port_location != std::string::npos) {
                remote_port = remote_host.substr(port_location+1);
                remote_host = remote_host.substr(0, port_location);
            } else {
                remote_port = "80"; // default port number
            }
            request_uri_suffix = "";
        }

    }

    return Status::OK;
}

std::string ReverseProxyHandler::GetName() {
    return "ReverseProxyHandler";
}

