#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);
	bool test1 = parser.Parse("configtest", &out_config);

  EXPECT_TRUE(success);
	EXPECT_TRUE(test1);
}
