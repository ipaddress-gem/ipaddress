#include "crunchy.hpp"

std::ostream& operator<<(std::ostream &o, const Crunchy &crunchy) {
    o << crunchy.toString();
      return o;
}

static auto _zero = Crunchy::from_8bit({0});
const Crunchy& Crunchy::zero() {
  return _zero;
}
static auto _one = Crunchy::from_8bit({1});
const Crunchy& Crunchy::one() {
  return _one;
}
static auto _two = Crunchy::from_8bit({2});
const Crunchy& Crunchy::two(){
  return _two;
}
