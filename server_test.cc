#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "server.h"

TEST(ServerTest, HandleAcceptSuccess) {
    boost::asio::io_service io_service;
	Server server(io_service, 8080);
	Connection conn(io_service);
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::success);

    EXPECT_TRUE(server.handle_accept(&conn, err));
}

TEST(ServerTest, HandleAcceptFail) {
    boost::asio::io_service io_service;
    Server server(io_service, 8080);
    Connection conn(io_service);
    boost::system::error_code err = boost::system::errc::make_error_code(boost::system::errc::bad_file_descriptor);

    EXPECT_FALSE(server.handle_accept(&conn, err));
}