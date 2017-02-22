#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "status_handler.h"

class StatusHandlerTest : public ::testing::Test {
protected:
    std::string GetName() {
        return status_handler_.GetName();
    }
    StatusHandler status_handler_;
};

TEST_F(StatusHandlerTest, GetNameCheck) {
    EXPECT_EQ("StatusHandler", GetName());
}
