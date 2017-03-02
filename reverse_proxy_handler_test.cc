#include <cstdlib>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "config_parser.h"
#include "reverse_proxy_handler.h"

class ReverseProxyHandlerTest : public ::testing::Test {
protected:
    bool Init() {
        int fd;
        NginxConfigParser parser;
        char name_config[] = "/tmp/configXXXXXX";
        std::string config_contents = "remote_host localhost:8081;";
        bool is_parsed = false;
        bool is_handler_initialized = false;

        // create a temporary config file with reverse proxy child block
        fd = mkstemp(name_config);
        write(fd, config_contents.c_str(), config_contents.size());
  
        // parse the config file and check if it succeeded before deleting
        is_parsed = parser.Parse(name_config, &config_);
        remove(name_config);

        // initialize the proxy handler and check if it succeeded
        if (reverse_proxy_handler_.Init("/simple_proxy", config_) == ReverseProxyHandler::Status::OK) {
            is_handler_initialized = true;
        }

        return (is_parsed && is_handler_initialized);
    }
    std::string GetName() {
        return reverse_proxy_handler_.GetName();
    }
    std::string HandleRequest(const std::string& raw_request) {
        auto request = Request::Parse(raw_request);
        Response response;
        reverse_proxy_handler_.HandleRequest(*request, &response);
        return response.ToString();
    }
    NginxConfig config_;
    ReverseProxyHandler reverse_proxy_handler_;
};

TEST_F(ReverseProxyHandlerTest, GetNameCheck) {
    EXPECT_EQ("ReverseProxyHandler", GetName());
}

TEST_F(ReverseProxyHandlerTest, ValidProxyRequest) {
    // Run separate webserver to send requests to from reverse proxy
    std::system("./server path_config2 &");
    ASSERT_TRUE(Init());
    EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nGET /echo HTTP/1.1\r\nHost: localhost: 8081\r\n\r\n", HandleRequest("GET /simple_proxy/echo HTTP/1.1"));
}

