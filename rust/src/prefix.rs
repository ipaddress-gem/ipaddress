//use std::ops::{Add, Sub};
use num::bigint::BigUint;

use num_traits::identities::Zero;
use num_traits::identities::One;
use num_integer::Integer;
use num_traits::cast::ToPrimitive;
use num_traits::cast::FromPrimitive;

pub struct Prefix {
    pub num: u8,
    pub ip_bits: u8,
    pub ip_part_bits: u8,
    pub in_mask: BigUint,
    pub vt_from: &'static (Fn(&Prefix, u8) -> Result<Prefix, String>),
    pub vt_to_ip_str: &'static (Fn(&Vec<u16>) -> String)
}

impl Prefix {
    #[allow(dead_code)]
    pub fn from(&self, num: u8) -> Result<Prefix, String>{
        return (self.vt_from)(self, num)
    }

    #[allow(dead_code)]
    pub fn to_ip_str(&self) -> String {
        return (self.vt_to_ip_str)(&self.ip_parts(&self.net_mask()))
    }

    #[allow(unused_variables)]
    pub fn ip_parts(&self, bu: &BigUint) -> Vec<u16> {
        let mut vec: Vec<u16> = Vec::new();
        let mut my = bu.clone();
        let part_mod = BigUint::from_u32(1 << self.ip_part_bits).unwrap();
        for i in 0..(self.ip_bits/self.ip_part_bits) {
            vec.push(my.mod_floor(&part_mod).to_u16().unwrap());
            my = my >> (self.ip_part_bits as usize);
        }
        vec.reverse();
        return vec
    }

    #[allow(dead_code)]
    #[allow(unused_variables)]
    pub fn in_mask(num: u8) -> BigUint {
        let mut mask = BigUint::zero();
        for i in 1..num {
            mask = mask + BigUint::one();
            mask = mask << 1;
        }
        return mask
    }

    #[allow(dead_code)]
    fn get_prefix(&self) -> u8 {
        return self.num
    }

    //  The hostmask is the contrary of the subnet mask,
    //  as it shows the bits that can change within the
    //  hosts
    //
    //    prefix = IPAddress::Prefix32.new 24
    //
    //    prefix.hostmask
    //      // => "0.0.0.255"
    //
    #[allow(dead_code)]
    pub fn host_mask(&self) -> BigUint {
        return self.in_mask.clone() >> (self.get_prefix() as usize)
    }

    #[allow(dead_code)]
    pub fn host_mask_str(&self) -> String {
        return (self.vt_to_ip_str)(&self.ip_parts(&self.host_mask()))
    }



    //
    //  Returns the length of the host portion
    //  of a netmask.
    //
    //    prefix = Prefix128.new 96
    //
    //    prefix.host_prefix
    //      // => 128
    //
    #[allow(dead_code)]
    pub fn host_prefix(&self) -> u8 {
        return self.ip_bits - self.num;
    }

    //
    //  Transforms the prefix into a string of bits
    //  representing the netmask
    //
    //    prefix = IPAddress::Prefix128.new 64
    //
    //    prefix.bits
    //      // => "1111111111111111111111111111111111111111111111111111111111111111"
    //          "0000000000000000000000000000000000000000000000000000000000000000"
    //
    #[allow(dead_code)]
    pub fn bits(&self) -> String {
        return self.net_mask().to_str_radix(2)
    }
    #[allow(dead_code)]
    pub fn net_mask(&self) -> BigUint {
        return (self.in_mask.clone() >> (self.host_prefix() as usize)) << (self.host_prefix() as usize);
    }

    #[allow(dead_code)]
    pub fn to_s(&self) -> String {
        return format!("#{}", self.get_prefix());
    }
    #[allow(dead_code)]
    pub fn inspect(&self) -> String {
        return self.to_s();
    }
    #[allow(dead_code)]
    pub fn to_i(&self) -> u8 {
        return self.get_prefix();
    }

    #[allow(dead_code)]
    pub fn add_prefix(&self, other: &Prefix) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other.get_prefix())
    }
    #[allow(dead_code)]
    pub fn add_u8(&self, other: u8) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other)
    }
    #[allow(dead_code)]
    pub fn sub_prefix(&self, other: &Prefix) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other.get_prefix())
    }
    #[allow(dead_code)]
    pub fn sub_u8(&self, other: u8) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other)
    }

}
