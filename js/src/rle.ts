


class Last {
    val: Rle = null;
    max_poses: { [id: string] : any } = {};
    ret: Rle[] = [];

    constructor() {
    }
    public handle_last() {
        if (null == this.val) {
            return;
        }
        let _last = this.val;
        let max_rles = this.max_poses[_last.part]; 
        if (max_rles == null) {
            max_rles = this.max_poses[_last.part] = [];
        }
        for (let idx in max_rles) {
            let prev = this.ret[idx];
            if (prev.cnt > _last.cnt) {
                console.log(`>>>>> last=${_last}->${idx}->prev=${prev}`);
                _last.max = false;
            } else if (prev.cnt == _last.cnt) {
                // nothing
            } else if (prev.cnt < _last.cnt) {
                console.log(`<<<<< last=${_last}->${idx}->prev=${prev}`);
                //self.ret[idx].max = false;
                prev.max = false;
            }
        }
        //println!("push:{}:{:?}", self.ret.len(), _last);
        max_rles.push(this.ret.length);
        _last.pos = this.ret.length;
        this.ret.push(_last);
    }
}

class Rle {
    public part: any;
    public pos: number;
    public cnt: number;
    public max: boolean;

    constructor(obj: { [id: string]: any }) {
        this.part = obj['part'];
        this.pos = obj['pos'];
        this.cnt = obj['cnt'];
        this.max = obj['max'];
    }

    public toString() {
        return `<Rle@part:${this.part},pos:${this.pos},cnt:${this.cnt},max:${this.max}>`;
    }

    public eq(other: Rle): boolean {
        return this.part == other.part && this.pos == other.pos &&
            this.cnt == other.cnt && this.max == other.max;
    }
    public ne(other: Rle): boolean {
        return !this.eq(other);
    }

    public static code(parts: any[]): Rle[] {
        let last = new Last();
        // println!("code");
        for (let i=0; i < parts.length; ++i) {
            let part = parts[i];
            // println!("part:{}", part);
            if (last.val && last.val.part == part) {
                last.val.cnt += 1;
            } else {
                last.handle_last();
                last.val = new Rle({ part: part, pos: 0, cnt: 1, max: true });
            }
        }
        last.handle_last();
        return last.ret;
    }

}

export default Rle;
