#ifndef __MOCHA__
#define __MOCHA__

#include <iostream>
#include <exception>
#include "chai.hpp"

typedef std::function<void()> MochaAction;

void describe(const char *title, MochaAction action) {
  std::cout << "Test:" << title << std::endl;
  (action)();
}

void it(const char *title, MochaAction action) {
  try {
    (action)();
    std::cout << "   FINE " << title << std::endl;
  } catch (Chai::AssertError &e) {
    std::cout << "   FAIL " << title << ":" << e.what() << std::endl;
  }
}

#endif
