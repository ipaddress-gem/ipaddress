
#ifndef __RLE__
#define __RLE__


class Last {
public:
    Rle val;
    std::map<size_t, > max_poses;
    std::vector<Rle> ret;

    public handle_last() {
        if (null == this.val) {
            return;
        }
        let _last = this.val;
        let max_rles = this.max_poses[_last.part];
        if (max_rles == null) {
            max_rles = this.max_poses[_last.part] = [];
        }
        // console.log(_last.part, this.max_poses);
        for (let idx in max_rles) {
            let prev = this.ret[max_rles[idx]];
            if (prev.cnt > _last.cnt) {
                // console.log(`>>>>> last=${_last}->${idx}->prev=${prev}`);
                _last.max = false;
            } else if (prev.cnt == _last.cnt) {
                // nothing
            } else if (prev.cnt < _last.cnt) {
                // console.log(`<<<<< last=${_last}->${idx}->prev=${prev}`);
                prev.max = false;
            }
        }
        //println!("push:{}:{:?}", self.ret.len(), _last);
        max_rles.push(this.ret.length);
        _last.pos = this.ret.length;
        this.ret.push(_last);
    }
};

class Rle {
public:
    size_t part;
    size_t pos;
    size_t cnt;
    bool max;

    Rle(size_t part, size_t pos, size_t cnt, bool max) {
        this->part = part;
        this->pos = pos;
        this->cnt = cnt;
        this->max = max;
    }

    // public toString() {
    //     return `<Rle@part:${this.part},pos:${this.pos},cnt:${this.cnt},max:${this.max}>`;
    // }

    bool eq(const Rle &other) const {
        return this->part == other.part && this->pos == other.pos &&
            this->cnt == other.cnt && this->max == other.max;
    }
    bool ne(const Rle &other) const {
        return !this.eq(other);
    }

    static std::vector<Rle> code(const std::vector<size_t> &parts) {
        Last last;
        // println!("code");
        for (size_t i = 0; i < parts.length; ++i) {
            auto part = parts[i];
            // console.log(`part:${part}`);
            if (last.val && last.val.part == part) {
                last.val.cnt += 1;
            } else {
                last.handle_last();
                last.val = Rle(part, 0, 1, true);
            }
        }
        last.handle_last();
        return last.ret;
    }

};

#endif
