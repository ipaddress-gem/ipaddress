use num::bigint::BigUint;


use num_traits::identities::One;
use num_traits::identities::Zero;
use num_integer::Integer;
use num_traits::cast::ToPrimitive;

#[derive(PartialEq, Eq)]
pub enum IpVersion { V4, V6 }

pub struct IpBits {
    pub version: IpVersion,
    pub vt_as_compressed_string: &'static (Fn(&IpBits, &BigUint) -> String),
    pub bits: usize,
    pub part_bits: usize,
    pub dns_bits: usize,
    pub rev_domain: &'static str,
    pub part_mod: BigUint,
    pub host_ofs: BigUint // ipv4=1, ipv6=0
}

impl IpBits {
    #[allow(unused_variables)]
    pub fn parts(&self, bu: &BigUint) -> Vec<u16> {
        let mut vec: Vec<u16> = Vec::new();
        let mut my = bu.clone();
        let part_mod = BigUint::one() << self.part_bits;
        for i in 0..(self.bits / self.part_bits) {
            vec.push(my.mod_floor(&part_mod).to_u16().unwrap());
            my = my >> (self.part_bits as usize);
        }
        vec.reverse();
        return vec;
    }

    pub fn as_compressed_string(&self, bu: &BigUint) -> String {
        return (self.vt_as_compressed_string)(self, bu);
    }

    //  Returns the IP address in in-addr.arpa format
    //  for DNS lookups
    //
    //    ip = IPAddress("172.16.100.50/24")
    //
    //    ip.reverse
    //      // => "50.100.16.172.in-addr.arpa"
    //
    pub fn dns_reverse(&self, bu: &BigUint) -> String {
        let mut ret = String::new();
        let part_mod = BigUint::one() << 4;
        let the_dot = String::from(".");
        let mut dot = &String::from("");
        let mut addr = bu.clone();
        for i in 0..(self.bits/self.dns_bits) {
            ret.push_str(dot);
            let lower = addr.mod_floor(&part_mod).to_usize().unwrap();
            ret.push_str(self.dns_part_format(lower).as_str());
            addr = addr >> self.dns_bits;
            dot = &the_dot;
        }
        ret.push_str(self.rev_domain);
        return ret;
    }

    pub fn dns_part_format(&self, i: usize) -> String {
        match self.version {
            IpVersion::V4 => {
                return format!("{}", i)
            },
            IpVersion::V6 => {
                return format!("{:04x}", i)
            },
        }
    }
}


fn ipv4_as_compressed(ip_bits: &IpBits, host_address: &BigUint) -> String {
    let ret = String::new();
    let mut sep = "";
    for part in ip_bits.parts(host_address) {
        ret.push_str(sep);
        ret.push_str(&format!("{}", part));
        sep = ".";
    }
    return ret;
}
fn ipv6_as_compressed(ip_bits: &IpBits, host_address: &BigUint) -> String {
    let ret = String::new();
    let mut sep = "";
    for part in ip_bits.parts(host_address) {
        ret.push_str(sep);
        ret.push_str(&format!("{:x}", part));
        sep = ":";
    }
    return ret;
}



pub const IP_BITS_V4: IpBits = IpBits {
    version: IpVersion::V4,
    vt_as_compressed_string: &ipv4_as_compressed,
    bits: 32,
    part_bits: 8,
    dns_bits: 8,
    rev_domain: ".in-addr.arpa",
    part_mod: BigUint::one() << 8,
    host_ofs: BigUint::one()
};

pub const IP_BITS_V6 : IpBits = IpBits {
     version: IpVersion::V6,
     vt_as_compressed_string: &ipv6_as_compressed,
     bits: 128,
     part_bits: 16,
     dns_bits: 8,
     rev_domain: ".ip6.arpa",
     part_mod: BigUint::one() << 16,
     host_ofs: BigUint::zero()
 };
