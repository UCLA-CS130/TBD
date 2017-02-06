#include "gtest/gtest.h"
#include "static_file_handler.h"
#include "http_request.h"

class StaticFileHandlerTest : public ::testing::Test {
protected:
    std::string get_mime_type(std::string file_path) {
        StaticFileHandler handler(file_path);
        return handler.get_mime_type();
    }
    std::string build_response(std::string file_path) {
        StaticFileHandler handler(file_path);
        return handler.build_response();
    }
    std::string read_file(bool &cannot_open_file, std::string file_path) {
        StaticFileHandler handler(file_path);
        return handler.read_file(cannot_open_file);
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
    EXPECT_EQ("text/plain", get_mime_type("turtle"));
}

TEST_F(StaticFileHandlerTest, ReadFileSuccessTest) {
    bool cannot_open_file = false;
    EXPECT_EQ("HEY!\n\n", read_file(cannot_open_file, "test.txt"));
    EXPECT_FALSE(cannot_open_file);
}

TEST_F(StaticFileHandlerTest, ReadFileErrorTest) {
    bool cannot_open_file = false;
    EXPECT_EQ("", read_file(cannot_open_file, "fail.txt"));
    EXPECT_TRUE(cannot_open_file);
}

TEST_F(StaticFileHandlerTest, BuildResponseSuccessTest) {
    const std::string expectation =
        "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHEY!\n\n";
    EXPECT_EQ(expectation, build_response("test.txt"));
}

TEST_F(StaticFileHandlerTest, BuildResponseErrorTest) {
    const std::string expectation =
        "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 File Not Found\n";
    EXPECT_EQ(expectation, build_response("fail.txt"));
}
