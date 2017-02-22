#include "gtest/gtest.h"
#include "static_file_handler.h"

class StaticFileHandlerTest : public ::testing::Test {
protected:
    std::string GetName() {
        return static_file_handler_.GetName();
    }

    StaticFileHandler static_file_handler_;
};

TEST_F(StaticFileHandlerTest, GetNameCheck) {
    EXPECT_EQ("StaticFileHandler", GetName());
}
