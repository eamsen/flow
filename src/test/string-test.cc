// Copyright 2013 Eugen Sawin <esawin@me73.com>
#include <gtest/gtest.h>
// #include <gmock/gmock.h>
#include <string>
#include "../string/string.h"

using std::string;

using namespace flow::string;  // NOLINT

TEST(StringTest, Replace) {
  {
    string s = "";
    Replace("", "", &s);
    EXPECT_EQ("", s);
  }
  {
    string s = "";
    Replace("a", "", &s);
    EXPECT_EQ("", s);
  }
  {
    string s = "";
    Replace("", "a", &s);
    EXPECT_EQ("", s);
  }
  {
    string s = "a";
    Replace("", "a", &s);
    EXPECT_EQ("a", s);
  }
  {
    string s = " ab ";
    Replace("c", "x", &s);
    EXPECT_EQ(" ab ", s);
  }
  {
    string s = "abca";
    Replace("b", "a", &s);
    EXPECT_EQ("aaca", s);
  }
  {
    string s = "abca";
    Replace("a", "", &s);
    EXPECT_EQ("bc", s);
  }
  {
    string s = "abca";
    Replace("b", "", &s);
    EXPECT_EQ("aca", s);
  }
  {
    string s = "abca";
    Replace("a", "xxx", &s);
    EXPECT_EQ("xxxbcxxx", s);
  }
  {
    string s = "   ";
    Replace(" ", "x ", &s);
    EXPECT_EQ("x x x ", s);
  }
}
