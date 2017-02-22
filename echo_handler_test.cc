#include "gtest/gtest.h"
#include "echo_handler.h"

class EchoHandlerTest : public ::testing::Test {
protected:
    std::string GetName() {
        return echo_handler_.GetName();
    }
    EchoHandler echo_handler_;
};

TEST_F(EchoHandlerTest, GetNameCheck) {
    EXPECT_EQ("EchoHandler", GetName());
}
