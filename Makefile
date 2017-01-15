all: server config_parser
	
server: server.cc config_parser.cc
	g++ -std=c++0x -o server server.cc server_main.cc config_parser.cc -lboost_system

config_parser: config_parser.cc config_parser_main.cc
	g++ config_parser.cc config_parser_main.cc -std=c++0x -g -Wall -o config_parser
