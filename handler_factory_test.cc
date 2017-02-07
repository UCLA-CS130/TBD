#include "gtest/gtest.h"
#include "handler_factory.h"

class HandlerFactoryTest : public ::testing::Test {
    protected:
        bool is_prefix(std::string short_str, std::string long_str) {
            HandlerFactory handler_factory((ServerConfig*)nullptr, (HttpRequest*)nullptr);
            return handler_factory.is_prefix(short_str, long_str);
        }
        std::string transform_path(std::string request) {
            parseConfig("listen 8080; path { /static1/ test/my/;\n }", &out_config);
            ServerConfig server_config(&out_config);
            HttpRequest http_request(request);
            HandlerFactory handler_factory(&server_config, &http_request);
            return handler_factory.transform_path();
        }
        std::string create_handler(std::string request) {
            parseConfig("listen 8080;\npath { /static1/ ./;\n /echo/ /; }", &out_config);
            ServerConfig server_config(&out_config);
            HttpRequest http_request(request);
            HandlerFactory handler_factory(&server_config, &http_request);
            std::unique_ptr<RequestHandler> handler(handler_factory.create_handler());
            return handler->build_response();
        }

        void parseConfig(const std::string config_string, NginxConfig* out_config) {
            std::stringstream config_stream(config_string);
            parser.Parse(&config_stream, out_config);
        }
        NginxConfigParser parser;
        NginxConfig out_config;
};

TEST_F(HandlerFactoryTest, EchoHandlerCreateTest) {
    EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nGET / HTTP/1.1\r\n\r\n",
                create_handler("GET / HTTP/1.1\r\n\r\n"));
}

TEST_F(HandlerFactoryTest, StaticHandlerCreateTest) {
    EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHEY!\n\n",
                create_handler("GET /static1/test.txt HTTP/1.1\r\n\r\n"));
}

TEST_F(HandlerFactoryTest, FaviconTransformPathTest) {
    EXPECT_EQ("favicon.png", transform_path("GET /favicon.ico HTTP/1.1\r\n\r\n"));
}

TEST_F(HandlerFactoryTest, StaticTransformPathTest) {
    EXPECT_EQ("test/my/cat.jpg", transform_path("GET /static1/cat.jpg HTTP/1.1\r\n\r\n"));
}

TEST_F(HandlerFactoryTest, IsPrefixSameStringsTest) {
    EXPECT_TRUE(is_prefix("TBD", "TBD"));
}


TEST_F(HandlerFactoryTest, IsPrefixTrueTest) {
    EXPECT_TRUE(is_prefix("/static", "/static/index.html"));
}


TEST_F(HandlerFactoryTest, IsPrefixFalseTest) {
    EXPECT_FALSE(is_prefix("/fail", "/static/index.html"));
}


