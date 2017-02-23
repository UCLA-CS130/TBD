#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "status_handler.h"

class StatusHandlerTest : public ::testing::Test {
protected:
    std::string GetName() {
        return status_handler_.GetName();
    }
    std::string HandleRequest() {
        auto request = Request::Parse("GET /status HTTP/1.1");
        Response response;
        status_handler_.HandleRequest(*request, &response);
        return response.ToString();
    }
    StatusHandler status_handler_;
};

TEST_F(StatusHandlerTest, GetNameCheck) {
    EXPECT_EQ("StatusHandler", GetName());
}

TEST_F(StatusHandlerTest, HandleStatusRequest) {
    EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nTotal number of requests: 0\n\nResponse code count: \n", HandleRequest());
}
