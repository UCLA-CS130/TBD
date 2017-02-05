#include "gtest/gtest.h"
#include "static_file_handler.h"
#include "http_request.h"

class StaticFileHandlerTest : public ::testing::Test {
protected:
    std::string get_mime_type(std::string file_path) {
        StaticFileHandler handler(file_path);
        return handler.get_mime_type();
    }
};

TEST_F(StaticFileHandlerTest, GetMimeTypeTestJPEG) {
    EXPECT_EQ("image/jpeg", get_mime_type("cat.jpeg"));
}

TEST_F(StaticFileHandlerTest, GetMimeTypeTestTXT) {
    EXPECT_EQ("text/plain", get_mime_type("dog.txt"));
}

TEST_F(StaticFileHandlerTest, GetMimeTypeTestHTML) {
    EXPECT_EQ("text/html", get_mime_type("bird.html"));
}

TEST_F(StaticFileHandlerTest, GetMimeTypeTestDefault) {
    EXPECT_EQ("", get_mime_type("turtle"));
}

// TODO: Test read_file and build_response
