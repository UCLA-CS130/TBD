#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <boost/asio.hpp>
#include <unordered_map>
#include "config_parser.h"
#include "request_handler.h"

class Server {
public:
	Server(boost::asio::io_service& io_service, NginxConfig* config);
    virtual ~Server();
    void run();
    void handle_request(boost::asio::ip::tcp::socket* socket);
    std::string handle_read(const char* data);
    void create_handler_map(NginxConfig* config);
    void init_status_counter();
    std::string find_uri_prefix(std::string uri);
    bool is_uri_prefix(std::string short_str, std::string long_str);
private:
    static const int MAX_LENGTH = 2048;
    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    NginxConfig* config_;
    std::unordered_map<std::string, std::unique_ptr<RequestHandler>> handler_map_;
};

#endif
