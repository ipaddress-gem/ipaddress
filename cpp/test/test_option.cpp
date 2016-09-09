
#include "mocha.hpp"
#include "chai.hpp"

#include "../src/option.hpp"


int c_count = 0;
class TestOptional {
  public:
    int _42;
    TestOptional() : _42(42) { c_count++; }
    ~TestOptional() { --c_count; }

};

int main(int, char **) {

  describe("#option", [](){

      it("default", [](){
          auto none = Option<TestOptional>();
          Chai::assert.isTrue(none.isNone());
          Chai::assert.isFalse(none.isSome());
          try {
            none.unwrap();
            Chai::assert.isTrue(true, "unwrap should not work");
          } catch (OptionError e) {
          }
      });

      it("None", [](){
          auto none = None<TestOptional>();
          Chai::assert.isTrue(none.isNone());
          Chai::assert.isFalse(none.isSome());
          try {
            none.unwrap();
            Chai::assert.isTrue(true, "unwrap should not work");
          } catch (OptionError e) {
          }
      });

      it("Some", [](){
          TestOptional to;
          auto some = Some(to);
          to._42 = 8199;
          Chai::assert.isFalse(some.isNone());
          Chai::assert.isTrue(some.isSome());
          Chai::assert.equal(some.unwrap()._42, 42);
          some.unwrap()._42 = 4711;
          Chai::assert.equal(some.unwrap()._42, 4711);
      });
      it("Memory", [](){
          TestOptional to;
          c_count = 0;
          {
            auto some = Some(to);
            Chai::assert.equal(c_count, 1);
            Chai::assert.equal(some.unwrap()._42, 42);
          }
          Chai::assert.equal(c_count, 0, "loosing memory");
      });

  });
  return exit();

}
