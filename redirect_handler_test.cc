#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "redirect_handler.h"

class RedirectHandlerTest : public ::testing::Test {
protected:
    std::string GetName() {
        return redirect_handler_.GetName();
    }
    std::string HandleRequest() {
        Request* req = nullptr;
        Response response;
        redirect_handler_.HandleRequest(*req, &response);
        return response.ToString();
    }
    RedirectHandler redirect_handler_;
};

TEST_F(RedirectHandlerTest, GetNameCheck) {
    EXPECT_EQ("RedirectHandler", GetName());
}

TEST_F(RedirectHandlerTest, HandlRedirect) {
    EXPECT_EQ("HTTP/1.1 302 Found\r\nLocation: \r\n\r\n",
            HandleRequest());
}
