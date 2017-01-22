#include <iostream>
#include <string>
#include <cstring>
#include "server.h"

using boost::asio::ip::tcp;

Server::Server(boost::asio::io_service& io_service, int port) 
    : io_service_(io_service), acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
}

Server::~Server() {}

void Server::start_accept() {
    Connection* new_connection = new Connection(io_service_);

    acceptor_.async_accept(new_connection->socket(),
                            boost::bind(&Server::handle_accept, this, new_connection,
                            boost::asio::placeholders::error));
}

void Server::handle_accept(Connection* new_connection, const boost::system::error_code& error) {
    if (!error) {
      new_connection->start();
    } else {
        delete new_connection;
    }

    start_accept();
}