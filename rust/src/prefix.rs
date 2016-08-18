//use std::ops::{Add, Sub};
use num::bigint::BigUint;

use num_traits::identities::Zero;
use num_traits::identities::One;
use num_integer::Integer;
use num_traits::cast::ToPrimitive;
use num_traits::cast::FromPrimitive;


pub struct Prefix {
    pub num: usize,
    pub ip_bits: ::ip_bits::IpBits,
    pub in_mask: BigUint,
    pub vt_from: &'static (Fn(&Prefix, usize) -> Result<Prefix, String>),
    //pub vt_to_ip_str: &'static (Fn(&Vec<u16>) -> String)
}

impl Prefix {
    #[allow(dead_code)]
    pub fn from(&self, num: usize) -> Result<Prefix, String>{
        return (self.vt_from)(self, num)
    }

    #[allow(dead_code)]
    pub fn to_ip_str(&self) -> String {
//        return (self.vt_to_ip_str)(&self.ip_bits.parts(&self.net_mask()))
    }

    pub fn size(&self) -> BigUint {
      return BigUint::one() << (self.ip_bits.bits-self.num.to_usize().unwrap())
    }


    #[allow(dead_code)]
    #[allow(unused_variables)]
    pub fn in_mask(num: usize) -> BigUint {
        let mut mask = BigUint::zero();
        for i in 1..num {
            mask = mask + BigUint::one();
            mask = mask << 1;
        }
        return mask
    }

    #[allow(dead_code)]
    fn get_prefix(&self) -> usize {
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
        return self.in_mask.clone() >> self.get_prefix().to_usize().unwrap()
    }

    #[allow(dead_code)]
    pub fn host_mask_str(&self) -> String {
//        return (self.vt_to_ip_str)(&self.ip_bits.parts(&self.host_mask()))
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
    pub fn host_prefix(&self) -> usize {
        return (self.ip_bits.bits) - self.num;
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
    pub fn to_i(&self) -> usize {
        return self.get_prefix();
    }

    #[allow(dead_code)]
    pub fn add_prefix(&self, other: &Prefix) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other.get_prefix())
    }
    #[allow(dead_code)]
    pub fn add_u8(&self, other: usize) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other)
    }
    #[allow(dead_code)]
    pub fn sub_prefix(&self, other: &Prefix) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other.get_prefix())
    }
    #[allow(dead_code)]
    pub fn sub_u8(&self, other: usize) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other)
    }

}
