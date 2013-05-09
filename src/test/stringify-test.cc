// Copyright 2013 Eugen Sawin <esawin@me73.com>
#include <gtest/gtest.h>
// #include <gmock/gmock.h>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <tuple>
#include "../io/stringify.h"

using std::string;
using std::stringstream;
using std::vector;
using std::tuple;
using std::make_tuple;
using std::make_pair;
using std::pair;
using std::map;
using std::set;

using namespace flow::io;  // NOLINT

TEST(StringifyTest, vector) {
  {
    vector<int> vec = {};
    EXPECT_EQ("()", Str(vec));
  }
  {
    vector<int> vec = {1};
    EXPECT_EQ("(1)", Str(vec));
  }
  {
    vector<int> vec = {1, 2, 3, 4};
    EXPECT_EQ("(1, 2, 3, 4)", Str(vec));
  }
  {
    vector<string> vec = {"a", "b", "c"};
    EXPECT_EQ("(\"a\", \"b\", \"c\")", Str(vec));
    EXPECT_EQ("('a', 'b', 'c')",
              Str(vec, ", ", "(", ")", "", "", "", "'"));
  }
  {
    vector<int> vec = {1, 2, 3, 4};
    EXPECT_EQ("[1; 2; 3; 4]", Str(vec, "; ", "[", "]"));
  }
  {
    vector<int> vec = {};
    stringstream ss;
    ss << vec;
    EXPECT_EQ("()", ss.str());
  }
  {
    vector<int> vec = {1};
    stringstream ss;
    ss << vec;
    EXPECT_EQ("(1)", ss.str());
  }
  {
    vector<int> vec = {1, 2, 3, 4};
    stringstream ss;
    ss << vec;
    EXPECT_EQ("(1, 2, 3, 4)", ss.str());
  }
  {
    vector<int> vec = {1, 2, 3, 4};
    stringstream ss;
    Stringify::delim = "; ";
    Stringify::wrap_start = "[";
    Stringify::wrap_end = "]";
    ss << vec;
    EXPECT_EQ("[1; 2; 3; 4]", ss.str());
  }
  {
    // TODO(esawin): Not supported, can not overload for string type without
    // breaking the general operator use.
    vector<string> vec = {"a", "b", "c"};
    stringstream ss;
    Stringify::delim = "; ";
    Stringify::wrap_start = "[";
    Stringify::wrap_end = "]";
    Stringify::string_wrap = "'";
    ss << vec;
    // EXPECT_EQ("['a'; 'b'; 'c']", ss.str());
  }
}

TEST(StringifyTest, tuple) {
  {
    tuple<string, int> t = make_tuple("a", 1);
    EXPECT_EQ("(\"a\", 1)", Str(t)); 
    EXPECT_EQ("['a'; 1]", Str(t, "; ", "[", "]", "", "", "", "'")); 
  }
  {
    tuple<string, int, pair<string, int>> t =
        make_tuple("a", 1, make_pair("b", 2));
    EXPECT_EQ("(\"a\", 1, (\"b\": 2))", Str(t)); 
  }
}

TEST(StringifyTest, map) {
  {
    map<string, int> m = {};
    EXPECT_EQ("()", Str(m));
  }
  {
    map<string, int> m = {{"a", 1}, {"b", 2}};
    EXPECT_EQ("((\"a\": 1), (\"b\": 2))", Str(m));
  }
}

TEST(StringifyTest, JsonArray) {
  {
    vector<int> v = {};
    EXPECT_EQ("[]", JsonArray(v));
  }
  {
    vector<int> v = {1, 2, 3};
    EXPECT_EQ("[1,2,3]", JsonArray(v));
  }
  {
    vector<string> v = {"a", "b", "c"};
    EXPECT_EQ("[\"a\",\"b\",\"c\"]", JsonArray(v));
  }
  {
    map<string, vector<int>> v = {{"a", {1}}, {"b", {1, 2, 3}}};
    EXPECT_EQ("[[\"a\",[1]],[\"b\",[1,2,3]]]", JsonArray(v));
  }
  {
    map<string, tuple<string, int, int>> v =
        {{"a", make_tuple("b", 1, 2)}, {"c", make_tuple("d", 3, 4)}};
    EXPECT_EQ("[[\"a\",[\"b\",1,2]],[\"c\",[\"d\",3,4]]]", JsonArray(v));
  }
}
