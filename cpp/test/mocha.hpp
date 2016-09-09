#ifndef __MOCHA__
#define __MOCHA__

#include <iostream>
#include <exception>
#include "chai.hpp"
#include <stdlib.h>

typedef std::function<void()> MochaAction;

static int failCount = 0;
static int okCount = 0;

int exit() {
  std::cout << std::dec << std::endl << "Total " << okCount+failCount
    << " Ok " << okCount
    << " Fail " << failCount
    << " Assertions " << Chai::assert.count << std::endl;
  std::exit(std::min(failCount, 27));
}

void describe(const char *title, MochaAction action) {
  std::cout << "Test:" << title << std::endl;
  (action)();
}

void it(const char *title, MochaAction action) {
  try {
    (action)();
    std::cout << "   FINE " << title << std::endl;
    ++okCount;
  } catch (Chai::AssertError &e) {
    std::cout << "   FAIL " << title << ":" << e.what() << std::endl;
    ++failCount;
  }
}

#endif
