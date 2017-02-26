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
    bool remote_port_found = false;

    // expecting two statements in child block: remote_host <remote_host>;
    //                                          remote_port <remote_port>;
    if (config.statements_.size() == 2) {
        for (std::size_t i = 0; i < config.statements_.size(); i++) {
            if (config.statements_[i]->tokens_.size() != 2) {
                return INTERNAL_ERROR;
            } else if (config.statements_[i]->tokens_[0] == "remote_host") {
                remote_host_ = config.statements_[i]->tokens_[1];
                remote_host_found = true;
            } else if (config.statements_[i]->tokens_[0] == "remote_port") {
                remote_port_ = std::stoi(config.statements_[i]->tokens_[1]);
                remote_port_found = true;
            } else {
                return INTERNAL_ERROR;
            }
        }
    } else {
        return INTERNAL_ERROR;
    }    

    if (remote_host_found && remote_port_found) {
        return OK;
    } else {
        return INTERNAL_ERROR;
    }
}

ReverseProxyHandler::Status ReverseProxyHandler::HandleRequest(const Request& request, Response* response) {
    // TODO: Implement reverse proxy request



    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(remote_host_, std::to_string(remote_port_));
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

    std::string request_string = std::string("GET /echo HTTP/1.1\r\n") +
                          "Host: " + remote_host_ +":" + std::to_string(remote_port_) + "\r\n" +
                          "Connection: keep-alive\r\n" +
                          "Accept-Encoding: gzip, deflate, sdch, br\r\n" +
                          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n" +
                          "\r\n";


     // Send the response back to the client and then we're done
    socket.write_some(boost::asio::buffer(request_string, request_string.size()));

    std::string remote_response;

    while(1)
    {
      
        boost::array<char, 128> buf;
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
        std::cout.write(buf.data(), len);
        remote_response += std::string(buf.data(), len);

        
    }

    std::cout << remote_response << std::endl;
    //std::unique_ptr<Request> response_as_request = Request::Parse(remote_response);

    response->SetStatus(Response::ResponseCode::OK);



    response->AddHeader("Content-Type", "text/plain");
    response->SetBody(request.raw_request());
    response->SetBody(remote_response);
    return Status::OK;
}

std::string ReverseProxyHandler::GetName() {
    return "ReverseProxyHandler";
}

