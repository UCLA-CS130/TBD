#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "config_parser.h"

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

#endif