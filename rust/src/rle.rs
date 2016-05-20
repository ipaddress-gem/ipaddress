use std::collections::HashMap;

struct Rle {
    part: u16,
    pos: u16,
    cnt: u16,
    max: bool
}

pub fn rle_code(parts: &Vec<u16>) -> Vec<Rle> {
    let mut ret : Vec<Rle> = Vec::new();
    let mut last : Option<Rle> = None;
    let mut max_pos: HashMap<u16, usize> = HashMap::new();
    for i in 0..parts.len() {
        let part = parts.get(i);
            if last.is_some() && last.unwrap().part == part {
                last.unwrap().cnt += 1;
            } else {
                if last.is_some() {
                    let _last = last.unwrap();
                    let max_pos = max_pos.entry(_last.part).or_insert(_last.pos);
                    if _last.pos != max_pos {
                        let prev = ret.get(max_pos).unwrap();
                        if prev.cnt >= last.cnt {
                            last.max = false;
                        } else {
                            prev.max = false;
                        }
                    }
                    ret.push(_last);
                }
                last = Some(Rle { part: part, pos: i, cnt: 1, max: true });
            }
    }
    if !last.is_none() {
        ret.push(last.unwrap());
    }
    // for rle in ret {
    //       (*_vec).push(route.clone());
    // }
    return ret;
}
