#include "gtest/gtest.h"
#include "static_file_handler.h"

class StaticFileHandlerTest : public ::testing::Test {
protected:
    std::string GetName() {
        return static_file_handler_.GetName();
    }
    std::string GetMimeType(const std::string& path) {
        return static_file_handler_.GetMimeType(path);
    }
    bool ReadFile(const std::string& path, std::string& content) {
        return static_file_handler_.ReadFile(path, content);
    }
    std::string HandleRequest(const std::string& raw) {
        auto request = Request::Parse(raw);
        Response response;
        static_file_handler_.HandleRequest(*request, &response);
        return response.ToString();
    }
    StaticFileHandler static_file_handler_;
};

TEST_F(StaticFileHandlerTest, GetNameCheck) {
    EXPECT_EQ("StaticFileHandler", GetName());
}

TEST_F(StaticFileHandlerTest, GetMimeTypeTestJPEG) {
    EXPECT_EQ("image/jpeg", GetMimeType("cat.jpeg"));
}

TEST_F(StaticFileHandlerTest, GetMimeTypeTestTXT) {
    EXPECT_EQ("text/plain", GetMimeType("dog.txt"));
}

TEST_F(StaticFileHandlerTest, GetMimeTypeTestHTML) {
    EXPECT_EQ("text/html", GetMimeType("bird.html"));
}

TEST_F(StaticFileHandlerTest, GetMimeTypeTestDefault) {
    EXPECT_EQ("text/plain", GetMimeType("turtle"));
}


TEST_F(StaticFileHandlerTest, ReadFileSuccessTest) {
    std::string file_content;
    EXPECT_TRUE(ReadFile("test.txt", file_content));
    EXPECT_EQ("HEY!\n\n", file_content);
}

TEST_F(StaticFileHandlerTest, ReadFileErrorTest) {
    std::string file_content;
    EXPECT_FALSE(ReadFile("fail.txt", file_content));
    EXPECT_EQ("", file_content);
}

TEST_F(StaticFileHandlerTest, HandleFailFileRequest) {
    EXPECT_EQ("HTTP/1.1 404 Not Found\r\n\r\n", HandleRequest("GET /fail.txt HTTP/1.1"));
}
