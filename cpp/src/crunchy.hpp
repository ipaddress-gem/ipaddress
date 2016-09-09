#ifndef __CRUNCHY__
#define __CRUNCHY__

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/spirit/include/qi.hpp>

#include <vector>

#include "result.hpp"

/**
 * based
 * Crunch - Arbitrary-precision integer arithmetic library
 * Copyright (C) 2014 Nenad Vukicevic crunch.secureroom.net/license
 *
 */
/**
 * @module Crunch
 * Radix: 28 bits
 * Endianness: Big
 *
 * @param {boolean} rawIn   - expect 28-bit arrays
 * @param {boolean} rawOut  - return 28-bit arrays
 */
// import Crunch from './crunch';

class NotImplementedException : public std::exception
{
public:
    NotImplementedException() {}
    ~NotImplementedException() {}
    virtual char const * what() const throw() { return "Function not yet implemented."; }
};

class Crunchy {
  typedef boost::multiprecision::uint128_t CrunchyType;
  CrunchyType num;
public:
  Crunchy clone() const {
    Crunchy ret;
    ret.num = this->num;
    return ret;
  }

  static Crunchy from_8bit(const std::vector<unsigned char> &number) {
    Crunchy ret;
    for (auto i : number) {
        ret.num = (ret.num << 8) + CrunchyType(i);
    }
    return ret;
  }

  static Result<Crunchy> parse(const std::string &val) {
    return Crunchy::from_string(val, 10);
  }

  static Crunchy from_number(size_t num)  {
    Crunchy ret;
    ret.num = CrunchyType(num);
    return ret;
  }
  static Result<Crunchy> from_string(const std::string &val, size_t radix = 10) {
  Crunchy ret;
  auto f(val.begin());
  auto l(val.end());
  if (radix == 10) {
    boost::spirit::qi::int_parser<CrunchyType, 10, 0, 39> uint128_dec;
    if (!boost::spirit::qi::parse(f, l, uint128_dec, ret.num)) {
        return Err<Crunchy>("inpossible to parse");
    }
  } else if (radix == 16) {
    boost::spirit::qi::int_parser<CrunchyType, 16, 0, 39> uint128_hex;
    if (!boost::spirit::qi::parse(f, l, uint128_hex, ret.num)) {
        return Err<Crunchy>("inpossible to parse");
    }
  } else {
    throw NotImplementedException();
  }
  return Ok(ret);
  }

  std::vector<unsigned char> to_8bit() const {
    std::vector<unsigned char> ret;
    auto my = this->num;
    // std::cout << "to_8bit:" << std::hex << my << std::endl;
    do {
        ret.push_back(static_cast<size_t>(my&0xff));
        my = my >> 8;
    } while (my != 0);
    std::reverse(ret.begin(),ret.end());
    // std::stringstream s2;
    // for (auto i : ret) {
    //     s2 << " " << std::hex << static_cast<size_t>(i);
    // }
    // std::cout << "to_8bit:" << s2.str() << std::endl;
    return ret;
  }

  int compare(const Crunchy &y) const {
    if (this->num < y.num) {
      // std::cout << "-1=" << this->num << "---" << y.num << std::endl;
      return -1;
    } else if (this->num > y.num) {
      // std::cout << "1=" << this->num << "---" << y.num << std::endl;
      return 1;
    }
    // std::cout << "0=" << this->num << "---" << y.num << std::endl;
    return 0;
  }

  bool eq(const Crunchy &oth) const {
    return this->compare(oth) == 0;
  }

  bool lte(const Crunchy &oth) const {
    return this->compare(oth) <= 0;
  }
  bool lt(const Crunchy &oth) const {
    return this->compare(oth) < 0;
  }

  bool gt(const Crunchy &oth) const {
    return this->compare(oth) > 0;
  }
  bool gte(const Crunchy &oth) const {
    return this->compare(oth) >= 0;
  }

  Crunchy add(const Crunchy &oth) const {
    Crunchy ret;
    ret.num = this->num + oth.num;
    return ret;
  }

  Crunchy sub(const Crunchy &oth) const {
    Crunchy ret;
    ret.num = this->num - oth.num;
    return ret;
  }
  Crunchy mul(const Crunchy &oth) const {
    Crunchy ret;
    ret.num = this->num * oth.num;
    return ret;
  }

  Crunchy shr(size_t s) const {
    Crunchy ret;
    ret.num = this->num >> s;
    return ret;
  }

  Crunchy shl(size_t s) const {
    Crunchy ret;
    ret.num = this->num << s;
    // std::cout << "shl:" << std::hex << this->num << ":" << s << ":" << ret.num << std::endl;
    return ret;
  }

  Crunchy div(const Crunchy &oth) const {
    Crunchy ret;
    ret.num = this->num / oth.num;
    return ret;
  }
  // public sub(cry: Crunchy): Crunchy {
  //   return Crunchy.from_8bit(Crunch.sub(this->num, cry.num));
  // }

  Crunchy mod(const Crunchy &oth) const {
    Crunchy ret;
    ret.num = this->num / oth.num;
    return ret;
  }

  size_t mds(size_t n) const {
    return static_cast<size_t>(this->num % n);
  }


  std::string toString(size_t radix = 10) const {
    std::stringstream s2;
    if (radix == 10) {
      s2 << std::dec;
    } else if (radix == 16) {
      s2 << std::hex;
    } else if (radix == 2) {
      std::vector<size_t> bits;
      auto my = this->num;
      do {
        size_t tmp = static_cast<size_t>(my&0x1);
        bits.push_back(tmp);
        my = my >> 1;
      } while(my != 0);
      for (int i = bits.size()-1; i >= 0; --i) {
        s2 << bits[i];
      }
      return s2.str();
    } else {
      throw NotImplementedException();
    }
    s2 << this->num;
    return s2.str();
  }

  static const Crunchy &zero();
  static const Crunchy &one();
  static const Crunchy &two();


};

std::ostream& operator<<(std::ostream &o, const Crunchy &crunchy);


#endif
