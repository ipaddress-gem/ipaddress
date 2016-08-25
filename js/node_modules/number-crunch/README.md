Crunch [![Build Status](https://img.shields.io/travis/vukicevic/crunch.svg?style=flat-square)](https://semaphoreci.com/vukicevic/crunch) [![NPM version](https://img.shields.io/npm/v/number-crunch.svg?style=flat-square)](https://www.npmjs.com/package/number-crunch) ![Bower version](https://img.shields.io/bower/v/crunch.svg?style=flat-square) ![License](https://img.shields.io/npm/l/number-crunch.svg?style=flat-square)
======
Crunch is an [arbitrary-precision integer arithmetic](http://en.wikipedia.org/wiki/Arbitrary-precision_arithmetic) library for JavaScript.

* [Homepage](http://crunch.js.org/)
* [Examples](http://crunch.js.org/examples/)
* [Tests](http://crunch.js.org/tests/)

It was designed to execute arithmetic operations as quickly as possible, in particular those upon which asymmetric encryption cryptosystems such as RSA are built.

Usage
-----
Crunch can be loaded as a classic browser script

```html
<script src="crunch.js"></script>
<script>
var crunch = Crunch();
</script>
```

or in a web worker

```javascript
var crunch = new Worker("crunch.js");
```

or it can be used as a node module

```javascript
npm install number-crunch
```

```javascript
var crunch = require("number-crunch");
```

**Example 1**
```javascript
x = [10, 123, 21, 127];
y = [4, 211, 176, 200];
crunch.add(x, y); //[15, 78, 198, 71]
```

The library accepts and returns 8-bit integer arrays which represent artbitrary-precision (big) integers, but internally it uses 28-bit arrays and performs the conversions automatically.

Crunch also converts the between big integer byte-array representation and base-10 strings (a string is used as the Number type could not represent large numbers) using the .stringify() and .parse() functions.


**Example 2**
```javascript
crunch.stringify([1,2,3,4,5,6,7,8,9,0]); // "4759477275222530853120"

crunch.parse("4759477275222530853120");  // [1,2,3,4,5,6,7,8,9,0]
```


Functions
----

Function | Input Parameters | Output
--- | --- | ---
add | x, y | x + y
sub | x, y | x - y
mul | x, y | x * y
mulk | x, y | x * y [Karatsuba algorithm](https://en.wikipedia.org/wiki/Karatsuba_algorithm)
div | x, y | x / y
sqr | x | x * x
mod | x, y | x % y
bmr | x, y, [mu] | x % y
exp | x, e, n | x^e % n
gar | x, p, q, d, u, [dp1], [dq1] | x^d % pq
inv | x, y | 1/x % y
cut | x | Remove leading zeroes of x
zero | x | Return zero array of length x
and | x, y | x AND y
or | x, y | x OR y
xor | x, y | x XOR y
not | x | NOT x
leftShift | x, s | x << s
rightShift<sup>[1](#footnote1)</sup> | x, s | x >>> s
compare | x, y | -1: x < y, 0: x = y, 1: x > y
decrement | x | x - 1
factorial | n | n! [n < 268435456]
nextPrime | x | First prime after x
testPrime | x | Boolean x is prime
stringify | x | String (base 10 representation)
parse | s | Arbitrary-precision integer
transform | x, [toRaw] | Radix conversion

<a name="footnote1">1</a>: Be aware, negative numbers retain their sign but Crunch uses a form of signed-magnitude representation rather than two's complement representation for negative numbers. Right shifting will not produce the same result as when it is done to complement representation. So: -4 >> 5 will be -0, not -1.

Miller-Rabin primality testing `mrb`, simple mod `mds` and greatest common divisor `gcd` are also implemented as internal methods not exposed via the Crunch object.

Web Workers
----

Crunch can be loaded in a Web Worker. Instruction messages are sent to the worker in the following format:

```javascript
{func: "", args: []}
```

**Example 3**

```javascript
var crunch = new Worker("crunch.js");
var message = {func: "add", args: [[10, 123, 21, 127], [4, 211, 176, 200]]};

crunch.onmessage = function(m) {
	console.log(m);
};

crunch.postMessage(message);
```