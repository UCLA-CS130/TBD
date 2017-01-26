#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "server.h"

class ServerTest : public ::testing::Test {
protected:
    bool handleAccept(boost::system::error_code err) {
        conn = new Connection(io_service);
        Server server(io_service, 8080);
        return server.handle_accept(conn, err);
    }
    Connection* conn;
    boost::asio::io_service io_service;
};

TEST_F(ServerTest, HandleAcceptSuccess) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::success);
    EXPECT_TRUE(handleAccept(err));
}

TEST_F(ServerTest, HandleAcceptFail) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::bad_file_descriptor);
    EXPECT_FALSE(handleAccept(err));
}