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
CXXFLAGS += -g -Wall -Wextra -Werror -static-libgcc -static-libstdc++ -pthread -Wl,-Bstatic -lm -std=c++0x

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


.PHONY: clean deploy

all: server

clean:
	rm -f config_parser server *_test *.o *.a *.gcno *.gcda *.gcov

deploy:
	docker build -t httpserver.build . ; \
	docker run httpserver.build > ./deploy/binary.tar; tar -xvf ./deploy/binary.tar -C ./deploy; \
	rm -f ./deploy/binary.tar; docker build -t httpserver -f deploy/Dockerfile.run ./deploy; \
	docker save httpserver | bzip2 | ssh -i cs130.pem ubuntu@ec2-54-202-151-253.us-west-2.compute.amazonaws.com 'bunzip2 | docker load; exit;'

server: server_main.o server.o config_parser.o echo_handler.o request_handler.o static_file_handler.o not_found_handler.o \
		status_counter.o status_handler.o reverse_proxy_handler.o redirect_handler.o
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

$(basename $(wildcard %.o)) : %.cc
	$(CXX) $(CXXFLAGS) -c %.cc

$(basename $(wildcard %_test.o)) : %_test.cc $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c %_test.cc

request_handler_test : request_handler_test.o request_handler.o echo_handler.o static_file_handler.o not_found_handler.o status_handler.o status_counter.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

echo_handler_test : echo_handler_test.o echo_handler.o request_handler.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

server_test : config_parser.o server.o server_test.o request_handler.o echo_handler.o  static_file_handler.o \
				 status_counter.o status_handler.o not_found_handler.o gmock_main.a reverse_proxy_handler.o redirect_handler.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

config_parser_test : config_parser.o config_parser_test.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

static_file_handler_test : static_file_handler.o static_file_handler_test.o request_handler.o not_found_handler.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

not_found_handler_test : not_found_handler_test.o not_found_handler.o request_handler.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

status_handler_test : status_handler_test.o status_handler.o request_handler.o status_counter.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

status_counter_test : status_counter_test.o status_counter.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

redirect_handler_test : redirect_handler_test.o redirect_handler.o request_handler.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

reverse_proxy_handler_test : reverse_proxy_handler_test.o reverse_proxy_handler.o request_handler.o config_parser.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -lboost_system

TESTS = config_parser_test server_test echo_handler_test \
		request_handler_test static_file_handler_test \
		not_found_handler_test status_handler_test \
		redirect_handler_test reverse_proxy_handler_test
test : $(TESTS)
	for t in $^ ; do ./$$t ; done

coverage : CXXFLAGS += -fprofile-arcs -ftest-coverage
coverage : test
	gcov -r server.cc; gcov -r config_parser.cc; \
	gcov -r echo_handler.cc; gcov -r request_handler.cc; \
	gcov -r static_file_handler.cc; gcov -r status_handler.cc; \
	gcov -r not_found_handler.cc; gcov -r status_counter.cc;

integration : 
	make clean && make;
	python server_integration_test.py;
	python multithreading_test.py;
