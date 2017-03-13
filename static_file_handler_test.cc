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
    std::string ProcessMarkdown(const std::string& content) {
        return static_file_handler_.ProcessMarkdown(content);
    }
    bool IsMarkdown(const std::string& file_path) {
        return static_file_handler_.IsMarkdown(file_path);
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

TEST_F(StaticFileHandlerTest, GetMimeTypeTestMarkdown) {
    EXPECT_EQ("text/html", GetMimeType("lama.md"));
}

TEST_F(StaticFileHandlerTest, IsMarkdownTestTrue) {
    EXPECT_TRUE(IsMarkdown("a.md"));
}

TEST_F(StaticFileHandlerTest, IsMarkdownTestFalse) {
    EXPECT_FALSE(IsMarkdown("a.txt"));
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

TEST_F(StaticFileHandlerTest, ProcessMarkdownTest) {
    std::string content = "# Title\n## Heading\nBody";
    std::string expected = "<h1>Title</h1>\n<h2>Heading</h2>\n<p>Body</p>\n\n";
    EXPECT_EQ(expected, ProcessMarkdown(content));
}