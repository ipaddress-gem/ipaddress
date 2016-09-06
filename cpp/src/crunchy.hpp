#ifndef __CRUNCHY__
#define __CRUNCHY__
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

class Crunchy {
  // size_t num: number[] = [];
  // negative: boolean = false;

  static zeroes: number[] = ((n: number): number[] => {
    let z: number[] = [];
    for (; z.push(0) < n;) { }
    return z;
  })(60);


  public clone(): Crunchy {
    let ret = new Crunchy();
    ret.num = this.num.slice();
    ret.negative = this.negative;
    return ret;
  }

  public static removeLeadingZeros(inn: number[]): number[] {
    let out = inn.slice();
    while (out[0] === 0 && out.length > 1) {
      out.shift();
    }
    return out;//.transformOut();
  }

  public static from_14bit(a: number[]): Crunchy {
    let ret = new Crunchy();
    ret.num = a;
    return ret;
  }

  public static from_8bit(a: number[]): Crunchy {
    let x = [0, 0, 0, 0, 0, 0].slice((a.length - 1) % 7);
    let z = new Crunchy();

    if (a[0] < 0) {
      a[0] *= -1;
      z.negative = true;
    } else {
      z.negative = false;
    }
    x = x.concat(a);
    for (let i = 0; i < x.length; i += 7) {
      z.num.push((x[i] * 1048576 + x[i + 1] * 4096 + x[i + 2] * 16 + (x[i + 3] >> 4)), ((x[i + 3] & 15) * 16777216 + x[i + 4] * 65536 + x[i + 5] * 256 + x[i + 6]));
    }
    z.num = Crunchy.removeLeadingZeros(z.num);
    return z;
  }

  public static parse(val: string): Crunchy {
    return Crunchy.from_string(val, 10);
  }

  public static from_number(val: number): Crunchy {
    return Crunchy.parse("" + val);
  }
  public static from_string(val: string, radix: number = 10): Crunchy {
    let x = val.split("");
    let p = Crunchy.one();
    let a = Crunchy.zero();
    let b = Crunchy.from_8bit([radix]);
    // console.log("fromstring:one:", p);
    // console.log("fromstring:zero:", a);
    // console.log("fromstring:radix:", b);
    let n = false;

    if (x[0] === "-") {
      n = true;
      x.shift();
    }
    while (x.length) {
      let c = parseInt(x.pop(), radix);
      if (isNaN(c)) {
        console.error("from_string:", val);
        return null;
      }
      // console.log(a,p,b);
      a = a.add(p.mul(Crunchy.from_8bit([c])));
      p = p.mul(b);
    }
    a.negative = n;
    return a;
  }

  public to_8bit(): number[] {
    let x = [0].slice((this.num.length - 1) % 2).concat(this.num);
    let z: number[] = [];
    // z.num = [];

    for (let i = 0; i < x.length;) {
      let u = x[i++];
      let v = x[i++];

      z.push(
        (u >> 20),
        (u >> 12 & 255),
        (u >> 4 & 255),
        ((u << 4 | v >> 24) & 255),
        (v >> 16 & 255),
        (v >> 8 & 255),
        (v & 255)
      );
    }
    // console.log("co:", a, z);
    z = Crunchy.removeLeadingZeros(z);

    if (this.negative) {
      z[0] *= -1;
    }
    return z;
  }

  public compare(y: Crunchy): number {
    let xl = this.num.length,
      yl = y.num.length; //zero front pad problem

    if (xl < yl) {
      return -1;
    } else if (xl > yl) {
      return 1;
    }

    for (let i = 0; i < xl; i++) {
      //console.log("x=y:", i, x, y);
      if (this.num[i] < y.num[i]) return -1;
      if (this.num[i] > y.num[i]) return 1;
    }

    return 0;
  }

  public eq(oth: Crunchy): boolean {
    return this.compare(oth) == 0;
  }

  public lte(oth: Crunchy): boolean {
    return this.compare(oth) <= 0;
  }
  public lt(oth: Crunchy): boolean {
    return this.compare(oth) < 0;
  }

  public gt(oth: Crunchy): boolean {
    return this.compare(oth) > 0;
  }
  public gte(oth: Crunchy): boolean {
    return this.compare(oth) >= 0;
  }

  public add(y: Crunchy): Crunchy {
    let z: Crunchy;
    if (this.negative) {
      if (y.negative) {
        z = this.unsigned_add(y);
        z.negative = true;
      } else {
        z = y.unsigned_sub(this, false).cut();
      }
    } else {
      z = y.negative ? this.unsigned_sub(y, false).cut() : this.unsigned_add(y);
    }
    return z;
  }

  public unsigned_add(_y: Crunchy): Crunchy {
    let n = this.num.length,
      t = _y.num.length,
      i = Math.max(n, t),
      c = 0,
      z = Crunchy.zeroes.slice(0, i);
    // console.log("add:1:", new Date());
    let x = this.num;
    let y = _y.num;
    if (n < t) {
      x = Crunchy.zeroes.slice(0, t - n).concat(this.num);
    } else if (n > t) {
      y = Crunchy.zeroes.slice(0, n - t).concat(y);
    }
    // console.log("add:2:", new Date());
    for (i -= 1; i >= 0; i--) {
      z[i] = x[i] + y[i] + c;

      if (z[i] > 268435455) {
        c = 1;
        z[i] -= 268435456;
      } else {
        c = 0;
      }
    }

    // console.log("add:3:", new Date());
    if (c === 1) {
      z.unshift(c);
    }
    let ret = new Crunchy();
    ret.num = z;
    // console.log("add:4:", new Date());
    return ret;
  }

  public sub(y: Crunchy): Crunchy {
    let z: Crunchy;
    if (this.negative) {
      if (y.negative) {
        // console.log("sub-c1");
        z = y.unsigned_sub(this, false).cut();
      } else {
        // console.log("sub-c2");
        z = this.unsigned_add(y);
        z.negative = true;
      }
    } else {
      // console.log("sub-c3", this, y);
      z = y.negative ? this.unsigned_add(y) : this.unsigned_sub(y, false).cut();
    }
    return z;
  }

  public unsigned_sub(_y: Crunchy, internal: boolean = false): Crunchy {
    let n = this.num.length,
      t = _y.num.length,
      i = Math.max(n, t),
      c = 0,
      z = Crunchy.zeroes.slice(0, i);
    let x = this.num;
    let y = _y.num;
    if (n < t) {
      x = Crunchy.zeroes.slice(0, t - n).concat(x);
    } else if (n > t) {
      y = Crunchy.zeroes.slice(0, n - t).concat(y);
    }
    for (i -= 1; i >= 0; i--) {
      z[i] = x[i] - y[i] - c;
      // console.log(z, x, y);

      if (z[i] < 0) {
        c = 1;
        // console.log("pre:+", z[i], z);
        z[i] += 268435456;
        // console.log("pre:-", z[i], z);
      } else {
        c = 0;
      }
    }

    let cry = new Crunchy();
    cry.num = z;
    if (c === 1 && !internal) {
      let zero = new Crunchy();
      zero.num = Crunchy.zeroes.slice(0, z.length);
      cry = zero.unsigned_sub(cry, true);
      cry.negative = true;
    }
    return cry;
  }
  public lsh(s: number): Crunchy {
    var ss = s % 28,
      ls = Math.floor(s / 28),
      l = this.num.length,
      z: Crunchy = this.clone(),
      t = 0;

    if (ss) {
      z.num = [];
      while (l--) {
        z.num[l] = ((this.num[l] << ss) + t) & 268435455;
        t = this.num[l] >>> (28 - ss);
      }

      if (t !== 0) {
        z.num.unshift(t);
      }

      z.negative = this.negative;

    }
    if (ls) {
      z.num = z.num.concat(Crunchy.zeroes.slice(0, ls));
      // return (ls) ? z.concat(zeroes.slice(0, ls)) : z;
    }
    // console.log(this, s, z);
    return z;
  }


  // public leftShift(s: number): Crunchy {
  //   return this.transformIn()

  //   Crunchy.transformOut(Crunch.lsh(.pop(), s));
  // }

  // public shl(num: number): Crunchy {
  //   return Crunchy.from_8bit(Crunch.leftShift(this.num, num));
  // }


  public rsh(s: number): Crunchy {
    let ss = s % 28,
      ls = Math.floor(s / 28),
      l = this.num.length - ls,
      z = this.clone();
    z.num = this.num.slice(0, l);
    if (ss) {
      while (--l >= 0) {
        z.num[l] = ((z.num[l] >> ss) | (z.num[l - 1] << (28 - ss))) & 268435455;
      }

      z.num[l] = z.num[l] >> ss;

      if (z.num[0] === 0) {
        z.num.shift();
      }
    }
    return z;
  }

  // public shr(num: number): Crunchy {
  //   //console.log("shr<<", num, this.num);
  //   let res = this.rightShift(this.num, num);
  //   // console.log("shr>>", num, this.num, res);
  //   return Crunchy.from_8bit(res);
  // }




  public mul(y: Crunchy): Crunchy {
    let yl: number, yh: number, c: number,
      n = this.num.length,
      i = y.num.length,
      z = Crunchy.zeroes.slice(0, n + i);

    while (i--) {
      c = 0;

      yl = y.num[i] & 16383;
      yh = y.num[i] >> 14;

      for (let j = n - 1, xl: number, xh: number, t1: number, t2: number; j >= 0; j--) {
        xl = this.num[j] & 16383;
        xh = this.num[j] >> 14;

        t1 = yh * xl + xh * yl;
        t2 = yl * xl + ((t1 & 16383) << 14) + z[j + i + 1] + c;

        z[j + i + 1] = t2 & 268435455;
        c = yh * xh + (t1 >> 14) + (t2 >> 28);
      }

      z[i] = c;
    }

    if (z[0] === 0) {
      z.shift();
    }
    let ret = new Crunchy();
    ret.negative = this.negative !== y.negative;
    ret.num = z;
    return ret;
  }

  public static msb(x: number): number {
    if (x !== 0) {
      let z = 0;
      for (let i = 134217728; i > x; z++) {
        i /= 2;
      }
      return z;
    }
  }

  // public transformIn(): Crunchy {
  //   // return Array.prototype.slice.call(a).map((v: Crunchy) => {
  //   //   return Crunchy.from_8bit(v.num.slice())
  //   // });
  //   return null;
  // }

  // public transformOut(): Crunchy {
  //   console.log("in-transformOut:", this);
  //   let ret = Crunchy.co(this);
  //   console.log("out-transformOut:", this, ret);
  //   return ret;
  // }

  public shr(s: number): Crunchy {
    let my = this.rsh(s);
    return my.cut();
  }

  public shl(s: number): Crunchy {
    return this.lsh(s).cut();

    // Crunch.transformOut(Crunch.lsh(Crunch.transformIn([x]).pop(), s));
  }
  public cut(): Crunchy {
    let out = this.clone();
    // beasty hack
    if (out.num.length == 0) {
      out.num = [0];
      return out;
    }
    while (out.num[0] === 0 && out.num.length > 1) {
      out.num.shift();
    }
    return out;//.transformOut();
  }
  //   return Crunchy.transformOut(this.num
  //     cut.apply(null, transformIn(arguments))
  //   );
  // }


  public div(y: Crunchy, internal: boolean = false): Crunchy {
    if (y.num.length === 1 && y.num[0] === 0) {
      return null;
    }
    // var u, v, xt, yt, d, q, k, i, z;
    let u: Crunchy;
    let v: Crunchy;
    let s = Crunchy.msb(y.num[0]) - 1;
    if (s > 0) {
      u = this.lsh(s);
      v = y.lsh(s);
    } else {
      u = this.clone();
      v = y.clone();
    }
    let d = u.num.length - v.num.length;
    let q = [0];
    let k = Crunchy.from_14bit(v.num.concat(Crunchy.zeroes.slice(0, d)));
    let yt = v.num[0] * 268435456 + v.num[1];

    // only cmp as last resort
    while (u.num[0] > k.num[0] || (u.num[0] === k.num[0] && u.compare(k) > -1)) {
      q[0]++;
      u = u.unsigned_sub(k, false);
    }

    for (let i = 1; i <= d; i++) {
      q[i] = u.num[i - 1] === v.num[0] ? 268435455 : ~~((u.num[i - 1] * 268435456 + u.num[i]) / v.num[0]);

      let xt = u.num[i - 1] * 72057594037927936 + u.num[i] * 268435456 + u.num[i + 1];
      while (q[i] * yt > xt) { //condition check can fail due to precision problem at 28-bit
        q[i]--;
      }

      k = v.mul(Crunchy.from_14bit([q[i]]));
      k.num = k.num.concat(Crunchy.zeroes.slice(0, d - i)); //concat after multiply, save cycles
      u = u.unsigned_sub(k, false);

      if (u.negative) {
        u = Crunchy.from_14bit(v.num.concat(Crunchy.zeroes.slice(0, d - i))).unsigned_sub(u, false);
        q[i]--;
      }
    }
    let z: Crunchy;
    if (internal) {
      z = (s > 0) ? u.cut().rsh(s) : u.cut();
    } else {
      z = Crunchy.from_14bit(Crunchy.removeLeadingZeros(q));
      z.negative = (this.negative !== y.negative) ? true : false;
    }

    return z;
  }
  // public sub(cry: Crunchy): Crunchy {
  //   return Crunchy.from_8bit(Crunch.sub(this.num, cry.num));
  // }

  public mod(y: Crunchy): Crunchy {

    //For negative x, cmp doesn't work and result of div is negative
    //so take result away from the modulus to get the correct result
    if (this.negative) {
      return y.sub(this.div(y, true));
    }
    switch (this.compare(y)) {
      case -1:
        return this;
      case 0:
        return Crunchy.from_8bit([0]);
      default:
        return this.div(y, true);
    }
  }

  public mds(n: number): number {
    let z = 0;
    for (let i = 0, l = this.num.length; i < l; i++) {
      z = ((this.num[i] >> 14) + (z << 14)) % n;
      z = ((this.num[i] & 16383) + (z << 14)) % n;
    }
    return z;
  }


  public toString(radix: number = 10): string {
    let a: string[] = [], i = 0;
    let x = this.clone();
    // console.log("toString:", new Date());
    let cradix = Crunchy.from_8bit([radix]);
    let zero = Crunchy.zero();
    do {
      let digit = x.mds(radix);
      x = x.div(cradix);
      a[i++] = "0123456789abcdef"[digit];
      // console.log("1-toString:", x, radix, digit, a.join(""));
      // console.log("2-toString:", x, radix, digit, a, Crunch.compare(x, Crunchy._zero.num));
    } while (!x.eq(zero));
    // console.log("Crunchy.tostring-1:", a);
    let ret = a.reverse().join("");
    // console.log("Crunchy.tostring-2:", ret, new Date());
    return ret;
  }

  // public toNumber() : number {
  //   return null;
  // }


  static _zero = Crunchy.from_8bit([0]);
  public static zero(): Crunchy {
    return Crunchy._zero;
  }
  static _one = Crunchy.from_8bit([1]);
  public static one(): Crunchy {
    return Crunchy._one;
  }

  static _two = Crunchy.from_8bit([2]);
  public static two(): Crunchy {
    return Crunchy._two;
  }

}

export default Crunchy;
