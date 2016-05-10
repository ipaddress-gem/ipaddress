use std::ops::{Add, Sub};

#[derive(Ord,PartialOrd,Eq,PartialEq,Debug,Copy,Clone)]
struct Prefix  {
    pub prefix: u8
}

impl Prefix {
    pub fn new(num: u8) {
        return Prefix { prefix: num };
    }
    pub fn to_s(&self) {
        return format!("#{}", self.prefix);
    }
    pub fn inspect(&self) {
        return self.to_s();
    }
    pub fn to_i(&self) {
        return self.prefix
    }

    // #
    // # Compare the prefix
    // #
    // def <=>(oth)
    //   @prefix <=> oth.to_i
    // end

}

impl Add for Prefix {
    type Output = Prefix;
    fn add(self, other: Prefix) -> Prefix {
        Prefix { prefix: self.prefix + other.prefix }
    }
}
impl Add<u8> for Prefix {
    type Output = Prefix;
    fn add(self, other: u8) -> Prefix {
        Prefix { prefix: self.prefix + other }
    }
}

impl Sub for Prefix {
    type Output = Prefix;
    fn sub(self, other: Prefix) -> Prefix {
        Prefix { prefix: self.prefix - other.prefix }
    }
}
impl Sub<u8> for Prefix {
    type Output = Prefix;
    fn sub(self, other: u8) -> Prefix {
        Prefix { prefix: self.prefix - other }
    }
}
