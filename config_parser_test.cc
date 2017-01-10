#include "gtest/gtest.h"
#include "config_parser.h"

class NginxConfigParserStringTest : public ::testing::Test {
	protected:
		bool ParseString(const std::string config_string) {
			std::stringstream config_stream(config_string);
			return parser.Parse(&config_stream, &out_config);
		}
		
		NginxConfigParser parser;
		NginxConfig out_config;
};

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigStatementTest, ToStringTest) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");

	EXPECT_EQ("foo bar;\n", statement.ToString(0));
}

/* TODO
TEST(NginxConfigTest, ToStringTest) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");

	NginxConfig config;
	//std::shared_ptr<NginxConfigStatement> sharedPtr;
	//sharedPtr.reset(&statement);
	config.statements_.push_back(std::shared_ptr<NginxConfigStatement>(&statement));

	EXPECT_EQ("foo bar;\n", config.ToString());
} */

TEST_F(NginxConfigParserStringTest, SimpleConfigTest) {
	EXPECT_TRUE(ParseString("foo bar;"));
	EXPECT_EQ(1, out_config.statements_.size());
	EXPECT_EQ("bar", out_config.statements_[0]->tokens_[1]);
}

TEST_F(NginxConfigParserStringTest, InvalidNormalString) {
	ASSERT_FALSE(ParseString("foo bar"));
}

TEST_F(NginxConfigParserStringTest, ValidBlock) {
	EXPECT_TRUE(ParseString("location { foo; }"));
}

TEST_F(NginxConfigParserStringTest, InvalidBlock1) {
	EXPECT_FALSE(ParseString("location { foo;"));
}

TEST_F(NginxConfigParserStringTest, InvalidBlock2) {
	EXPECT_FALSE(ParseString("location foo;}"));
}

TEST(NginxConfigParserTest, TestConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

	bool test1 = parser.Parse("configtest", &out_config);

	EXPECT_TRUE(test1);
}
