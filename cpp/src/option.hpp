#ifndef __OPTION__
#define __OPTION__

#include <exception>
#include <string>
#include <memory>

class OptionError : public std::exception {
    const std::string msg;
  public:
    OptionError(const char *msg) : msg(msg) {}
    ~OptionError() {}

    virtual const char* what() const throw() {
      return msg.c_str();
    }
};

template<typename T> class Option {
  bool none;
  std::shared_ptr<T> t;
  public:
  Option(T *t = 0) : none(t == 0) {
    if (t != 0) {
      auto my = new T();
      *my = *t;
      this->t = std::shared_ptr<T>(my);
    }
  }
  bool isNone() const {
    return none;
  }
  bool isSome() const {
    return !isNone();
  }
  const T& unwrap() const {
    if (this->none) {
      throw OptionError("try to unwrap a none option");
    }
    return *t;
  }
  T& unwrap() {
    if (this->none) {
      throw OptionError("try to unwrap a none option");
    }
    return *t;
  }
};

template<typename T> Option<T> None() {
  return Option<T>();
}

template<typename T> Option<T> Some(T &t) {
  return Option<T>(&t);
}

template<typename T>
std::ostream& operator<<(std::ostream &o, const Option<T> &option) {
  if (option.isNone()) {
    o << "None";
  } else {
    o << "Some(" << option.unwrap() << ")";
  }
  return o;
}

#endif
