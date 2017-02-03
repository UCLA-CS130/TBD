#include "gtest/gtest.h"
#include "http_request.h"

TEST(HttpRequestTest, ParseRequestTest) {
    HttpRequest http_request("GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
    EXPECT_EQ("/", http_request.request_path_);
    EXPECT_EQ("localhost:8080", http_request.header_map_["Host"]);
}

TEST(HttpRequestTest, ParseRequestWithRequestLineOnly) {
    HttpRequest http_request("GET /echo HTTP/1.1\r\n\r\n");
    EXPECT_EQ("/echo", http_request.request_path_);
    EXPECT_EQ(0, http_request.header_map_.size());
}