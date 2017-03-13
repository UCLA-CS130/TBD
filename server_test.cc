#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "server.h"

class ServerTest : public ::testing::Test {
protected:
    std::string handle_read(const char* data) {
        NginxConfigParser parser;
        NginxConfig config;
        parser.Parse("path_config", &config);

        boost::asio::io_service io_service;
        //ServerConfig server_config(&config);
        //Server server(io_service, &server_config);
        Server server(io_service, &config);
        return server.handle_read(data);
    }
};

TEST_F(ServerTest, HandleReadSimpleSuccess) {
    std::string expected_response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Encoding: gzip\r\n\r\n\x1F\x8B\b\0\0\0\0\0\x2\xFFsw\rQ\xD0O\xCB\xCFW\xF0\b\t\t\xD0\" \"7\xD4\" \"3\xE4\xE5\xF2\xC8/.\xB1R\xC8\xC9ON\xCC\xC9\01-\f,\fx\xB9x\xB9\0;\xEF(\x9D+\0\0\0";
    std::string actual_response = handle_read("GET /foo HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
    EXPECT_STREQ(expected_response.c_str(), actual_response.c_str());
}
