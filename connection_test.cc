#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "connection.h"

class ConnectionTest : public ::testing::Test {
protected:
    bool handleRead(boost::system::error_code err) {
        Connection conn(io_service);
        return conn.handle_read(err, 24);
    }
    bool handleWrite(boost::system::error_code err) {
        Connection conn(io_service);
        return conn.closeSocket(err);
    }
    boost::asio::io_service io_service;
};

TEST_F(ConnectionTest, HandleReadSuccess) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::success);
    EXPECT_TRUE(handleRead(err));
}

TEST_F(ConnectionTest, HandleReadFail) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::bad_file_descriptor);
    EXPECT_FALSE(handleRead(err));
}

TEST_F(ConnectionTest, HandleWriteSuccess) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::success);
    EXPECT_TRUE(handleWrite(err));
}

TEST_F(ConnectionTest, HandleWriteFail) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::bad_file_descriptor);
    EXPECT_FALSE(handleWrite(err));
}