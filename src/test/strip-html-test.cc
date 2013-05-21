// Copyright 2013 Eugen Sawin <esawin@me73.com>
#include <gtest/gtest.h>
// #include <gmock/gmock.h>
#include <string>
#include <sstream>
#include "../io/html.h"

using std::string;
using std::stringstream;

using namespace flow::io;  // NOLINT

TEST(StripHtmlTest, test1) {
  string html = "<html><head><title>Title</title></head>";
  html += "<body>Body</body></html>";
  EXPECT_EQ("Body", StripHtml(html));

  html = "<html><head><title>Title</title></head><body>";
  html += "<div id=\"id1\" class=\"class1 class2\">Body</div>";
  html += "</body></html>";
  EXPECT_EQ("Body", StripHtml(html));

  html = "<html><head><title>Title</title></head><body>";
  html += "<div id=\"id1\" class=\"class1 class2\">Body</div>";
  html += "<script type=\"text/javascript\">function f() { return 0; }";
  html += "</script></body></html>";
  EXPECT_EQ("Body", StripHtml(html));
}
