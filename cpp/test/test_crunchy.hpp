// import { should } from "should";
import { assert } from 'chai';
import * as Mocha from 'mocha';

// import 'should';

// import Crunch from "../src/crunch";
import Crunchy from "../src/crunchy";

describe("#compare", function () {

  it("Should confirm equality", function () {
    let x = Crunchy.from_string("12345678901234567890"),
      y = Crunchy.from_string("12345678901234567890");
    assert.isTrue(x.eq(y));
    // this.done();
  });

  it("Should find first is greater than second", function () {
    let x = Crunchy.from_string("12345678901234567890");
    let y = Crunchy.from_string("1234567890");
    assert.isTrue(x.gt(y));
    // this.done();
  });

  it("Should find first is less than second", function () {
    let x = Crunchy.from_string("1234567890");
    let y = Crunchy.from_string("12345678901234567890");
    assert.isTrue(x.lt(y));
    // this.done();
  });

});

describe("#add", function () {

  it("Should add numbers", function () {
    var x = Crunchy.from_8bit([242, 62]),
        y = Crunchy.from_8bit([42, 2]);

    assert.deepEqual(x.add(y).to_8bit(), [1, 28, 64]);
    // x.should.eql([242, 62]);
    // y.should.eql([42, 2]);
  });

  it("Should add numbers, first longer than second", function () {
    var x = Crunchy.from_8bit([26, 255, 230, 17]),
        y = Crunchy.from_8bit([42, 34]);

    assert.deepEqual(x.add(y).to_8bit(), [27, 0, 16, 51]);

    // x, [26, 255, 230, 17]);
    // y.should.eql([42, 34]);
  });

  it("Should add numbers, second longer than first", function () {
    var x = Crunchy.from_8bit([234, 34]),
        y = Crunchy.from_8bit([255, 255, 230, 17]);

    assert.deepEqual(x.add(y).to_8bit(), [1, 0, 0, 208, 51]);

    // x.should.eql([234, 34]);
    // y.should.eql([255, 255, 230, 17]);
  });

  it("Should add two identical numbers", function () {
    var x = Crunchy.from_8bit([255, 255, 255, 255, 255, 255]),
        y = Crunchy.from_8bit([255, 255, 255, 255, 255, 255]);

    assert.deepEqual(x.add(y).to_8bit(), [1, 255, 255, 255, 255, 255, 254]);

    // x.should.eql([255, 255, 255, 255, 255, 255]);
    // y.should.eql([255, 255, 255, 255, 255, 255]);
  });

  it("Should add positive to negative number expecting positive result", function () {
    var x = Crunchy.from_8bit([51, 254, 144, 207]),
        y = Crunchy.from_8bit([-20, 89, 145, 32]);

    assert.deepEqual(x.add(y).to_8bit(), [31, 164, 255, 175]);

    // x.should.eql([51, 254, 144, 207]);
    // y.should.eql([-20, 89, 145, 32]);
  });

  it("Should add positive to negative number expecting negative result", function () {
    var x = Crunchy.from_8bit([242, 62]),
        y = Crunchy.from_8bit([-242, 64]);

    assert.deepEqual(x.add(y).to_8bit(), [-2]);

    // x.should.eql([242, 62]);
    // y.should.eql([-242, 64]);
  });

  it('Should add negative to positive number expecting positive result', function () {
    var x = Crunchy.from_8bit([-42, 2]),
        y = Crunchy.from_8bit([242, 62]);

    assert.deepEqual(x.add(y).to_8bit(), [200, 60]);

    // x, [-42, 2]);
    // y, [242, 62]);
  });

  it("Should add negative to positive number expecting negative result", function () {
    var x = Crunchy.from_8bit([-42, 2]),
        y = Crunchy.from_8bit([42, 0]);

    assert.deepEqual(x.add(y).to_8bit(), [-2]);
    // x, [-42, 2]);
    // y, [42, 0]);
  });

  it("Should add two negative numbers expecting negative result", function () {
    var x = Crunchy.from_8bit([-242, 62]),
        y = Crunchy.from_8bit([-42, 2]);

    assert.deepEqual(x.add(y).to_8bit(), [-1, 28, 64]);

    // x, [-242, 62]);
    // y, [-42, 2]);
  });

  it("Should add zero to number", function () {
    var x = Crunchy.from_8bit([26, 255, 230, 17]),
        y = Crunchy.from_8bit([0]);

    assert.deepEqual(x.add(y).to_8bit(), [26, 255, 230, 17]);

    // x, [26, 255, 230, 17]);
    // y, [0]);
  });

  it("Should add number to zero", function () {
    var x = Crunchy.from_8bit([0]),
        y = Crunchy.from_8bit([43, 123, 200, 1]);

    assert.deepEqual(x.add(y).to_8bit(), [43, 123, 200, 1]);

    // x, [0]);
    // y, [43, 123, 200, 1]);
  });

  it("Should add two zeros", function () {
    var x = Crunchy.from_8bit([0]),
        y = Crunchy.from_8bit([0]);

    assert.deepEqual(x.add(y).to_8bit(), [0]);

    // x.should.eql([0]);
    // y.should.eql([0]);
  });

});


describe("#add-from_string", function() {

  it("Should add numbers", function() {
    // console.log(new Date());
    let x = Crunchy.from_string("12345678911234567891");
    // console.log(new Date());
    let y = Crunchy.from_string("12345678901234567890");
    // console.log(new Date(), x, y);
    // console.log("addddddd:", new Date(), x, x.toString(), y, y.toString(), x.add(y).toString());
    assert.equal(x.add(y).toString(), "24691357812469135781");
    // console.log(new Date());
    // this.done();
  });

  it("Should add numbers, first longer than second", function() {
    let x = Crunchy.from_string("12345678901234567890");
    let y = Crunchy.from_string("1234567890");
    assert.equal(x.add(y).toString(), "12345678902469135780");
    // this.done();
  });

  it("Should add numbers, second longer than first", function() {
    let x = Crunchy.from_string("1234567890");
    let y = Crunchy.from_string("12345678901234567890");
    assert.equal(x.add(y).toString(), "12345678902469135780");
    // this.done();
  });

  it("Should add two identical numbers", function() {
    let x = Crunchy.from_string("12345678901234567890");
    let y = Crunchy.from_string("12345678901234567890");
    assert.equal(x.add(y).toString(), "24691357802469135780");
    // this.done();
  });

  it("Should add zero to number", function() {
    let x = Crunchy.from_string("12345678901234567890");
    let y = Crunchy.from_string("0");
    assert.equal(x.add(y).toString(), "12345678901234567890");
    // this.done();
  });

  it("Should add number to zero", function() {
    let x = Crunchy.from_string("0");
    let y = Crunchy.from_string("12345678901234567890");
    assert.equal(x.add(y).toString(), "12345678901234567890");
    // this.done();
  });

  it("Should add two zeros", function() {
    let x = Crunchy.from_string("0");
    let y = Crunchy.from_string("0");
    assert.equal(x.add(y).toString(), "0");
    // this.done();
  });

});

describe("#sub", function() {

  it("Should subtract numbers", function() {
    let x = Crunchy.from_8bit([170, 1, 79, 119, 242, 62]),
        y = Crunchy.from_8bit([17, 241, 123, 250, 42, 2]);

    // console.log("Should subtract numbers:", x, y, x.sub(y));
    assert.deepEqual(x.sub(y).to_8bit(), [152, 15, 211, 125, 200, 60]);
    // x.to_8bit().should.eql([170, 1, 79, 119, 242, 62]);
    // y.should.eql([17, 241, 123, 250, 42, 2]);
  });

  it("Should subtract one", function() {
    let x = Crunchy.from_8bit([240, 0, 0, 0, 0, 0]),
        y = Crunchy.from_8bit([1]);

    assert.deepEqual(x.sub(y).to_8bit(), [239, 255, 255, 255, 255, 255]);

    // x.should.eql([240, 0, 0, 0, 0, 0]);
    // y.should.eql([1]);
  });

  it("Should subtract numbers expecting negative result", function() {
    let x = Crunchy.from_8bit([26]),
        y = Crunchy.from_8bit([255]);

    assert.deepEqual(x.sub(y).to_8bit(), [-229]);

    // x.should.eql([26]);
    // y.should.eql([255]);
  });

  it("Should subtract numbers expecting negative result", function() {
    let x = Crunchy.from_8bit([188, 196]),
        y = Crunchy.from_8bit([188, 197]);

    assert.deepEqual(x.sub(y).to_8bit(), [-1]);

    // x.should.eql([188, 196]);
    // y.should.eql([188, 197]);
  });

  it("Should subtract negative number from positive", function() {
    let x = Crunchy.from_8bit([240, 0, 0, 0, 0, 0]),
        y = Crunchy.from_8bit([-1]);
    // console.log("negative:", x, y, x.sub(y));
    assert.deepEqual(x.sub(y).to_8bit(), [240, 0, 0, 0, 0, 1]);

    // x.should.eql([240, 0, 0, 0, 0, 0]);
    // y.should.eql([-1]);
  });

  it("Should subtract positive number from negative", function() {
    let x = Crunchy.from_8bit([-240, 0, 0, 0, 0, 0]),
        y = Crunchy.from_8bit([1]);

    assert.deepEqual(x.sub(y).to_8bit(), [-240, 0, 0, 0, 0, 1]);

    // x.should.eql([-240, 0, 0, 0, 0, 0]);
    // y.should.eql([1]);
  });

  it("Should subtract two negative numbers", function() {
    let x = Crunchy.from_8bit([-240, 0, 0, 0, 0, 0]),
        y = Crunchy.from_8bit([-1]);

    assert.deepEqual(x.sub(y).to_8bit(), [-239, 255, 255, 255, 255, 255]);

    // x.should.eql([-240, 0, 0, 0, 0, 0]);
    // y.should.eql([-1]);
  });

  it("Should subtract two equal numbers expecting zero", function() {
    let x = Crunchy.from_8bit([20]),
        y = Crunchy.from_8bit([20]);

    assert.deepEqual(x.sub(y).to_8bit(), [0]);

    // x.should.eql([20]);
    // y.should.eql([20]);
  });

  it("Should subtract zero", function() {
    let x = Crunchy.from_8bit([244, 137, 7, 161]),
        y = Crunchy.from_8bit([0]);

    assert.deepEqual(x.sub(y).to_8bit(), [244, 137, 7, 161]);

    // x.should.eql([244, 137, 7, 161]);
    // y.should.eql([0]);
  });

  it("Should subtract zero from zero", function() {
    let x = Crunchy.from_8bit([0]),
    y = Crunchy.from_8bit([0]);

    assert.deepEqual(x.sub(y).to_8bit(), [0]);

    // x.should.eql([0]);
    // y.should.eql([0]);
  });

  it("Should subtract from zero", function() {
    let x = Crunchy.from_8bit([0]),
        y = Crunchy.from_8bit([15]);

    assert.deepEqual(x.sub(y).to_8bit(), [-15]);

    // x.should.eql([0]);
    // y.should.eql([15]);
  });

});

describe("#mul", function() {

  it("Should multiply numbers", function() {
    let x = Crunchy.from_8bit([162, 51, 95]),
        y = Crunchy.from_8bit([42, 18, 204]);

    assert.deepEqual(x.mul(y).to_8bit(), [26, 168, 86, 115, 157, 180]);
  });

  it("Should multiply numbers", function() {
    let x = Crunchy.from_8bit([255, 65, 34, 51, 95]),
        y = Crunchy.from_8bit([42, 18, 204]);

    assert.deepEqual(x.mul(y).to_8bit(), [41, 243, 109, 152, 188, 115, 157, 180]);
  });

  it("Should multiply numbers", function() {
    let x = Crunchy.from_8bit([255, 255, 255, 255]),
        y = Crunchy.from_8bit([255, 255, 255, 255]);

    assert.deepEqual(x.mul(y).to_8bit(), [255, 255, 255, 254, 0, 0, 0, 1]);
  });

  it("Should multiply positive with negative number", function() {
    let x = Crunchy.from_8bit([77, 242, 62]),
        y = Crunchy.from_8bit([-42, 2, 113, 43, 57, 65]);

    assert.deepEqual(x.mul(y).to_8bit(), [-12, 202, 124, 133, 146, 125, 36, 79, 190]);
  });

  it("Should multiply negative with positive number", function() {
    let x = Crunchy.from_8bit([-255, 17, 162, 62]),
        y = Crunchy.from_8bit([255, 17, 162, 62]);

    assert.deepEqual(x.mul(y).to_8bit(), [-254, 36, 34, 110, 119, 14, 135, 4]);
  });

  it("Should multiply two negative numbers", function() {
    let x = Crunchy.from_8bit([-162, 51, 95]),
        y = Crunchy.from_8bit([-42, 18, 204]);

    assert.deepEqual(x.mul(y).to_8bit(), [26, 168, 86, 115, 157, 180]);
  });

  it("Should multiply by zero", function() {
    let x = Crunchy.from_8bit([77, 242, 62]),
        y = Crunchy.from_8bit([0]);

    assert.deepEqual(x.mul(y).to_8bit(), [0]);
  });

  it("Should multiply zero by number", function() {
    let x = Crunchy.from_8bit([0]),
        y = Crunchy.from_8bit([77, 242, 62]);

    assert.deepEqual(x.mul(y).to_8bit(), [0]);
  });

  it("Should multiply zeros", function() {
    let x = Crunchy.from_8bit([0]),
        y = Crunchy.from_8bit([0]);

    assert.deepEqual(x.mul(y).to_8bit(), [0]);
  });

});



describe("#div", function () {

  it("Should divide numbers", function () {
    let x = Crunchy.from_8bit([52, 155, 168, 23, 6, 85]),
      y = Crunchy.from_8bit([19, 26, 247]);

    assert.deepEqual(x.div(y).to_8bit(), [2, 192, 234, 136]);
  });

  it("Should divide by one", function() {
    let x = Crunchy.from_8bit([15, 127, 73, 1]),
        y = Crunchy.from_8bit([1]);

    assert.deepEqual(x.div(y).to_8bit(), [15, 127, 73, 1]);
  });

  it("Should divide by self", function() {
    let x =  Crunchy.from_8bit([15, 127, 73, 1]),
        y =  Crunchy.from_8bit([15, 127, 73, 1]);

    assert.deepEqual(x.div(y).to_8bit(), [1]);
  });

  it("Should divide negative number", function() {
    let x =  Crunchy.from_8bit([-170, 153, 136]),
        y =  Crunchy.from_8bit([17, 68]);

    assert.deepEqual(x.div(y).to_8bit(), [-9, 225]);
  });

  it("Should divide by negative number", function() {
    let x = Crunchy.from_8bit([170, 153, 136, 119, 102, 85]),
        y = Crunchy.from_8bit([-17, 68]);

    assert.deepEqual(x.div(y).to_8bit(), [-9, 225, 129, 255, 9]);
  });

  it("Should divide two negative numbers", function() {
    let x =  Crunchy.from_8bit([-52, 155, 168, 23, 6, 85]),
        y =  Crunchy.from_8bit([-19, 26, 247]);

    assert.deepEqual(x.div(y).to_8bit(), [2, 192, 234, 136]);
  });

  it("Should divide zero", function() {
    let x =  Crunchy.from_8bit([0]),
        y =  Crunchy.from_8bit([17, 68]);

    assert.deepEqual(x.div(y).to_8bit(), [0]);
  });

  it("Shouldn't divide by zero", function() {
    let x =  Crunchy.from_8bit([170, 153]),
        y =  Crunchy.from_8bit([0]);

    assert.isNotOk(x.div(y));
  });

  it("Shouldn't divide zero by zero", function() {
    let x =  Crunchy.from_8bit([0]),
        y =  Crunchy.from_8bit([0]);

    assert.isNotOk(x.div(y));
  });

});

// describe("#mod", function() {

//   it("Should calculate modulo", function() {
//     let x = [52, 155, 168, 23, 6, 85],
//         y = [19, 26, 247];

//     Crunch.mod(x, y).should.eql([10, 237, 29]);

//     x.should.eql([52, 155, 168, 23, 6, 85]);
//     y.should.eql([19, 26, 247]);
//   });

//   it("Should calculate modulo of number smaller than modulus", function() {
//     let x = [1, 0],
//         y = [1, 241];

//     Crunch.mod(x, y).should.eql([1, 0]);

//     x.should.eql([1, 0]);
//     y.should.eql([1, 241]);
//   });

//   it("Should calculate modulo of modulus", function() {
//     let x = [1, 241],
//         y = [1, 241];

//     Crunch.mod(x, y).should.eql([0]);

//     x.should.eql([1, 241]);
//     y.should.eql([1, 241]);
//   });

//   it("Should calculate modulo of zero", function() {
//     let x = [0],
//         y = [1, 241];

//     Crunch.mod(x, y).should.eql([0]);

//     x.should.eql([0]);
//     y.should.eql([1, 241]);
//   });

//   // it("Should calculate modulo of negative", function() {
//   //   let x = [-4],
//   //       y = [3];

//   //   Crunch.mod(x, y).should.eql([2]);

//   //   x.should.eql([-4]);
//   //   y.should.eql([3]);
//   // });

//   // it("Should calculate modulo of negative", function() {
//   //   let x = [ -10, 18 ],
//   //       y = [ 123 ];

//   //   Crunch.mod(x, y).should.eql([ 5 ]);

//   //   x.should.eql([ -10, 18 ]);
//   //   y.should.eql([ 123 ]);
//   // });

//   // it("Should calculate modulo of negative", function() {
//   //   let x = [ -15, 62, 62, 18, 176 ],
//   //       y = [ 139, 207, 194, 82 ];

//   //   Crunch.mod(x, y).should.eql([ 12, 123, 46, 72 ]);

//   //   x.should.eql([ -15, 62, 62, 18, 176 ]);
//   //   y.should.eql([ 139, 207, 194, 82 ]);
//   // });

// });


describe("#leftShift", function() {

  it("Should left shift a number", function() {
    let x = Crunchy.from_8bit([22, 11]),
        s = 5;

    assert.deepEqual(x.shl(s).to_8bit(), [2, 193, 96]);
  });

  it("Should left shift a number more", function() {
    let x = Crunchy.from_8bit([1]),
        s = 64;

    assert.deepEqual(x.shl(s).to_8bit(), [1,0,0,0,0,0,0,0,0]);
  });

  it("Should left shift zero", function() {
    let x = Crunchy.from_8bit([0]),
        s = 80;

    assert.deepEqual(x.shl(s).to_8bit(), [0]);
  });

  it("Should left shift negative", function() {
    let x = Crunchy.from_8bit([-3]),
        s = 8;

    assert.deepEqual(x.shl(s).to_8bit(), [-3, 0]);
  });

  it("Should left shift one", function() {
    let x = Crunchy.from_8bit([1, 0, 0, 0, 0]),
        s = 1;

    assert.deepEqual(x.shl(s).to_8bit(), [2, 0, 0, 0, 0]);
  });

});

describe("#rightShift", function() {

  it("Should right shift a number to zero", function() {
    let x = Crunchy.from_14bit([167837953])
    // assert.deepEqual(x.rsh(32).num, [0]);
    assert.deepEqual(x.shr(32).num, [0]);
    assert.deepEqual(x.shr(33).num, [0]);
  });


  it("Should right shift a number", function() {
    let x = Crunchy.from_8bit([22, 11]),
        s = 5;

    assert.deepEqual(x.shr(s).to_8bit(), [176]);
  });

  it("Should right shift a number out of existance", function() {
    let x = Crunchy.from_8bit([22, 11]),
        s = 20;

    assert.deepEqual(x.shr(s).to_8bit(), [0]);
  });

  it("Should right shift a negative number", function() {
    let x = Crunchy.from_8bit([-3]),
        s = 8;
    assert.deepEqual(x.shr(s).to_8bit(),[-0]);
  });

});

// describe("#parse", function() {

//   it("Should convert string to integer", function() {
//     let s = "6315359056060240643798024";

//     Crunchy.parse(s).to_8bit().should.eql([5, 57, 84, 76, 233, 0, 120, 91, 180, 180, 8]);

//     s.should.equal("6315359056060240643798024");
//   });

//   it("Should convert string negative number to integer", function() {
//     let s = "-256";

//     Crunchy.parse(s).to_8bit().should.eql([-1, 0]);

//     s.should.equal("-256");
//   });

// });

// describe("#stringify", function() {

//   it("Should convert integer to string", function() {
//     let x = [5, 57, 84, 76, 233, 0, 120, 91, 180, 180, 8];

//     Crunchy.from_8bit(x).to_8bit().should.equal("6315359056060240643798024");

//     x.should.eql([5, 57, 84, 76, 233, 0, 120, 91, 180, 180, 8]);
//   });

// });

