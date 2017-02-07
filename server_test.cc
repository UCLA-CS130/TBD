#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "server.h"

class ServerTest : public ::testing::Test {
protected:
    bool handle_accept(boost::system::error_code err) {
        NginxConfig config;
        ServerConfig server_config(&config);
        Server server(io_service, &server_config);
        conn = new Connection(io_service, &server_config);
        return server.handle_accept(conn, err);
    }
    Connection* conn;
    boost::asio::io_service io_service;
};

TEST_F(ServerTest, HandleAcceptSuccess) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::success);
    EXPECT_TRUE(handle_accept(err));
}

TEST_F(ServerTest, HandleAcceptFail) {
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::bad_file_descriptor);
    EXPECT_FALSE(handle_accept(err));
}
