#include "gtest/gtest.h"
#include "request_handler.h"

// Request Tests

TEST(RequestTest, ParseRequestTest) {
    std::string raw = "GET /foo HTTP/1.1\r\nUser-Agent: curl/7.35.0\r\n";
    auto request = Request::Parse(raw);

    EXPECT_EQ(raw, request->raw_request());
    EXPECT_EQ("GET", request->method());
    EXPECT_EQ("/foo", request->uri());
    EXPECT_EQ("HTTP/1.1", request->version());
    EXPECT_EQ("", request->body());
    EXPECT_EQ(1, request->headers().size());
    EXPECT_EQ("User-Agent", request->headers().at(0).first);
    EXPECT_EQ("curl/7.35.0", request->headers()[0].second);
}

// Response Tests



// RequestHandler Tests


