use num::bigint::BigUint;


use num_traits::identities::One;
use num_traits::identities::Zero;
// use num_integer::Integer;
use num_traits::cast::ToPrimitive;
use num_traits::FromPrimitive;
use core::convert::From;
use core::ops::Rem;
use core::ops::Shr;
// use core::marker::Copy;
use core::clone::Clone;
use std::fmt;
use rle;

#[allow(dead_code)]
#[derive(PartialOrd, PartialEq, Eq, Debug, Clone, Copy)]
pub enum IpVersion {
    V4,
    V6,
}

// #[derive(Debug, Clone)]
pub struct IpBits {
    pub version: IpVersion,
    pub vt_as_compressed_string: fn(&IpBits, &BigUint) -> String,
    pub vt_as_uncompressed_string: fn(&IpBits, &BigUint) -> String,
    pub bits: usize,
    pub part_bits: usize,
    pub dns_bits: usize,
    pub rev_domain: &'static str,
    pub part_mod: BigUint,
    pub host_ofs: BigUint, // ipv4=1, ipv6=0
}

impl Clone for IpBits {
    fn clone(&self) -> IpBits {
        IpBits {
            version: self.version,
            vt_as_compressed_string: self.vt_as_compressed_string,
            vt_as_uncompressed_string: self.vt_as_uncompressed_string,
            bits: self.bits,
            part_bits: self.part_bits,
            dns_bits: self.dns_bits,
            rev_domain: self.rev_domain,
            part_mod: self.part_mod.clone(),
            host_ofs: self.host_ofs.clone(),
        }
    }
}

impl fmt::Debug for IpBits {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "IpBits: {:?}", self.version)
    }
}


impl IpBits {
    #[allow(unused_variables)]
    pub fn parts(&self, bu: &BigUint) -> Vec<u16> {
        let mut vec: Vec<u16> = Vec::new();
        let mut my = bu.clone();
        let part_mod = BigUint::one() << self.part_bits;// - BigUint::one();
        for i in 0..(self.bits / self.part_bits) {
            vec.push(my.clone().rem(&part_mod).to_u16().unwrap());
            my = my.shr(self.part_bits);
        }
        vec.reverse();
        return vec;
    }

    pub fn as_compressed_string(&self, bu: &BigUint) -> String {
        return (self.vt_as_compressed_string)(self, bu);
    }
    #[allow(dead_code)]
    pub fn as_uncompressed_string(&self, bu: &BigUint) -> String {
        return (self.vt_as_uncompressed_string)(self, bu);
    }

    //  Returns the IP address in in-addr.arpa format
    //  for DNS lookups
    //
    //    ip = IPAddress("172.16.100.50/24")
    //
    //    ip.reverse
    //      // => "50.100.16.172.in-addr.arpa"
    //
    // #[allow(dead_code)]
    // pub fn dns_reverse(&self, bu: &BigUint) -> String {
    //     let mut ret = String::new();
    //     let part_mod = BigUint::one() << 4;
    //     let the_dot = String::from(".");
    //     let mut dot = &String::from("");
    //     let mut addr = bu.clone();
    //     for _ in 0..(self.bits / self.dns_bits) {
    //         ret.push_str(dot);
    //         let lower = addr.mod_floor(&part_mod).to_usize().unwrap();
    //         ret.push_str(self.dns_part_format(lower).as_str());
    //         addr = addr >> self.dns_bits;
    //         dot = &the_dot;
    //     }
    //     ret.push_str(self.rev_domain);
    //     return ret;
    // }

    pub fn dns_part_format(&self, i: u8) -> String {
        match self.version {
            IpVersion::V4 => return format!("{}", i),
            IpVersion::V6 => return format!("{:01x}", i),
        }
    }
}


fn ipv4_as_compressed(ip_bits: &IpBits, host_address: &BigUint) -> String {
    let mut ret = String::new();
    let mut sep = "";
    for part in ip_bits.parts(host_address) {
        ret.push_str(sep);
        ret.push_str(&format!("{}", part));
        sep = ".";
    }
    return ret;
}
fn ipv6_as_compressed(ip_bits: &IpBits, host_address: &BigUint) -> String {
    //println!("ipv6_as_compressed:{}", host_address);
    let mut ret = String::new();
    let the_colon = String::from(":");
    let the_empty = String::from("");
    let mut colon = &the_empty;
    let mut done = false;
    for rle in rle::code(&ip_bits.parts(host_address)) {
        // println!(">>{:?}", rle);
        for _ in 0..rle.cnt {
            if done || !(rle.part == 0 && rle.max) {
                ret.push_str(&format!("{}{:x}", colon, rle.part));
                colon = &the_colon;
            } else if rle.part == 0 && rle.max {
                ret.push_str("::");
                colon = &the_empty;
                done = true;
                break;
            }
        }
    }
    return ret;
}
fn ipv6_as_uncompressed(ip_bits: &IpBits, host_address: &BigUint) -> String {
    let mut ret = String::new();
    let mut sep = "";
    for part in ip_bits.parts(host_address) {
        ret.push_str(sep);
        ret.push_str(&format!("{:04x}", part));
        sep = ":";
    }
    return ret;
}



pub fn v4() -> IpBits {
    IpBits {
        version: IpVersion::V4,
        vt_as_compressed_string: ipv4_as_compressed,
        vt_as_uncompressed_string: ipv4_as_compressed,
        bits: 32,
        part_bits: 8,
        dns_bits: 8,
        rev_domain: "in-addr.arpa",
        part_mod: BigUint::from_usize(1 << 8).unwrap(),
        host_ofs: BigUint::one(),
    }
}

pub fn v6() -> IpBits {
    return IpBits {
        version: IpVersion::V6,
        vt_as_compressed_string: ipv6_as_compressed,
        vt_as_uncompressed_string: ipv6_as_uncompressed,
        bits: 128,
        part_bits: 16,
        dns_bits: 4,
        rev_domain: "ip6.arpa",
        part_mod: BigUint::from_usize(1 << 16).unwrap(),
        host_ofs: BigUint::zero(),
    };
}
