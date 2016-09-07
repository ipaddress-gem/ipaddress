#ifndef __chai__
#define __chai__

#include <exception>

namespace Chai {

  class AssertError : public std::exception {
    const char *msg;
    public:
    AssertError(const char *msg) : msg(msg) {}
    ~AssertError() {}

    virtual const char* what() const throw() {
      return msg;
    }
  };

  class Assert {
    public:
      void isTrue(bool v, const char *msg = "") const {
        if (!v) { throw AssertError(msg); }
      }
      void isFalse(bool v, const char *msg = "") const {
        if (v) { throw AssertError(msg); }
      }
      void equal(const char *t1, const char *t2, const char *msg = "") const {
        if (strcmp(t1,t2)) { throw AssertError(msg); }
      }
      template<typename T> void equal(T t1, T t2, const char *msg = "") const {
        if (t1 != t2) { throw AssertError(msg); }
      }
  };
  static Assert assert;
}
#endif
