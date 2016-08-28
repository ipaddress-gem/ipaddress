

import * as Crunch from 'number-crunch';


class Crunchy {
  num: number[];

  public static from_crunch(num: number[]) : Crunchy {
    let ret = new Crunchy();
    ret.num = num;
    return ret;
  }

  public static parse(val: any) : Crunchy  {
    let ret = new Crunchy();
    ret.num = Crunch.parse(val);
    return ret;
  }

  public static from_number(val: number) : Crunchy {
    return null;
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
    return null;
  }

  public lte(oth: Crunchy) : boolean {
    return null;
  }
  public lt(oth: Crunchy) : boolean {
    return null;
  }

  public gt(oth: Crunchy) : boolean {
    return null;
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

  public toString(num: number = 10) : string {
    return Crunch.stringify(this.num); 
  }

  public toNumber() : number {
    return null;
  }


  static _zero = Crunchy.parse(0);
  public static zero() : Crunchy {
    return Crunchy._zero;
  }
  static _one = Crunchy.parse(1);
  public static one() : Crunchy {
    return Crunchy._one;
  }

  static _two = Crunchy.parse(2)
  public static two() : Crunchy {
    return Crunchy._two;
  }

}

export default Crunchy;
