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

TEST_F(NginxConfigParserStringTest, NestedBlocks1) {
	EXPECT_TRUE(ParseString("server { location { foo; }}"));
}

TEST_F(NginxConfigParserStringTest, NestedBlocks2) {
	EXPECT_TRUE(ParseString("server { location { foo; } bar; }"));
}

TEST_F(NginxConfigParserStringTest, CommentTest1) {
	EXPECT_FALSE(ParseString("# This will match any URI beginning with /forum"));
}

TEST_F(NginxConfigParserStringTest, CommentTest2) {
	EXPECT_TRUE(ParseString("foo; # This will match any URI beginning with /forum"));
}
