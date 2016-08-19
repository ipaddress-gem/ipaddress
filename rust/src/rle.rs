use std::collections::HashMap;

#[derive(Debug, Copy, Clone)]
pub struct Rle {
    part: u16,
    pos: u16,
    cnt: u16,
    max: bool
}


#[allow(dead_code)]
pub fn rle_code(parts: &Vec<u16>) -> Vec<Rle> {
    let mut ret : Vec<Rle> = Vec::new();
    let mut last : Option<Rle> = None;
    let mut max_poses: HashMap<u16, u16> = HashMap::new();
    for i in 0..parts.len() {
        let part = parts.get(i).unwrap();
            if last.is_some() && last.unwrap().part == *part {
                last.unwrap().cnt += 1;
            } else {
                if last.is_some() {
                    let mut _last = last.unwrap();
                    let max_pos = max_poses.entry(_last.part).or_insert(_last.pos);
                    if _last.pos != *max_pos {
                        let ref mut prev = ret[*max_pos as usize];
                        if prev.cnt >= _last.cnt {
                            _last.max = false;
                        } else {
                            prev.max = false;
                        }
                    }
                    ret.push(_last);
                }
                last = Some(Rle { part: *part, pos: i as u16, cnt: 1, max: true });
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
