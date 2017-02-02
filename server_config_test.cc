#include "gtest/gtest.h"
#include "server_config.h"

TEST(ServerConfigTest, GetPortTest) {
    NginxConfigParser parser;
    NginxConfig config;
    parser.Parse("example_config", &config);

    ServerConfig server_config(&config);
    EXPECT_EQ(8080, server_config.get_port());
}
