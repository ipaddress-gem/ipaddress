#ifndef __chai__
#define __chai__

#include <exception>
#include <string>
#include <sstream>

namespace Chai {

  class AssertError : public std::exception {
    const std::string msg;
    public:
    AssertError(const char *msg) : msg(msg) {}
    ~AssertError() {}

    virtual const char* what() const throw() {
      return msg.c_str();
    }
  };

  class Assert {
    public:
      void isTrue(bool v, const char *msg = "") const {
        if (!v) { 
          std::stringstream s2;
          s2 << "isTrue got false, ";
          s2 << msg;
          throw AssertError(s2.str().c_str()); 
        }
      }
      void isFalse(bool v, const char *msg = "") const {
        if (v) { 
          std::stringstream s2;
          s2 << "isFalse got true, ";
          s2 << msg;
          throw AssertError(s2.str().c_str()); 
        }
      }
      void equal(const char *t1, const char *t2, const char *msg = "") const {
        if (strcmp(t1,t2)) { 
          std::stringstream s2;
          s2 << "is not equal ";
          s2 << t1 << "==" << t2 << ". ";
          s2 << msg;
          throw AssertError(s2.str().c_str()); 
        }
      }
      template<typename T> void equal(T t1, T t2, const char *msg = "") const {
        if (t1 != t2) { 
          std::stringstream s2;
          s2 << "is not equal ";
          s2 << t1 << "==" << t2 << ". ";
          s2 << msg;
          throw AssertError(s2.str().c_str()); 
        }
      }
  };
  static Assert assert;
}
#endif
