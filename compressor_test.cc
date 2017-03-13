#include "gtest/gtest.h"
#include "compressor.h"

// Expectations taken from outside gzip compression

TEST(CompressorTest, CompressEmptyTest) {
    EXPECT_STREQ("\x1F\x8B\b\0\0\0\0\0\x2\xFF\x3\0\0\0\0\0\0\0\0\0",
              Compressor::compress("").c_str());
}

TEST(CompressorTest, CompressStringTest) {
    EXPECT_STREQ("\x1F\x8B\b\0\0\0\0\0\x2\xFF\vI-.\x1\02\xD1Mx\x4\0\0\0",
              Compressor::compress("Test").c_str());
}
