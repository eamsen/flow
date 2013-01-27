// Copyright 2013 Eugen Sawin <esawin@me73.com>
#include <iostream>
#include "./clock.h"

using std::cout;
using std::endl;
using flow::Clock;
using flow::ProcessClock;
using flow::ThreadClock;

int main() {
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
