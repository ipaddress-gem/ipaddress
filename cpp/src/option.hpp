#ifndef __OPTION__
#define __OPTION__

class OptionError : public std::exception {
    const char *msg;
  public:
    OptionError(const char *msg) : msg(msg) {}
    ~OptionError() {}

    virtual const char* what() const throw() {
      return msg;
    }
};

template<typename T> class Option {
  bool none;
  T t; 
  public:
  Option(T *t = 0) : none(t == 0) {
    if (t != 0) {
      this->t = *t;
    }
  }
  bool isNone() const {
    return none;
  }
  bool isSome() const {
    return !isNone();
  }
  T& unwrap() {
    if (this->none) {
      throw OptionError("try to unwrap a none option");
    }
    return t;
  }
};

template<typename T> Option<T> None() {
  return Option<T>();
}

template<typename T> Option<T> Some(T &t) {
  return Option<T>(&t);
}

#endif
