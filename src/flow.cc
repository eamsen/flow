// Copyright 2013 Eugen Sawin <esawin@me73.com>
#include <cassert>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <sstream>
#include "./clock.h"
#include "./serialize.h"
#include "./stringify.h"
#include "./string.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::list;
using std::stringstream;
using std::pair;

void ClockDemo() {
  using flow::time::Clock;
  using flow::time::ProcessClock;
  using flow::time::ThreadClock;

  Clock realtime;
  ProcessClock proctime;
  ThreadClock threadtime;
  cout << "Here are some timings: ";
  for (int i = 0; i < 5; ++i) {
    cout << ThreadClock() - threadtime
         << "/" << ProcessClock() - proctime
         << "/" << Clock() - realtime << " ";
  }
  cout << endl;
}

void SerializeDemo() {
  using flow::io::Read;
  using flow::io::Write;

  {
    vector<vector<vector<int> > > nested =
        {{{}},
         {{1}},
         {{1}, {1, 2}}};
    stringstream ss;
    Write(nested, ss);
    vector<vector<vector<int> > > in;
    Read(ss, &in);
    assert(nested == in);
  }
  {
    unordered_map<string, vector<int> > nested =
        {{"key1", vector<int>({})},
         {"key2", vector<int>({1})},
         {"key3", vector<int>({1, 2, 3})}};
    stringstream ss;
    Write(nested, ss);
    unordered_map<string, vector<int> > in;
    Read(ss, &in);
    assert(nested == in);
  }
}

void StringifyDemo() {
  using flow::io::Str;

  {
    vector<int> vec = {1, 2, 3, 4};
    assert(Str(vec) == "(1, 2, 3, 4)");
    std::stringstream ss;
    ss << vec;
    assert(ss.str() == Str(vec));
  }
  {
    vector<pair<string, int>> vec = {{"a", 1}, {"bb", 2}, {"ccc", 3}};
    assert(Str(vec) == "(a: 1, bb: 2, ccc: 3)");
    std::stringstream ss;
    ss << vec;
    assert(ss.str() == Str(vec));
  }
  {
    vector<int> vec = {1, 2, 3, 4};
    assert(Str(vec, "; ", "[", "]") == "[1; 2; 3; 4]");
    std::stringstream ss;
    ss << vec;
    assert(ss.str() == Str(vec));
  }
  {
    vector<vector<vector<int> > > nested =
        {{{}},
         {{1}},
         {{1}, {1, 2}}};
    assert(Str(nested) == "((()), ((1)), ((1), (1, 2)))");
    std::stringstream ss;
    ss << nested;
    assert(ss.str() == Str(nested));
  }
  {
    unordered_map<string, vector<int> > nested =
        {{"key1", vector<int>({})},
         {"key2", vector<int>({1, 2, 3})}};
    string str = Str(nested);
    assert(str == "(key1: (), key2: (1, 2, 3))" ||
           str == "(key2: (1, 2, 3), key1: ())");
    std::stringstream ss;
    ss << nested;
    assert(ss.str() == Str(nested));
  }
}

void StringDemo() {
  using flow::string::Replace;
  string s = "The red fox has blue eyes.";
  Replace("red", "green", &s);
  assert(s == "The green fox has blue eyes.");
}

int main() {
  ClockDemo();
  SerializeDemo();
  StringifyDemo();
  StringDemo();
  cout << "Flow works flawlessly (otherwise you would not read this)." << endl;
}
