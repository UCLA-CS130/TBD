#include "gtest/gtest.h"
#include "echo_handler.h"

TEST(EchoHandlerTest, ConstructResponseTest) {
    EchoHandler echo_handler("GET / HTTP/1.1");
    EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nGET / HTTP/1.1",
                echo_handler.constructResponse());
}
