# Sources:
# https://github.com/google/googlemock/blob/master/googlemock/make/Makefile
# https://github.com/google/googletest/blob/master/googletest/make/Makefile


# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR=googletest/googletest
GMOCK_DIR=googletest/googlemock

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -Werror -pthread -std=c++0x

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
				$(GTEST_DIR)/include/gtest/internal/*.h


GMOCK_HEADERS = $(GMOCK_DIR)/include/gmock/*.h \
                $(GMOCK_DIR)/include/gmock/internal/*.h \
				$(GTEST_HEADERS)

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
GMOCK_SRCS_ = $(GMOCK_DIR)/src/*.cc $(GMOCK_HEADERS)


all: server

clean:
	rm -f config_parser config_parser_test server server_test connection_test *.o *.a *.gcno *.gcda *.gcov

server: server_main.o server.o config_parser.o connection.o echo_handler.o request_handler.o server_config.o
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

config_parser: config_parser.cc config_parser_main.cc
	g++ config_parser.cc config_parser_main.cc -std=c++0x -g -Wall -o config_parser

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

gmock-all.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
			-c $(GMOCK_DIR)/src/gmock-all.cc

gmock_main.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
            -c $(GMOCK_DIR)/src/gmock_main.cc

gmock.a : gmock-all.o gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gmock_main.a : gmock-all.o gtest-all.o gmock_main.o
	$(AR) $(ARFLAGS) $@ $^

request_handler.o : request_handler.cc
	$(CXX) $(CXXFLAGS) -c request_handler.cc

request_handler_test.o : request_handler_test.cc $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c request_handler_test.cc

request_handler_test : request_handler_test.o request_handler.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

echo_handler.o : echo_handler.cc
	$(CXX) $(CXXFLAGS) -c echo_handler.cc

echo_handler_test.o : echo_handler_test.cc $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c echo_handler_test.cc

echo_handler_test : echo_handler_test.o echo_handler.o request_handler.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

server_config.o : server_config.cc
	$(CXX) $(CXXFLAGS) -c server_config.cc

server_config_test.o : server_config_test.cc $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c server_config_test.cc

server_config_test : server_config_test.o server_config.o config_parser.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

connection.o : connection.cc
	$(CXX) $(CXXFLAGS) -c connection.cc

connection_test.o : connection_test.cc $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c connection_test.cc

connection_test : connection_test.o connection.o request_handler.o echo_handler.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

server_main.o : server_main.cc
	$(CXX) $(CXXFLAGS) -c server_main.cc

server.o : server.cc
	$(CXX) $(CXXFLAGS) -c server.cc

server_test.o : server_test.cc $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c server_test.cc

server_test : config_parser.o server.o server_test.o connection.o request_handler.o echo_handler.o server_config.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

config_parser.o : config_parser.cc
	$(CXX) $(CXXFLAGS) -c config_parser.cc

config_parser_test.o : config_parser_test.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c config_parser_test.cc

config_parser_test : config_parser.o config_parser_test.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

test : config_parser_test server_test connection_test echo_handler_test request_handler_test server_config_test
	./config_parser_test; ./server_test; ./connection_test; ./echo_handler_test; ./request_handler_test; ./server_config_test

coverage : CXXFLAGS += -fprofile-arcs -ftest-coverage
coverage : server_test connection_test
	./server_test && gcov -r server.cc; ./connection_test && gcov -r connection.cc

integration : 
	python server_integration_test.py
