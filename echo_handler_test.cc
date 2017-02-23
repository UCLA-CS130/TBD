#include "gtest/gtest.h"
#include "echo_handler.h"

class EchoHandlerTest : public ::testing::Test {
protected:
    std::string GetName() {
        return echo_handler_.GetName();
    }
    std::string HandleRequest(const std::string& raw_request) {
        auto request = Request::Parse(raw_request);
        Response response;
        echo_handler_.HandleRequest(*request, &response);
        return response.ToString();
    }
    EchoHandler echo_handler_;
};

TEST_F(EchoHandlerTest, GetNameCheck) {
    EXPECT_EQ("EchoHandler", GetName());
}

TEST_F(EchoHandlerTest, HandleEchoRequest) {
    EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nGET / HTTP/1.1",
            HandleRequest("GET / HTTP/1.1"));
}

