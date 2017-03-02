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

TEST(ResponseTest, CreateResponseTest) {
    Response r;
    r.SetStatus(Response::ResponseCode::OK);
    r.AddHeader("Field", "Value");
    r.SetBody("Body");

    EXPECT_EQ(Response::ResponseCode::OK, r.GetStatus());
    EXPECT_EQ("Body", r.body());
    EXPECT_EQ("HTTP/1.1 200 OK\r\nField: Value\r\n\r\nBody", r.ToString());
}

TEST(ResponseTest, ParseResponseTest) {
    std::string raw = "HTTP/1.1 302 Found\r\n"
                      "Location: http://www.ucla.edu/\r\n"
                      "\r\n"
                      "Body\n";
    auto response = Response::Parse(raw);

    EXPECT_EQ(302, response->GetStatus());
    EXPECT_EQ("Body\n", response->body());
    EXPECT_EQ(1, response->headers().size());
    EXPECT_EQ("Location", response->headers().at(0).first);
    EXPECT_EQ("http://www.ucla.edu/", response->headers()[0].second);
    EXPECT_EQ("HTTP/1.1 302 Found\r\n"
              "Location: http://www.ucla.edu/\r\n"
              "\r\n"
              "Body\n", response->ToString());
}


// RequestHandler Tests

class RequestHandlerTest : public ::testing::Test {
protected:
    std::string CreateByName(const std::string& type) {
        auto handler = RequestHandler::CreateByName(type);
        return handler->GetName();
    }
};

TEST_F(RequestHandlerTest, CreateEchoTest) {
    EXPECT_EQ("EchoHandler", CreateByName("EchoHandler"));
}

TEST_F(RequestHandlerTest, CreateStaticTest) {
    EXPECT_EQ("StaticFileHandler", CreateByName("StaticFileHandler"));
}

TEST_F(RequestHandlerTest, CreateNotFoundTest) {
    EXPECT_EQ("NotFoundHandler", CreateByName("NotFoundHandler"));
}

TEST_F(RequestHandlerTest, CreateStatusTest) {
    EXPECT_EQ("StatusHandler", CreateByName("StatusHandler"));
}

