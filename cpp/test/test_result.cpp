
#include "mocha.hpp"
#include "chai.hpp"

#include "../src/result.hpp"


class TestOptional {
  public:
    int _42;
    TestOptional() : _42(42) {}
};

int main(int, char **) {

  describe("#result", [](){

      it("Error", [](){
          auto err = Err<TestOptional>("testcase"); 
          Chai::assert.isTrue(err.isErr());
          Chai::assert.isFalse(err.isOk());
          Chai::assert.equal(err.text(), "testcase");
          try {
            err.unwrap();
            Chai::assert.isTrue(true, "unwrap should not work");
          } catch (OptionError e) {
          }
      });

      it("Some", [](){
          TestOptional to;
          auto some = Ok(to);
          Chai::assert.isFalse(some.isErr());
          Chai::assert.isTrue(some.isOk());
          Chai::assert.equal(some.unwrap()._42, 42);
          some.unwrap()._42 = 4711;
          Chai::assert.equal(some.unwrap()._42, 4711);
      });

  });
}
