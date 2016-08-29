

import * as Crunch from 'number-crunch';


class Crunchy {
  num: number[];

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
    return null;
  }

  public clone() : Crunchy {
    let ret = new Crunchy();
    ret.num = this.num.slice();
    return ret;
  }

  public eq(oth: Crunchy) : boolean {
    return Crunch.cmp(this.num, oth.num) == 0;
  }

  public lte(oth: Crunchy) : boolean {
    return Crunch.cmp(this.num, oth.num) <= 0;
  }
  public lt(oth: Crunchy) : boolean {
    return Crunch.cmp(this.num, oth.num) == -1;
  }

  public gt(oth: Crunchy) : boolean {
    return Crunch.cmp(this.num, oth.num) == 1;
  }

  public shl(num: number) : Crunchy {
    return Crunchy.from_crunch(Crunch.leftShift(this.num, num));
  }

  public shr(num: number) : Crunchy {
    return Crunchy.from_crunch(Crunch.rightShift(this.num, num));
  }

  public add(cry: Crunchy) : Crunchy {
    return Crunchy.from_crunch(Crunch.add(this.num, cry.num));
  }

  public sub(cry: Crunchy) : Crunchy {
    return Crunchy.from_crunch(Crunch.sub(this.num, cry.num));
  }

  public mod(cry: Crunchy) : Crunchy {
    return Crunchy.from_crunch(Crunch.mod(this.num, cry.num));
  }

  static mds(x: number[], n: number) : number {
    for (var i = 0, z = 0, l = x.length; i < l; i++) {
      z = ((x[i] >> 14) + (z << 14)) % n;
      z = ((x[i] & 16383) + (z << 14)) % n;
    }
    return z;
  }

  public toString(radix: number = 10) : string {
    let a: string[], i = 0;
    let x = this.num.slice();
    do {
      let digit = Crunchy.mds(x, radix);
      x      = Crunch.div(x, [radix]);
      a[i++] = "0123456789abcdef"[digit];
    } while (Crunch.cmp(x, Crunchy._zero));
    return a.reverse().join("");
  }

  public toNumber() : number {
    return null;
  }


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
