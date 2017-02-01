#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "connection.h"

class ConnectionTest : public ::testing::Test {
protected:
    bool handle_read(boost::system::error_code err) {
        conn = new Connection(io_service);
        return conn->handle_read(err, bytes_transferred);
    }
    bool close_socket(boost::system::error_code err) {
        conn = new Connection(io_service);
        return conn->close_socket(err);
    }
    size_t bytes_transferred = 24;
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

TEST_F(ConnectionTest, CloseSocketSuccess) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::success);
    EXPECT_TRUE(close_socket(err));
}

TEST_F(ConnectionTest, CloseSocketFail) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::bad_file_descriptor);
    EXPECT_FALSE(close_socket(err));
}
