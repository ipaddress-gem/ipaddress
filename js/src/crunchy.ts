

import * as Crunch from 'number-crunch';


class Crunchy {
  num: number[];

  public clone() : Crunchy {
    let ret = new Crunchy();
    ret.num = this.num.slice();
    return ret;
  }

  public static from_crunch(num: number[]) : Crunchy {
    let ret = new Crunchy();
    ret.num = num;
    return ret;
  }

  public static parse(val: string) : Crunchy  {
    let ret = new Crunchy();
    ret.num = Crunch.parse(val);
    return ret;
  }

  public static from_number(val: number) : Crunchy {
    return Crunchy.parse(""+val);
  }
  public static from_string(val: string, radix : number = 10) : Crunchy {
    let x = val.split("");
    let p = Crunchy.one();
    let a = Crunchy.zero();
    let b = Crunchy.parse(""+radix);
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
      a = a.add(p.mul(Crunchy.parse(""+c)));
      p = p.mul(b);
    }
    // a.num.negative = n;
    return a;
  }

  

  public eq(oth: Crunchy) : boolean {
    return Crunch.compare(this.num, oth.num) == 0;
  }

  public lte(oth: Crunchy) : boolean {
    return Crunch.compare(this.num, oth.num) <= 0;
  }
  public lt(oth: Crunchy) : boolean {
    return Crunch.compare(this.num, oth.num) < 0;
  }

  public gt(oth: Crunchy) : boolean {
    return Crunch.compare(this.num, oth.num) > 0;
  }
  public gte(oth: Crunchy) : boolean {
    return Crunch.compare(this.num, oth.num) >= 0;
  }

  public shl(num: number) : Crunchy {
    return Crunchy.from_crunch(Crunch.leftShift(this.num, num));
  }

  public shr(num: number) : Crunchy {
    //console.log("shr<<", num, this.num);
    let res = Crunch.rightShift(this.num, num);
    // console.log("shr>>", num, this.num, res);
    return Crunchy.from_crunch(res);
  }

  public add(cry: Crunchy) : Crunchy {
    return Crunchy.from_crunch(Crunch.add(this.num, cry.num));
  }
  public mul(cry: Crunchy) : Crunchy {
    return Crunchy.from_crunch(Crunch.mul(this.num, cry.num));
  }

  public sub(cry: Crunchy) : Crunchy {
    return Crunchy.from_crunch(Crunch.sub(this.num, cry.num));
  }

  public mod(cry: Crunchy) : Crunchy {
    return Crunchy.from_crunch(Crunch.mod(this.num, cry.num));
  }

  public mds(mod: number) : number {
    return Crunchy._mds(this.num, mod);
  }

  static _mds(x: number[], n: number) : number {
    for (var i = 0, z = 0, l = x.length; i < l; i++) {
      z = ((x[i] >> 14) + (z << 14)) % n;
      z = ((x[i] & 16383) + (z << 14)) % n;
    }
    return z;
  }

  public toString(radix: number = 10) : string {
    let a: string[] = [], i = 0;
    let x = this.num.slice();
    do {
      let digit = Crunchy._mds(x, radix);
      // console.log("0-toString:", x, radix, digit, a);
      x      = Crunch.div(x, [radix]);
      // console.log("1-toString:", x, radix, digit, a);
      a[i++] = "0123456789abcdef"[digit];
      // console.log("2-toString:", x, radix, digit, a, Crunch.compare(x, Crunchy._zero.num));
    } while (Crunch.compare(x, Crunchy._zero.num));
    // console.log("Crunchy.tostring-1:", a);
    let ret = a.reverse().join("");
    // console.log("Crunchy.tostring-2:", ret, a);
    return ret;
  }

  // public toNumber() : number {
  //   return null;
  // }


  static _zero = Crunchy.from_number(0);
  public static zero() : Crunchy {
    return Crunchy._zero;
  }
  static _one = Crunchy.from_number(1);
  public static one() : Crunchy {
    return Crunchy._one;
  }

  static _two = Crunchy.from_number(2);
  public static two() : Crunchy {
    return Crunchy._two;
  }

}

export default Crunchy;
