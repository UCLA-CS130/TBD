#include "gtest/gtest.h"
#include "server_config.h"

TEST(ServerConfigTest, GetPortTest) {
    NginxConfigParser parser;
    NginxConfig config;
    parser.Parse("example_config", &config);

    ServerConfig server_config(&config);
    EXPECT_EQ(8080, server_config.get_port());
}

TEST(ServerConfigTest, GetPathMapTest) {
    NginxConfigParser parser;
    NginxConfig config;
    parser.Parse("path_config", &config);

    ServerConfig server_config(&config);
    EXPECT_EQ("/home/", server_config.get_path_map()["static1"]);
    EXPECT_EQ("/other/dir/", server_config.get_path_map()["static2"]);
    EXPECT_EQ(2, server_config.get_path_map().size());
}
