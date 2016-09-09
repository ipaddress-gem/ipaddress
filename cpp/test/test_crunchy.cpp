
#include "../src/crunchy.hpp"
#include "chai.hpp"
#include "mocha.hpp"

int main(int, char **) {

  describe("#compare", []() {

    it("Should confirm equality", []() {
      auto x = Crunchy::from_string("12345678901234567890").unwrap();
      auto y = Crunchy::from_string("12345678901234567890").unwrap();
      // std::cout << x << ":" << y << std::endl;
      Chai::assert.isTrue(x.eq(y));
      // this.done();
    });

    it("Should find first is greater than second", []() {
      auto x = Crunchy::from_string("12345678901234567890").unwrap();
      auto y = Crunchy::from_string("1234567890").unwrap();
      // std::cout << x << ":" << y << std::endl;
      Chai::assert.isTrue(x.gt(y));
      // this.done();
    });

    it("Should find first is less than second", []() {
      auto x = Crunchy::from_string("1234567890").unwrap();
      auto y = Crunchy::from_string("12345678901234567890").unwrap();
      // std::cout << x << ":" << y << std::endl;
      Chai::assert.isTrue(x.lt(y));
      // this.done();
    });

  });

  describe("#add", []() {

    it("Should add numbers", []() {
      auto x = Crunchy::from_8bit({242, 62});
      auto y = Crunchy::from_8bit({42, 2});

      Chai::assert.deepEqual(x.add(y).to_8bit(), {1, 28, 64});
      // x.should.eql([242, 62]);
      // y.should.eql([42, 2]);
    });

    it("Should add numbers, first longer than second", []() {
      auto x = Crunchy::from_8bit({26, 255, 230, 17});
      auto y = Crunchy::from_8bit({42, 34});

      Chai::assert.deepEqual(x.add(y).to_8bit(), {27, 0, 16, 51});

      // x, [26, 255, 230, 17]);
      // y.should.eql([42, 34]);
    });

    it("Should add numbers, second longer than first", []() {
      auto x = Crunchy::from_8bit({234, 34});
      auto y = Crunchy::from_8bit({255, 255, 230, 17});

      Chai::assert.deepEqual(x.add(y).to_8bit(), {1, 0, 0, 208, 51});

      // x.should.eql([234, 34]);
      // y.should.eql([255, 255, 230, 17]);
    });

    it("Should add two identical numbers", []() {
      auto x = Crunchy::from_8bit({255, 255, 255, 255, 255, 255});
      auto y = Crunchy::from_8bit({255, 255, 255, 255, 255, 255});

      Chai::assert.deepEqual(x.add(y).to_8bit(),
                             {1, 255, 255, 255, 255, 255, 254});

      // x.should.eql([255, 255, 255, 255, 255, 255]);
      // y.should.eql([255, 255, 255, 255, 255, 255]);
    });

    it("Should add positive to negative number expecting positive result",
       []() {
         auto y = Crunchy::from_8bit({51, 254, 144, 207});
         auto x = Crunchy::from_8bit(
             {0xff,      0xff,      0xff,       0xff,     0xff, 0xff, 0xff,
              0xff,      0xff,      0xff,       0xff,     0xff, 0xff, 0xff,
              0xff,      0xff,      0xff,       0xff,     0xff, 0xff, 0xff,
              0xff,      0xff,      0xff,       0xff,     0xff, 0xff, 0xff,
              0xff - 20, 0xff - 89, 0xff - 145, 0xff - 31});

         Chai::assert.deepEqual(x.add(y).to_8bit(), {31, 164, 255, 175});

         // x.should.eql([51, 254, 144, 207]);
         // y.should.eql([-20, 89, 145, 32]);
       });

    it("Should add positive to negative number expecting negative result",
       []() {
         auto x = Crunchy::from_8bit(
             {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,       0xff,
              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,       0xff,
              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,       0xff,
              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff - 242, 0xff - 61});
         auto y = Crunchy::from_8bit({242, 64});

         Chai::assert.deepEqual(x.add(y).to_8bit(), {2});

         // x.should.eql([242, 62]);
         // y.should.eql([-242, 64]);
       });

    it("Should add negative to positive number expecting positive result",
       []() {
         auto x = Crunchy::from_8bit({42, 2});
         auto y = Crunchy::from_8bit({242, 62});

         Chai::assert.deepEqual(x.add(y).to_8bit(), {1, 28, 64});

         // x, [-42, 2]);
         // y, [242, 62]);
       });

    it("Should add negative to positive number expecting negative result",
       []() {
         auto x = Crunchy::from_8bit({42, 2});
         auto y = Crunchy::from_8bit({42, 0});

         Chai::assert.deepEqual(x.add(y).to_8bit(), {84, 2});
         // x, [-42, 2]);
         // y, [42, 0]);
       });

    it("Should add two negative numbers expecting negative result", []() {
      auto x = Crunchy::from_8bit({242, 62});
      auto y = Crunchy::from_8bit({42, 2});

      Chai::assert.deepEqual(x.add(y).to_8bit(), {1, 28, 64});

      // x, [-242, 62]);
      // y, [-42, 2]);
    });

    it("Should add zero to number", []() {
      auto x = Crunchy::from_8bit({26, 255, 230, 17});
      auto y = Crunchy::from_8bit({0});

      Chai::assert.deepEqual(x.add(y).to_8bit(), {26, 255, 230, 17});

      // x, [26, 255, 230, 17]);
      // y, { 0 });
    });

    it("Should add number to zero", []() {
      auto x = Crunchy::from_8bit({0});
      auto y = Crunchy::from_8bit({43, 123, 200, 1});

      Chai::assert.deepEqual(x.add(y).to_8bit(), {43, 123, 200, 1});

      // x, { 0 });
      // y, [43, 123, 200, 1]);
    });

    it("Should add two zeros", []() {
      auto x = Crunchy::from_8bit({0});
      auto y = Crunchy::from_8bit({0});

      Chai::assert.deepEqual(x.add(y).to_8bit(), {0});

      // x.should.eql({ 0 });
      // y.should.eql({ 0 });
    });

  });

  describe("#add-from_string", []() {

    it("Should add numbers", []() {
      // console.log(new Date());
      auto x = Crunchy::from_string("12345678911234567891").unwrap();
      // console.log(new Date());
      auto y = Crunchy::from_string("12345678901234567890").unwrap();
      // console.log(new Date(), x, y);
      // console.log("addddddd:", new Date(), x, x.toString(), y, y.toString(),
      // x.add(y).toString());
      Chai::assert.equal(x.add(y).toString(), "24691357812469135781");
      // console.log(new Date());
      // this.done();
    });

    it("Should add numbers, first longer than second", []() {
      auto x = Crunchy::from_string("12345678901234567890").unwrap();
      auto y = Crunchy::from_string("1234567890").unwrap();
      Chai::assert.equal(x.add(y).toString(), "12345678902469135780");
      // this.done();
    });

    it("Should add numbers, second longer than first", []() {
      auto x = Crunchy::from_string("1234567890").unwrap();
      auto y = Crunchy::from_string("12345678901234567890").unwrap();
      Chai::assert.equal(x.add(y).toString(), "12345678902469135780");
      // this.done();
    });

    it("Should add two identical numbers", []() {
      auto x = Crunchy::from_string("12345678901234567890").unwrap();
      auto y = Crunchy::from_string("12345678901234567890").unwrap();
      Chai::assert.equal(x.add(y).toString(), "24691357802469135780");
      // this.done();
    });

    it("Should add zero to number", []() {
      auto x = Crunchy::from_string("12345678901234567890").unwrap();
      auto y = Crunchy::from_string("0").unwrap();
      Chai::assert.equal(x.add(y).toString(), "12345678901234567890");
      // this.done();
    });

    it("Should add number to zero", []() {
      auto x = Crunchy::from_string("0").unwrap();
      auto y = Crunchy::from_string("12345678901234567890").unwrap();
      Chai::assert.equal(x.add(y).toString(), "12345678901234567890");
      // this.done();
    });

    it("Should add two zeros", []() {
      auto x = Crunchy::from_string("0").unwrap();
      auto y = Crunchy::from_string("0").unwrap();
      Chai::assert.equal(x.add(y).toString(), "0");
      // this.done();
    });

  });

  describe("#sub", []() {

    it("Should subtract numbers", []() {
      auto x = Crunchy::from_8bit({170, 1, 79, 119, 242, 62});
      auto y = Crunchy::from_8bit({17, 241, 123, 250, 42, 2});

      // console.log("Should subtract numbers:", x, y, x.sub(y));
      Chai::assert.deepEqual(x.sub(y).to_8bit(), {152, 15, 211, 125, 200, 60});
      // x.to_8bit().should.eql([170, 1, 79, 119, 242, 62]);
      // y.should.eql([17, 241, 123, 250, 42, 2]);
    });

    it("Should subtract one", []() {
      auto x = Crunchy::from_8bit({240, 0, 0, 0, 0, 0});
      auto y = Crunchy::from_8bit({1});

      Chai::assert.deepEqual(x.sub(y).to_8bit(),
                             {239, 255, 255, 255, 255, 255});

      // x.should.eql([240, 0, 0, 0, 0, 0]);
      // y.should.eql({ 1 });
    });

    it("Should subtract numbers expecting negative result", []() {
      auto x = Crunchy::from_8bit({26});
      auto y = Crunchy::from_8bit({255});

      Chai::assert.deepEqual(x.sub(y).to_8bit(),
                             {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1b});

      // x.should.eql({ 26 });
      // y.should.eql({ 255 });
    });

    it("Should subtract numbers expecting negative result", []() {
      auto x = Crunchy::from_8bit({188, 196});
      auto y = Crunchy::from_8bit({188, 197});

      Chai::assert.deepEqual(x.sub(y).to_8bit(),
                             {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff});

      // x.should.eql([188, 196]);
      // y.should.eql([188, 197]);
    });

    it("Should subtract negative number from positive", []() {
      auto y = Crunchy::from_8bit({240, 0, 0, 0, 0, 0});
      auto x = Crunchy::from_8bit({1});
      // console.log("negative:", x, y, x.sub(y));
      Chai::assert.deepEqual(x.sub(y).to_8bit(),
                             {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x00, 0x01});

      // x.should.eql([240, 0, 0, 0, 0, 0]);
      // y.should.eql([-1]);
    });

    it("Should subtract positive number from negative", []() {
      auto x = Crunchy::from_8bit({240, 0, 0, 0, 0, 0});
      auto y = Crunchy::from_8bit({1});

      Chai::assert.deepEqual(x.sub(y).to_8bit(),
                             {239, 255, 255, 255, 255, 255});

      // x.should.eql([-240, 0, 0, 0, 0, 0]);
      // y.should.eql({ 1 });
    });

    it("Should subtract two negative numbers", []() {
      auto x = Crunchy::from_8bit({240, 0, 0, 0, 0, 0});
      auto y = Crunchy::from_8bit({1});

      Chai::assert.deepEqual(x.sub(y).to_8bit(),
                             {239, 255, 255, 255, 255, 255});

      // x.should.eql([-240, 0, 0, 0, 0, 0]);
      // y.should.eql([-1]);
    });

    it("Should subtract two equal numbers expecting zero", []() {
      auto x = Crunchy::from_8bit({20});
      auto y = Crunchy::from_8bit({20});

      Chai::assert.deepEqual(x.sub(y).to_8bit(), {0});

      // x.should.eql({ 20 });
      // y.should.eql({ 20 });
    });

    it("Should subtract zero", []() {
      auto x = Crunchy::from_8bit({244, 137, 7, 161});
      auto y = Crunchy::from_8bit({0});

      Chai::assert.deepEqual(x.sub(y).to_8bit(), {244, 137, 7, 161});

      // x.should.eql([244, 137, 7, 161]);
      // y.should.eql({ 0 });
    });

    it("Should subtract zero from zero", []() {
      auto x = Crunchy::from_8bit({0});
      auto y = Crunchy::from_8bit({0});

      Chai::assert.deepEqual(x.sub(y).to_8bit(), {0});

      // x.should.eql({ 0 });
      // y.should.eql({ 0 });
    });

    it("Should subtract from zero", []() {
      auto x = Crunchy::from_8bit({0});
      auto y = Crunchy::from_8bit({15});

      Chai::assert.deepEqual(x.sub(y).to_8bit(),
                             {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1});

      // x.should.eql({ 0 });
      // y.should.eql({ 15 });
    });

  });

  describe("#mul", []() {

    it("Should multiply numbers", []() {
      auto x = Crunchy::from_8bit({162, 51, 95});
      auto y = Crunchy::from_8bit({42, 18, 204});

      Chai::assert.deepEqual(x.mul(y).to_8bit(), {26, 168, 86, 115, 157, 180});
    });

    it("Should multiply numbers", []() {
      auto x = Crunchy::from_8bit({255, 65, 34, 51, 95});
      auto y = Crunchy::from_8bit({42, 18, 204});

      Chai::assert.deepEqual(x.mul(y).to_8bit(),
                             {41, 243, 109, 152, 188, 115, 157, 180});
    });

    it("Should multiply numbers", []() {
      auto x = Crunchy::from_8bit({255, 255, 255, 255});
      auto y = Crunchy::from_8bit({255, 255, 255, 255});

      Chai::assert.deepEqual(x.mul(y).to_8bit(),
                             {255, 255, 255, 254, 0, 0, 0, 1});
    });

    it("Should multiply positive with negative number", []() {
      auto x = Crunchy::from_8bit({77, 242, 62});
      auto y = Crunchy::from_8bit({42, 2, 113, 43, 57, 65});

      Chai::assert.deepEqual(x.mul(y).to_8bit(),
                             {12, 202, 124, 133, 146, 125, 36, 79, 190});
    });

    it("Should multiply negative with positive number", []() {
      auto x = Crunchy::from_8bit({255, 17, 162, 62});
      auto y = Crunchy::from_8bit({255, 17, 162, 62});

      Chai::assert.deepEqual(x.mul(y).to_8bit(),
                             {254, 36, 34, 110, 119, 14, 135, 4});
    });

    it("Should multiply two negative numbers", []() {
      auto x = Crunchy::from_8bit({162, 51, 95});
      auto y = Crunchy::from_8bit({42, 18, 204});

      Chai::assert.deepEqual(x.mul(y).to_8bit(), {26, 168, 86, 115, 157, 180});
    });

    it("Should multiply by zero", []() {
      auto x = Crunchy::from_8bit({77, 242, 62});
      auto y = Crunchy::from_8bit({0});

      Chai::assert.deepEqual(x.mul(y).to_8bit(), {0});
    });

    it("Should multiply zero by number", []() {
      auto x = Crunchy::from_8bit({0});
      auto y = Crunchy::from_8bit({77, 242, 62});

      Chai::assert.deepEqual(x.mul(y).to_8bit(), {0});
    });

    it("Should multiply zeros", []() {
      auto x = Crunchy::from_8bit({0});
      auto y = Crunchy::from_8bit({0});

      Chai::assert.deepEqual(x.mul(y).to_8bit(), {0});
    });

  });

  describe("#div", []() {

    it("Should divide numbers", []() {
      auto x = Crunchy::from_8bit({52, 155, 168, 23, 6, 85});
      auto y = Crunchy::from_8bit({19, 26, 247});

      Chai::assert.deepEqual(x.div(y).to_8bit(), {2, 192, 234, 136});
    });

    it("Should divide by one", []() {
      auto x = Crunchy::from_8bit({15, 127, 73, 1});
      auto y = Crunchy::from_8bit({1});

      Chai::assert.deepEqual(x.div(y).to_8bit(), {15, 127, 73, 1});
    });

    it("Should divide by self", []() {
      auto x = Crunchy::from_8bit({15, 127, 73, 1});
      auto y = Crunchy::from_8bit({15, 127, 73, 1});

      Chai::assert.deepEqual(x.div(y).to_8bit(), {1});
    });

    it("Should divide negative number", []() {
      auto x = Crunchy::from_8bit({170, 153, 136});
      auto y = Crunchy::from_8bit({17, 68});

      Chai::assert.deepEqual(x.div(y).to_8bit(), {9, 225});
    });

    it("Should divide by negative number", []() {
      auto x = Crunchy::from_8bit({170, 153, 136, 119, 102, 85});
      auto y = Crunchy::from_8bit({17, 68});

      Chai::assert.deepEqual(x.div(y).to_8bit(), {9, 225, 129, 255, 9});
    });

    it("Should divide two negative numbers", []() {
      auto x = Crunchy::from_8bit({52, 155, 168, 23, 6, 85});
      auto y = Crunchy::from_8bit({19, 26, 247});

      Chai::assert.deepEqual(x.div(y).to_8bit(), {2, 192, 234, 136});
    });

    it("Should divide zero", []() {
      auto x = Crunchy::from_8bit({0});
      auto y = Crunchy::from_8bit({17, 68});

      Chai::assert.deepEqual(x.div(y).to_8bit(), {0});
    });

    it("Shouldn't divide by zero", []() {
      auto x = Crunchy::from_8bit({170, 153});
      auto y = Crunchy::from_8bit({0});
      try {
        x.div(y);
        Chai::assert.isTrue(false, "never should touched");
      } catch (std::overflow_error e) {
      }
      // Chai::assert.isNotOk(x.div(y));
    });

    it("Shouldn't divide zero by zero", []() {
      auto x = Crunchy::from_8bit({0});
      auto y = Crunchy::from_8bit({0});
      try {
        x.div(y);
        Chai::assert.isTrue(false, "never should touched");
      } catch (std::overflow_error e) {
      }
    });

  });

  describe("#mod", []() {

    it("Should calculate modulo", []() {
      auto x = Crunchy::from_8bit({52, 155, 168, 23, 6, 85}),
           y = Crunchy::from_8bit({19, 26, 247});

      Chai::assert.deepEqual(x.mod(y).to_8bit(), {10, 237, 29});
    });

    it("Should calculate modulo of number smaller than modulus", []() {
      auto x = Crunchy::from_8bit({1, 0}), y = Crunchy::from_8bit({1, 241});

      Chai::assert.deepEqual(x.mod(y).to_8bit(), {1, 0});
    });

    it("Should calculate modulo of modulus", []() {
      auto x = Crunchy::from_8bit({1, 241}), y = Crunchy::from_8bit({1, 241});

      Chai::assert.deepEqual(x.mod(y).to_8bit(), {0});
    });

    it("Should calculate modulo of zero", []() {
      auto x = Crunchy::from_8bit({0}), y = Crunchy::from_8bit({1, 241});

      Chai::assert.deepEqual(x.mod(y).to_8bit(), {0});
    });

    // it("Should calculate modulo of negative", []() {
    //   auto x = Crunchy::zero().sub(Crunchy::from_8bit({4})),
    //        y = Crunchy::from_8bit({3});
    //
    //   Chai::assert.deepEqual(x.mod(y).to_8bit(), {2});
    // });
    //
    // it("Should calculate modulo of negative", []() {
    //   auto x = Crunchy::zero().sub(Crunchy::from_8bit({10, 18})),
    //        y = Crunchy::from_8bit({123});
    //
    //   Chai::assert.deepEqual(x.mod(y).to_8bit(), {5});
    // });
    //
    // it("Should calculate modulo of negative", []() {
    //   auto x = Crunchy::zero().sub(Crunchy::from_8bit({15, 62, 62, 18, 176})),
    //        y = Crunchy::from_8bit({139, 207, 194, 82});
    //
    //   Chai::assert.deepEqual(x.mod(y).to_8bit(), {12, 123, 46, 72});
    // });

  });

  describe("#leftShift", []() {

    it("Should left shift a number", []() {
      auto x = Crunchy::from_8bit({22, 11});
      size_t s = 5;

      Chai::assert.deepEqual(x.shl(s).to_8bit(), {2, 193, 96});
    });

    it("Should left shift a number more", []() {
      auto x = Crunchy::from_8bit({1});
      size_t s = 64;

      Chai::assert.deepEqual(x.shl(s).to_8bit(), {1, 0, 0, 0, 0, 0, 0, 0, 0});
    });

    it("Should left shift zero", []() {
      auto x = Crunchy::from_8bit({0});
      size_t s = 80;

      Chai::assert.deepEqual(x.shl(s).to_8bit(), {0});
    });

    it("Should left shift negative", []() {
      auto x = Crunchy::from_8bit({3});
      size_t s = 8;

      Chai::assert.deepEqual(x.shl(s).to_8bit(), {3, 0});
    });

    it("Should left shift one", []() {
      auto x = Crunchy::from_8bit({1, 0, 0, 0, 0});
      size_t s = 1;
      Chai::assert.deepEqual(x.shl(s).to_8bit(), {2, 0, 0, 0, 0});
    });

  });

  describe("#rightShift", []() {

    it("Should right shift a number to zero", []() {
      auto x = Crunchy::from_8bit({44, 44, 44});
      // Chai::assert.deepEqual(x.rsh(32).num, { 0 });
      Chai::assert.deepEqual(x.shr(32).to_8bit(), {0});
      Chai::assert.deepEqual(x.shr(33).to_8bit(), {0});
    });

    it("Should right shift a number", []() {
      auto x = Crunchy::from_8bit({22, 11});
      size_t s = 5;
      // std::cout << "XXXX 22:11" << x << std::endl;
      Chai::assert.deepEqual(x.shr(s).to_8bit(), {176});
    });

    it("Should right shift a number out of existance", []() {
      auto x = Crunchy::from_8bit({22, 11});
      size_t s = 20;

      Chai::assert.deepEqual(x.shr(s).to_8bit(), {0});
    });

    it("Should right shift a negative number", []() {
      auto x = Crunchy::from_8bit({3});
      size_t s = 8;
      Chai::assert.deepEqual(x.shr(s).to_8bit(), {0});
    });

  });

  // describe("#parse", [](){

  //   it("Should convert string to integer", [](){
  //     auto s = "6315359056060240643798024";

  //     Crunchy::parse(s).to_8bit().should.eql([5, 57, 84, 76, 233, 0, 120,
  //     91,
  //     180, 180, 8]);

  //     s.should.equal("6315359056060240643798024");
  //   });

  //   it("Should convert string negative number to integer", [](){
  //     auto s = "-256";

  //     Crunchy::parse(s).to_8bit().should.eql([-1, 0]);

  //     s.should.equal("-256");
  //   });

  // });

  // describe("#stringify", [](){

  //   it("Should convert integer to string", [](){
  //     auto x = [5, 57, 84, 76, 233, 0, 120, 91, 180, 180, 8];

  //     Crunchy::from_8bit(x).to_8bit().should.equal("6315359056060240643798024");

  //     x.should.eql([5, 57, 84, 76, 233, 0, 120, 91, 180, 180, 8]);
  //   });

  // });
  return exit();
}
