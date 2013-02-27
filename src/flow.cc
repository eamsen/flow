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

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::list;
using std::stringstream;
using flow::time::Clock;
using flow::time::ProcessClock;
using flow::time::ThreadClock;
using flow::io::Read;
using flow::io::Write;

void ClockDemo() {
  Clock realtime;
  ProcessClock proctime;
  ThreadClock threadtime;
  cout << "flow";
  for (int i = 0; i < 10; ++i) {
    cout << " " << Clock() - realtime
         << "/" << ProcessClock() - proctime
         << "/" << ThreadClock() - threadtime;
  }
  cout << endl;
}

void SerializeDemo() {
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

int main() {
  ClockDemo();
  SerializeDemo();
}
