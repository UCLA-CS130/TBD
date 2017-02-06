#include "gtest/gtest.h"
#include "handler_factory.h"

class HandlerFactoryTest : public ::testing::Test {
    protected:
        bool is_prefix(std::string short_str, std::string long_str) {
            HandlerFactory handler_factory((ServerConfig*)nullptr, (HttpRequest*)nullptr);
            return handler_factory.is_prefix(short_str, long_str);
        }
};

TEST_F(HandlerFactoryTest, IsPrefixSameStringsTest) {
    EXPECT_TRUE(is_prefix("TBD", "TBD"));
}


TEST_F(HandlerFactoryTest, IsPrefixTrueTest) {
    EXPECT_TRUE(is_prefix("/static", "/static/index.html"));
}


TEST_F(HandlerFactoryTest, IsPrefixFalseTest) {
    EXPECT_FALSE(is_prefix("/fail", "/static/index.html"));
}
