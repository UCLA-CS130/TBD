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
    std::string expected_response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nGET /foo HTTP/1.1\r\nHost: localhost:8080\r\n\r\n";
    std::string actual_response = handle_read("GET /foo HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
    EXPECT_EQ(expected_response, actual_response);
}
