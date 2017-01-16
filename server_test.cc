#include <string>
#include "gtest/gtest.h"
#include "config_parser.h"
#include "server.h"

TEST(ServerTest, NoThrow) {
	Server server;
    std::string filename = "example_config";
	EXPECT_NO_THROW(server.runServer(filename.c_str()));
}