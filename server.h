#include <iostream>
#include <string>
#include "config_parser.h"
#include <boost/asio.hpp>

class Server {
	public:
		Server() {};
		bool runServer(const char* filename);	
        std::string getConfigValue(std::string configName);
    private:
        NginxConfigParser configParser;
        NginxConfig config;
        boost::asio::io_service io_service;
};