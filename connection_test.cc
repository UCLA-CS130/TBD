#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "connection.h"

class ConnectionTest : public ::testing::Test {
protected:
    bool handle_read(boost::system::error_code err) {
        conn = new Connection(io_service);
        return conn->handle_read(err, 24);
    }
    bool handle_write(boost::system::error_code err) {
        conn = new Connection(io_service);
        return conn->close_socket(err);
    }
    Connection* conn;
    boost::asio::io_service io_service;
};

TEST_F(ConnectionTest, HandleReadSuccess) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::success);
    EXPECT_TRUE(handle_read(err));
}

TEST_F(ConnectionTest, HandleReadFail) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::bad_file_descriptor);
    EXPECT_FALSE(handle_read(err));
}

TEST_F(ConnectionTest, HandleWriteSuccess) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::success);
    EXPECT_TRUE(handle_write(err));
}

TEST_F(ConnectionTest, HandleWriteFail) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::bad_file_descriptor);
    EXPECT_FALSE(handle_write(err));
}