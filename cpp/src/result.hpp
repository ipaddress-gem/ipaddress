#ifndef __RESULT__
#define __RESULT__

#include "option.hpp"

class ResultError : public std::exception {
    const char *msg;
  public:
    ResultError(const char *msg) : msg(msg) {}
    ~ResultError() {}

    virtual const char* what() const throw() {
      return msg;
    }
};

template<typename T>
class Result {
  Option<T> val;
  const char *msg;
  public:
  Result(T *t, const char *msg = 0) : val(None<T>()), msg(msg) {
    if (t != 0) {
      this->val = Some<T>(*t);
    }
  }

  bool isOk() const {
    return msg == 0;
  }
  bool isErr() const {
    return msg != 0;
  }

  const char *text() const {
    if (isOk()) {
      throw ResultError("try to access text");
    }
    return msg;
  }

  T& unwrap() {
    return val.unwrap();
  }

};

template<typename T> Result<T> Ok(T &t) {
  return Result<T>(&t);
}
template<typename T> Result<T> Err(const char *msg) {
  return Result<T>(0, msg);
}

#endif
