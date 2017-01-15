all: 
	g++ -std=c++0x -o server server.cc config_parser.cc -lboost_system -lboost_date_time -lboost_thread
