#include "chai.hpp"
#include "mocha.hpp"

#include <sstream>
#include <vector>

#include "../src/rle.hpp"

void assert_rle(std::vector<Rle> left, std::vector<Rle> right, const char *str) {
  // std::cout << str << std::endl;
  Chai::assert.equal(left.size(), right.size(), "array size() missmatch");
  for (size_t i = 0; i < left.size(); ++i) {
    // std::cout << left[i] << "==" << right[i] << std::endl;
    Chai::assert.isTrue(left[i].eq(right[i]),
                        ([str, i, left, right]() -> const char * {
                          std::stringstream s2;
                          s2 << str << "=>";
                          s2 << "left:" << left[i].toString();
                          s2 << " ";
                          s2 << "right:" << right[i].toString();
                          return s2.str().c_str();
                        })());
  }
}

int main(int, char **) {

  describe("rle", []() {
    it("test_rle", []() {
      assert_rle(Rle::code({}), {}, "empty");
      assert_rle(Rle::code({4711}), {Rle(4711, 0, 1, true)}, "one");
      assert_rle(Rle::code({4711, 4711}), {Rle(4711, 0, 2, true)}, "two");
      assert_rle(Rle::code({4711, 4711, 4811}),
                 {Rle(4711, 0, 2, true), Rle(4811, 1, 1, true)}, "three");
      assert_rle(Rle::code({4711, 4711, 4811, 4711, 4711}),
                 {Rle(4711, 0, 2, true), Rle(4811, 1, 1, true),
                  Rle(4711, 2, 2, true)},"double");
      assert_rle(Rle::code({4711, 4711, 4711, 4811, 4711, 4711}),
                 {Rle(4711, 0, 3, true), Rle(4811, 1, 1, true),
                  Rle(4711, 2, 2, false)}, "triple");
      assert_rle(
          Rle::code({4711, 4711, 4711, 4811, 4711, 4711, 4911, 4911, 4911}),
          {Rle(4711, 0, 3, true), Rle(4811, 1, 1, true), Rle(4711, 2, 2, false),
           Rle(4911, 3, 3, true)}, "double + triple");

      assert_rle(Rle::code({0x2001, 0x888, 0, 0x6630, 0, 0, 0, 0}),
                 {Rle(0x2001, 0, 1, true), Rle(0x888, 1, 1, true),
                  Rle(0, 2, 1, false), Rle(0x6630, 3, 1, true),
                  Rle(0, 4, 4, true)}, "real");
    });
  });
}

//  });
//  });
