
#include "mocha.hpp"
#include "chai.hpp"

#include "../src/option.hpp"


class TestOptional {
  public:
    int _42;
    TestOptional() : _42(42) {}
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
          Chai::assert.isFalse(some.isNone());
          Chai::assert.isTrue(some.isSome());
          Chai::assert.equal(some.unwrap()._42, 42);
          some.unwrap()._42 = 4711;
          Chai::assert.equal(some.unwrap()._42, 4711);
      });

  });
  return exit();

}
