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

TEST(NginxConfigStatementTest, SimpleToStringTest) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");

	EXPECT_EQ("foo bar;\n", statement.ToString(0));
}

TEST(NginxConfigStatementTest, BlockToStringTest) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("{");
	statement.tokens_.push_back("bar");
	statement.tokens_.push_back(";");
	statement.tokens_.push_back("}");

	EXPECT_EQ("foo { bar ; };\n", statement.ToString(0));
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

TEST_F(NginxConfigParserStringTest, EmptyBlock) {
	EXPECT_FALSE(ParseString("location { }"));
}

TEST_F(NginxConfigParserStringTest, MoreOpenThanCloseBlock) {
	EXPECT_FALSE(ParseString("location { foo;"));
}

TEST_F(NginxConfigParserStringTest, MoreCloseThanOpenBlock) {
	EXPECT_FALSE(ParseString("location foo;}"));
}

TEST_F(NginxConfigParserStringTest, TwoConsecCloseBlocks) {
	EXPECT_TRUE(ParseString("server { location { foo; }}"));
}

TEST_F(NginxConfigParserStringTest, RegularNestedBlocks) {
	EXPECT_TRUE(ParseString("server { location { foo; } bar; }"));
}

TEST_F(NginxConfigParserStringTest, ConfigWithOnlyComment) {
	EXPECT_FALSE(ParseString("# This will match any URI beginning with /forum"));
}

TEST_F(NginxConfigParserStringTest, RegularConfigWithComment) {
	EXPECT_TRUE(ParseString("foo; # This will match any URI beginning with /forum"));
}
