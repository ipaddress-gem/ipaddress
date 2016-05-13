//use std::ops::{Add, Sub};
use num::bigint::BigInt;

pub struct Prefix {
    pub num: u8,
    pub vt_from: &'static (Fn(&Prefix, u8) -> Result<Prefix, String>),
    pub vt_host_prefix: &'static (Fn(&Prefix) -> u8),
    pub vt_bits: &'static (Fn(&Prefix) -> String),
    pub vt_to_ip_str: &'static (Fn(&Prefix) -> String),
    pub vt_to_number: &'static (Fn(&Prefix) -> BigInt),
}

impl Prefix {
    #[warn(dead_code)]
    fn get_prefix(&self) -> u8 {
        return self.num
    }
    #[warn(dead_code)]
    pub fn from(&self, num: u8) -> Result<Prefix, String>{
        return (self.vt_from)(self, num)
    }
    #[warn(dead_code)]
    pub fn host_prefix(&self) -> u8 {
        return (self.vt_host_prefix)(self)
    }
    #[warn(dead_code)]
    pub fn bits(&self) -> String {
        return (self.vt_bits)(self)
    }
    #[warn(dead_code)]
    pub fn to_ip_str(&self) -> String {
        return (self.vt_to_ip_str)(self)
    }
    #[warn(dead_code)]
    pub fn to_number(&self) -> BigInt {
        return (self.vt_to_number)(self)
    }
    #[warn(dead_code)]
    pub fn to_s(&self) -> String {
        return format!("#{}", self.get_prefix());
    }
    #[warn(dead_code)]
    pub fn inspect(&self) -> String {
        return self.to_s();
    }
    #[warn(dead_code)]
    pub fn to_i(&self) -> u8 {
        return self.get_prefix();
    }
    #[warn(dead_code)]
    pub fn add_prefix(&self, other: Prefix) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other.get_prefix())
    }
    #[warn(dead_code)]
    pub fn add_u8(&self, other: u8) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other)
    }
    #[warn(dead_code)]
    pub fn sub_prefix(&self, other: Prefix) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other.get_prefix())
    }
    #[warn(dead_code)]
    pub fn sub_u8(&self, other: u8) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other)
    }
}
