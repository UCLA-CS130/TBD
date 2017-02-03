#include "gtest/gtest.h"
#include "server_config.h"

class ServerConfigTest : public ::testing::Test {
    protected:
        void parseConfig(const std::string config_string, NginxConfig* out_config) {
            std::stringstream config_stream(config_string);
            parser.Parse(&config_stream, out_config);
        }

        NginxConfigParser parser;
        NginxConfig out_config;
};

TEST_F(ServerConfigTest, GetPortTest) {
    parseConfig("listen 8081;", &out_config);

    ServerConfig server_config(&out_config);
    EXPECT_EQ(8081, server_config.get_port());
}

TEST_F(ServerConfigTest, MissingPortTest) {
    parseConfig("", &out_config);

    ServerConfig server_config(&out_config);
    EXPECT_EQ(8080, server_config.get_port());
}

TEST_F(ServerConfigTest, GetPathMapTest) {
    parseConfig("path { static1 /home/;\n static2 /other/dir/;\n }", &out_config);

    ServerConfig server_config(&out_config);
    EXPECT_EQ(2, server_config.get_path_map().size());
    EXPECT_EQ("/home/", server_config.get_path_map()["static1"]);
    EXPECT_EQ("/other/dir/", server_config.get_path_map()["static2"]);
}

TEST_F(ServerConfigTest, MissingPathMapTest) {
    parseConfig("", &out_config);

    ServerConfig server_config(&out_config);
    EXPECT_EQ(1, server_config.get_path_map().size());
    EXPECT_EQ("./", server_config.get_path_map()["root"]);
}
