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
    EXPECT_STREQ("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Encoding: gzip\r\n\r\n\x1F\x8B\b\0\0\0\0\0\x2\xFFsw\rQ\xD0O\xCB\xCFW\xF0\b\t\t\xD0\" \"7\xD4\" \"3\xE4\xE5\xF2\xC8/.\xB1R\xC8\xC9ON\xCC\xC9\01-\f,\fx\xB9x\xB9\0;\xEF(\x9D+\0\0\0",
            HandleRequest("GET / HTTP/1.1").c_str());
}

