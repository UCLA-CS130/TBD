/* Source:
    http://www.boost.org/doc/libs/1_53_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_server.cpp
*/

#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <utility>
#include "server.h"
#include "status_counter.h"

using boost::asio::ip::tcp;

Server::Server(boost::asio::io_service& io_service, NginxConfig* config)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), std::stoi(config->getConfigValue("port")))),
      config_(config) {
    create_handler_map(config_);
}

Server::~Server() {}

// Start accepting requests
void Server::run() {
    while (true) {
        tcp::socket socket(io_service_);
        acceptor_.accept(socket);

        char data[MAX_LENGTH];
        memset(data, 0, MAX_LENGTH);
        boost::system::error_code error;
        socket.read_some(boost::asio::buffer(data), error);

        if (error) {
            std::cout << "Error reading request" << std::endl;
        } else {
            std::string message = handle_read(data);
            boost::asio::write(socket, boost::asio::buffer(message, message.size()));
        }
        socket.close();
    }
}

std::string Server::handle_read(const char* data) {
    std::string longest_prefix;
    std::string data_string = std::string(data);
    auto request = Request::Parse(data_string);
    Response response;


    // check if request has a Referer field
    using Headers = std::vector<std::pair<std::string, std::string>>;
    Headers headers = request->headers();

    std::string referer_url;
    std::cout << "\nRequest:" << request->uri() << "; headers:" << std::endl;
    for (std::size_t i = 0; i < headers.size(); i++) {
        if(headers[i].first == "Referer") {
            referer_url = headers[i].second;
        }
        std::cout << "\t" << headers[i].first << ": " << headers[i].second << std::endl;
    }
    // if the field exists, we use that for longest prefix
    if (referer_url != "") {
        std::size_t slash_pos = referer_url.find('/');
        slash_pos = referer_url.find('/', slash_pos+1);
        slash_pos = referer_url.find('/', slash_pos+1); //find third slash
        std::string referer_uri = referer_url.substr(slash_pos);
        std::cout << "Referer url: " << referer_url << "\t" << "Referer uri: " << referer_uri << std::endl;
        longest_prefix = find_uri_prefix(referer_uri);
        if (longest_prefix == "default") {//if its referring of a reference, won't have a prefix, try every single one?
            for ( auto it = handler_map_.begin(); it != handler_map_.end(); it++ ) {
                if (it->second->GetName() == "ReverseProxyHandler") {
                    std::cout << "Trying a reverseproxyhanlder!" << std::endl;
                    RequestHandler::Status status1 = it->second->HandleRequest(*request, &response);

                    if (status1  == RequestHandler::OK ) {
                        std::cout << "Got a good response!!" << std::endl;
                        return response.ToString();
                    }
                }
            }
        }


    } else {
        longest_prefix = find_uri_prefix(request->uri());
    } 
    std::cout << "longest prefix: " << longest_prefix << std::endl;
    RequestHandler::Status status = handler_map_[longest_prefix]->HandleRequest(*request, &response);

    // update status counter
    StatusCounter::get_instance().request_count_++;
    StatusCounter::get_instance().status_code_map_[request->uri()][response.GetStatus()]++;

    if (status == RequestHandler::OK) {
        std::cout << "Handle request returned status OK." << std::endl;
    } else if (status == RequestHandler::FILE_NOT_FOUND) {
        std::cout << "Handle request returned status FILE_NOT_FOUND." << std::endl;

        if (handler_map_[longest_prefix]->GetName() != "NotFoundHandler")
            handler_map_["default"]->HandleRequest(*request, &response);
    }
    std::cout << "Returning response" << std::endl;

    return response.ToString();
}

void Server::create_handler_map(NginxConfig* config) {
    std::vector<std::shared_ptr<NginxConfigStatement>> statements = config->statements_;
    for (size_t i = 0; i < statements.size(); i++) {
        if (statements[i]->tokens_[0] == "path") {
            std::string uri_prefix = statements[i]->tokens_[1];
            handler_map_[uri_prefix] = RequestHandler::CreateByName(statements[i]->tokens_[2]);

            RequestHandler::Status status = handler_map_[uri_prefix]->Init(uri_prefix, *(statements[i]->child_block_));
            if (status == RequestHandler::Status::INTERNAL_ERROR) {
                std::cout << "Bad config file. Server exiting." << std::endl;
                exit(1);
            }
        } else if (statements[i]->tokens_[0] == "default") {
            handler_map_["default"] = RequestHandler::CreateByName("NotFoundHandler");
        }
    }

    init_status_counter();
}

void Server::init_status_counter() {
    if (handler_map_.find("/status") != handler_map_.end()) {
        for (auto it = handler_map_.begin(); it != handler_map_.end(); it++) {
            std::string uri_prefix = it->first;
            std::string handler_name = it->second->GetName();
            StatusCounter::get_instance().handler_name_map_[uri_prefix] = handler_name;
        }
    }
}

// Finds the correct handler using longest uri prefix matching
std::string Server::find_uri_prefix(std::string request_uri) {
    std::string longest_prefix_match = "";
    for (auto it = handler_map_.begin(); it != handler_map_.end(); it++) {
        std::string prefix = it->first;
        if (is_uri_prefix(prefix, request_uri) &&
            prefix.size() > longest_prefix_match.size()) {
            longest_prefix_match = prefix;
        }
    }

    if (longest_prefix_match == "")
        longest_prefix_match = "default";

    return longest_prefix_match;
}

// Returns true if short_str is a uri prefix of long_str
// e.g. /foo is a uri prefix of /foo/boo, but not of /fooo
bool Server::is_uri_prefix(std::string short_str, std::string long_str) {
    if (short_str.size() > long_str.size()) return false;

    if ((long_str.substr(0, short_str.size()) == short_str) &&
        (short_str == long_str || long_str[short_str.size()] == '/')) {
        return true;
    } else {
        return false;
    }
}
