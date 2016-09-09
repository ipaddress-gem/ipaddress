#include <iostream>

#include "option.hpp"

#include "rle.hpp"

std::ostream& operator<<(std::ostream &o, const Rle &rle) {
    o << rle.toString();
    return o;
}


class Last {
public:
    Option<Rle> val;
    typedef std::map<size_t, std::vector<size_t>> IdxMap;
    IdxMap max_poses;
    std::vector<Rle> ret;

    Last() {}

    void handle_last() {
        if (this->val.isNone()) {
            return;
        }
        auto &_last = this->val.unwrap();
        // std::cout << "handle_last:" << _last.part << std::endl;
        auto max_rles = this->max_poses.find(_last.part);
        if (max_rles == this->max_poses.end()) {
            max_rles = this->max_poses.insert(IdxMap::value_type(_last.part, std::vector<size_t>())).first;
        }
        // console.log(_last.part, this->max_poses);
        for (auto idx : max_rles->second) {
            // std::cout << "max_rles:" << idx;
            Rle &prev = this->ret[idx];
            if (prev.cnt > _last.cnt) {
                // console.log(`>>>>> last=${_last}->${idx}->prev=${prev}`);
                // std::cout << "_last.max=" << idx << " " << _last << std::endl;
                _last.max = false;
            } else if (prev.cnt == _last.cnt) {
                // nothing
            } else if (prev.cnt < _last.cnt) {
                // console.log(`<<<<< last=${_last}->${idx}->prev=${prev}`);
                // std::cout << "prev.max=" << idx << " " << prev << std::endl;
                prev.max = false;
            }
        }
        //println!("push:{}:{:?}", self.ret.len(), _last);
        max_rles->second.push_back(this->ret.size());
        _last.pos = this->ret.size();
        this->ret.push_back(_last);
    }
};

std::vector<Rle> Rle::code(const std::vector<size_t> &parts) {
    Last last;
    // println!("code");
    for (auto part : parts) {
        // console.log(`part:${part}`);
        if (last.val.isSome() && last.val.unwrap().part == part) {
            last.val.unwrap().cnt += 1;
        } else {
            last.handle_last();
            Rle rle(part, 0, 1, true);
            last.val = Some(rle);
        }
    }
    last.handle_last();
    return last.ret;
}
