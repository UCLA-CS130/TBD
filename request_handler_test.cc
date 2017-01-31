#include "gtest/gtest.h"
#include "request_handler.h"

TEST(RequestHandlerTest, BuildStatusLineOK) {
    RequestHandler request_handler;
    EXPECT_EQ("HTTP/1.1 200 OK\r\n", request_handler.build_status_line(200));
}

TEST(RequestHandlerTest, BuildStatusLineNotFound) {
    RequestHandler request_handler;
    EXPECT_EQ("HTTP/1.1 404 Not Found\r\n", request_handler.build_status_line(404));
}

TEST(RequestHandlerTest, BuildHeaderTest) {
    RequestHandler request_handler;
    EXPECT_EQ("Content-type: text/plain\r\n", request_handler.build_header("Content-type", "text/plain"));
}
