#ifndef __chai__
#define __chai__

#include <exception>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

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
      size_t count = 0;
      void isTrue(bool v, const std::string &msg) {
        this->isTrue(v, msg.c_str());
      }
      void isTrue(bool v, const char *msg = "") {
        ++count;
        if (!v) {
          std::stringstream s2;
          s2 << "isTrue got false, ";
          s2 << msg;
          throw AssertError(s2.str().c_str());
        }
      }
      void isFalse(bool v, const char *msg = "") {
        ++count;
        if (v) {
          std::stringstream s2;
          s2 << "isFalse got true, ";
          s2 << msg;
          throw AssertError(s2.str().c_str());
        }
      }
      void equal(const std::string &t1, const char *t2, const char *msg = "") {
        this->equal(t1, std::string(t2), msg);
      }
      void equal(const char *t1, const std::string &t2, const char *msg = "") {
        this->equal(std::string(t1), t2, msg);
      }
      void equal(const char *t1, const char *t2, const char *msg = "") {
        this->equal(std::string(t1), std::string(t2), msg);
      }
      void equal(unsigned char t1, unsigned char t2, const char *msg = "") {
        ++count;
        if (t1 != t2) {
          std::stringstream s2;
          s2 << "uchar is not equal ";
          s2 << std::hex;
          s2 << static_cast<size_t>(t1) << "==" << static_cast<size_t>(t2) << ". ";
          s2 << msg;
          // std::cout << "MSG=>" << msg << std::endl;
          throw AssertError(s2.str().c_str());
        }

      }
      template<typename T> void equal(T t1, T t2, const char *msg = "") {
        ++count;
        if (t1 != t2) {
          std::stringstream s2;
          s2 << "is not equal ";
          s2 << t1 << "==" << t2 << ". ";
          s2 << msg;
          throw AssertError(s2.str().c_str());
        }
      }
      static std::string vec_to_string(size_t m, const std::vector<unsigned char> &t) {
        std::stringstream s2;
        s2 << std::hex;
        const char *sep = "";
        for (size_t i = 0; i < t.size(); ++i) {
            s2 << sep;
            if (m == i) { s2 << "["; }
            s2 << static_cast<size_t>(t[i]);
            if (m == i) {  s2 << "]"; }
            sep = " ";
        }
        return s2.str();
      }
      static std::string vec_to_string(size_t m, const std::vector<char> &t) {
        std::stringstream s2;
        s2 << std::hex;
        const char *sep = "";
        for (size_t i = 0; i < t.size(); ++i) {
            s2 << sep;
            if (m == i) { s2 << "["; }
            s2 << static_cast<size_t>(t[i]);
            if (m == i) {  s2 << "]"; }
            sep = " ";
        }
        return s2.str();
      }

      template<typename T>
      static std::string vec_to_string(size_t m, const std::vector<T> &t) {
        std::stringstream s2;
        const char *sep = "";
        for (size_t i = 0; i < t.size(); ++i) {
            s2 << sep;
            if (m == i) { s2 << "["; }
            s2 << t[i];
            if (m == i) {  s2 << "]"; }
            sep = " ";
        }
        return s2.str();
      }

      template<typename T> void deepEqual(std::vector<T> left, std::vector<T> right, const char *msg = "") {
        ++count;
          std::stringstream s2;
          s2 << "array size() missmatch:" <<  msg << ":" <<
            Assert::vec_to_string(0, left) << "====" <<
            Assert::vec_to_string(0, right);
          this->equal(left.size(), right.size(), s2.str().c_str());
          for (size_t i = 0; i < left.size(); ++i) {
            // std::cout << left[i] << "==" << right[i] << std::endl;
            std::stringstream s2;
            s2 << msg << "=>";
            s2 << "left:" << Assert::vec_to_string(i, left);
            s2 << "=====";
            s2 << "right:" << Assert::vec_to_string(i, right);
            this->equal(left[i], right[i], s2.str().c_str());
          }
      }
  };
  static Assert assert;
}
#endif
