#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "reverse_proxy.h"

class ReverseProxyHandlerTest : public ::testing::Test {
protected:
    std::string GetName() {
        return reverse_proxy_handler_.GetName();
    }
    std::string HandleRequest() {
        Request* req = nullptr;
        Response response;
        reverse_proxy_handler_.HandleRequest(*req, &response);
        return response.ToString();
    }
    
    ReverseProxyHandler reverse_proxy_handler_;
}

TEST_F(ReverseProxyHandlerTest, GetNameCheck) {
    EXPECT_EQ("ReverseProxyHandler", GetName());
}

// TODO: do actual requests with reverse proxy and check responses with another
// webserver (std::system('./server path_config2"); ...



