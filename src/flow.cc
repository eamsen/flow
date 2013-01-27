// Copyright 2013 Eugen Sawin <esawin@me73.com>
#include <iostream>
#include "./clock.h"

using std::cout;
using std::endl;
using flow::Clock;

int main() {
  Clock begtime;
  cout << "flow";
  for (int i = 0; i < 10; ++i) {
    cout << " " << Clock() - begtime;
  }
  cout << endl;
}
