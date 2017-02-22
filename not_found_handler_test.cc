#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "not_found_handler.h"

class NotFoundHandlerTest : public ::testing::Test {
protected:
    std::string GetName() {
        return not_found_handler_.GetName();
    }
    NotFoundHandler not_found_handler_;
};

TEST_F(NotFoundHandlerTest, GetNameCheck) {
    EXPECT_EQ("NotFoundHandler", GetName());
}

