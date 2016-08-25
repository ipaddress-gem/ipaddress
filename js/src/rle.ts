
interface RleType {

}

class Rle {
    pub part: RleType;
    pub pos: number;
    pub cnt: number;
    pub max: boolean;

impl<T: Display + LowerHex> fmt::Debug for Rle<T> {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "<Rle@part:{:x},pos:{},cnt:{},max:{}>",
            self.part, self.pos, self.cnt, self.max)
    }
}


impl<T: PartialEq> PartialEq<Rle<T>> for Rle<T> {
    fn eq(&self, other: &Rle<T>) -> bool {
        return self.part.eq(&other.part) && self.pos == other.pos &&
                self.cnt == other.cnt && self.max == other.max;
    }
    fn ne(&self, other: &Self) -> bool {
        !self.eq(other)
    }
}

//impl<T: PartialEq> Eq for Rle<T> {}
struct Last<T: Eq + Hash + Display + Copy + Clone> {
    pub val: Option<Rle<T>>,
    pub max_poses: HashMap<T, Vec<usize>>,
    pub ret: Vec<Rle<T>>
}

impl<T: Eq + Hash + Display + Copy + Clone + LowerHex> Last<T> {
    pub fn handle_last(&mut self) {
    if self.val.is_none() {
        return;
    }
    let ref mut _last = self.val.as_mut().unwrap();
    let mut max_rles = self.max_poses.entry(_last.part.clone()).or_insert(Vec::new());
    for idx in max_rles.clone() {
        let ref mut prev = self.ret[idx];
        if prev.cnt > _last.cnt {
            // println!(">>>>> last={:?}->{}->prev={:?}", _last, idx, prev);
            _last.max = false;
        } if prev.cnt == _last.cnt {
            // nothing
        } else if prev.cnt < _last.cnt {
            // println!("<<<<< last={:?}->{}->prev={:?}", _last, idx, prev);
            //self.ret[idx].max = false;
            prev.max = false;
        }
    }
    //println!("push:{}:{:?}", self.ret.len(), _last);
    max_rles.push(self.ret.len());
    _last.pos = self.ret.len();
    self.ret.push(_last.clone());
    }
}

#[allow(dead_code)]
pub fn code<T: Eq + Hash + Display + Copy + Clone + LowerHex>(parts: &Vec<T>) -> Vec<Rle<T>> {
    let mut last = Last {
        val: None,
        max_poses: HashMap::new(),
        ret: Vec::new()
    };
    // println!("code");
    for i in 0..parts.len() {
        let ref part = parts[i];
        // println!("part:{}", part);
        if last.val.is_some() && last.val.unwrap().part == *part {
            last.val.as_mut().unwrap().cnt += 1;
        } else {
            last.handle_last();
            last.val = Some(Rle::<T>{ part: part.clone(), pos: 0, cnt: 1, max: true });
        }
    }
    last.handle_last();
    return last.ret;
}
