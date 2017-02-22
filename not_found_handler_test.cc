#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "not_found_handler.h"

class NotFoundHandlerTest : public ::testing::Test {
protected:
    std::string GetName() {
        return not_found_handler_.GetName();
    }
    std::string HandleRequest() {
        Request* req = nullptr;
        Response response;
        not_found_handler_.HandleRequest(*req, &response);
        return response.ToString();
    }
    NotFoundHandler not_found_handler_;
};

TEST_F(NotFoundHandlerTest, GetNameCheck) {
    EXPECT_EQ("NotFoundHandler", GetName());
}

TEST_F(NotFoundHandlerTest, HandleNotFound) {
    EXPECT_EQ("HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 File Not Found\n",
            HandleRequest());
}
