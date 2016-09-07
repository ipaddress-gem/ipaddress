#ifndef __MOCHA__
#define __MOCHA__

#include <iostream>
#include <exception>

typedef std::function<void()> MochaAction;

void describe(const char *title, MochaAction action) {
  std::cout << "Test:" << title << std::endl; 
  (action)();
}

void it(const char *title, MochaAction action) {
  try {
    (action)();
    std::cout << "   FINE " << title << std::endl;
  } catch (std::exception e) {
    std::cout << "   FAIL " << title << std::endl;
  }
}

#endif
