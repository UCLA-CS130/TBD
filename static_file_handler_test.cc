#include "gtest/gtest.h"
#include "static_file_handler.h"
#include "http_request.h"

class StaticFileHandlerTest : public ::testing::Test {
protected:
    std::string get_mime_type(std::string request_string) {
        HttpRequest request(request_string);
        StaticFileHandler handler(&request);
        return handler.get_mime_type();
    }
};

TEST_F(StaticFileHandlerTest, GetMimeTypeTestJPEG) {
    EXPECT_EQ("image/jpeg", get_mime_type("GET /cat.jpeg HTTP/1.1\r\n\r\n"));
}

TEST_F(StaticFileHandlerTest, GetMimeTypeTestTXT) {
    EXPECT_EQ("text/plain", get_mime_type("GET /dog.txt HTTP/1.1\r\n\r\n"));
}

TEST_F(StaticFileHandlerTest, GetMimeTypeTestHTML) {
    EXPECT_EQ("text/html", get_mime_type("GET /bird.html HTTP/1.1\r\n\r\n"));
}

TEST_F(StaticFileHandlerTest, GetMimeTypeTestDefault) {
    EXPECT_EQ("", get_mime_type("GET /turtle HTTP/1.1\r\n\r\n"));
}

// TODO: Test read_file and build_response
