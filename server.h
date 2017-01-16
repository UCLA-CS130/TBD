#include <iostream>
#include "config_parser.h"
#include <boost/asio.hpp>

class Server {
	public:
		Server() {};

		bool runServer(const char* filename);	

    private:
        NginxConfigParser configParser;
        NginxConfig config;
        boost::asio::io_service io_service;
};