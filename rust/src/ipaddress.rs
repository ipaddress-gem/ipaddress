
use core::ops::Add;
use core::ops::Shl;
use core::ops::Shr;
use core::ops::Sub;
use core::ops::Rem;
// use core::ops::Rem;
// use core::ops::Range;
//use core::iter::Step;
use core::iter::Iterator;
use core::cmp::Ordering;
use core::cmp::Ord;
use core::convert::From;
// use core::fmt::Debug;

use num::bigint::BigUint;
use num::bigint::ToBigUint;
// use num_integer::Integer;

use ip_bits::IpBits;
use prefix::Prefix;
use regex::Regex;
use std::f64;
use std::fmt;

use num_traits::identities::Zero;
use num_traits::identities::One;
use num_traits::FromPrimitive;

use num_traits::cast::ToPrimitive;

use ip_bits::IpVersion;

// impl Step for BigUint {
//     fn step(&self, by: &Self) -> Option<Self> {
//         return self.add(by);
//     }
//     fn steps_between(start: &Self, end: &Self, by: &Self) -> Option<BigUint> {
//         return None;
//     }
// }


pub struct IPAddress {

    pub ip_bits: IpBits,
    pub host_address: BigUint,
    pub prefix: Prefix,
    pub mapped: Option<Box<IPAddress>>,
    pub vt_is_private: fn(&IPAddress) -> bool,
    pub vt_is_loopback: fn(&IPAddress) -> bool,
    pub vt_to_ipv6: fn(&IPAddress) -> IPAddress
}

impl fmt::Debug for IPAddress {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "IPAddress: {}", self.to_string())
    }
}


impl Clone for IPAddress {
    fn clone(&self) -> IPAddress {
        IPAddress {
            ip_bits: self.ip_bits.clone(),
            host_address: self.host_address.clone(),
            prefix: self.prefix.clone(),
            mapped: self.mapped.clone(),
            vt_is_private: self.vt_is_private,
            vt_is_loopback: self.vt_is_loopback,
            vt_to_ipv6: self.vt_to_ipv6
        }
    }
}


impl Ord for IPAddress {
    fn cmp(&self, oth: & IPAddress) -> Ordering {
            if self.ip_bits.version != oth.ip_bits.version {
                if self.ip_bits.version == IpVersion::V6 {
                    return Ordering::Greater;
                }
                return Ordering::Less;
            }
            //let adr_diff = self.host_address - oth.host_address;
            if self.host_address < oth.host_address  {
                return Ordering::Less;
            } else if self.host_address > oth.host_address {
                return Ordering::Greater;
            }
            return Ordering::Equal;
    }
}

impl PartialOrd for IPAddress {
    fn partial_cmp(&self, other: &IPAddress) -> Option<Ordering> {
        Some(self.cmp(other))
    }

}

impl PartialEq for IPAddress {
    fn eq(&self, other: &Self) -> bool {
        return self.ip_bits.version == other.ip_bits.version &&
            self.prefix == other.prefix &&
            self.host_address == other.host_address &&
            self.mapped.eq(&other.mapped)
    }
    fn ne(&self, other: &Self) -> bool {
        !self.eq(other)
    }
}

impl Eq for IPAddress {}


impl IPAddress {
    // Parse the argument string to create a new
    // IPv4, IPv6 or Mapped IP object
    //
    //   ip  = IPAddress.parse "172.16.10.1/24"
    //   ip6 = IPAddress.parse "2001:db8::8:800:200c:417a/64"
    //   ip_mapped = IPAddress.parse "::ffff:172.16.10.1/128"
    //
    // All the object created will be instances of the
    // correct class:
    //
    //  ip.class
    //    //=> IPAddress::IPv4
    //  ip6.class
    //    //=> IPAddress::IPv6
    //  ip_mapped.class
    //    //=> IPAddress::IPv6::Mapped
    //
    pub fn parse<S: Into<String>>(_str: S) -> Result<IPAddress, String> {
        let str = _str.into();
        let re_mapped = Regex::new(r":.+\.").unwrap();
        let re_ipv4 = Regex::new(r"\.").unwrap();
        let re_ipv6 = Regex::new(r":").unwrap();
        if re_mapped.is_match(&str) {
            return ::ipv6_mapped::new(str);
        } else {
            if re_ipv4.is_match(&str) {
                return ::ipv4::new(str);
            } else if re_ipv6.is_match(&str) {
                return ::ipv6::new(str);
            }
        }
        return Err(format!("Unknown IP Address {}", str));
    }

    pub fn split_at_slash(str: &String) -> (String, Option<String>) {
        let slash : Vec<&str> = str.trim().split("/").collect();
        let mut addr = String::new();
        if slash.get(0).is_some() {
            addr.push_str(slash.get(0).unwrap().to_string().trim());
        }
        if slash.get(1).is_some() {
            return (addr, Some(String::from(slash.get(1).unwrap().to_string().trim())));
        } else {
            return (addr, None);
        }
    }
    #[allow(dead_code)]
    pub fn from(&self, addr: &BigUint, prefix: &Prefix) -> IPAddress {
        return IPAddress {
            ip_bits: self.ip_bits.clone(),
            host_address: addr.clone(),
            prefix: prefix.clone(),
            mapped: self.mapped.clone(),
            vt_is_private: self.vt_is_private,
            vt_is_loopback: self.vt_is_loopback,
            vt_to_ipv6: self.vt_to_ipv6
        };
    }

    // True if the object is an IPv4 address
    //
    //   ip = IPAddress("192.168.10.100/24")
    //
    //   ip.ipv4?
    //     //-> true
    //
    #[allow(dead_code)]
    pub fn is_ipv4(&self) -> bool {
        return self.ip_bits.version == IpVersion::V4
    }

    // True if the object is an IPv6 address
    //
    //   ip = IPAddress("192.168.10.100/24")
    //
    //   ip.ipv6?
    //     //-> false
    //
    #[allow(dead_code)]
    pub fn is_ipv6(&self) -> bool {
      return self.ip_bits.version == IpVersion::V6
    }

    // Checks if the given string is a valid IP address,
    // either IPv4 or IPv6
    //
    // Example:
    //
    //   IPAddress::valid? "2002::1"
    //     //=> true
    //
    //   IPAddress::valid? "10.0.0.256"
    //     //=> false
    //
    #[allow(dead_code)]
    pub fn is_valid<S: Into<String>>(_addr: S) -> bool {
        let addr = _addr.into();
        return IPAddress::is_valid_ipv4(addr.clone()) || IPAddress::is_valid_ipv6(addr);
    }



    // Checks if the given string is a valid IPv4 address
    //
    // Example:
    //
    //   IPAddress::valid_ipv4? "2002::1"
    //     //=> false
    //
    //   IPAddress::valid_ipv4? "172.16.10.1"
    //     //=> true
    //
    pub fn split_to_u32(addr: &String) -> Result<u32, String> {
        let mut ip : u32 = 0;
        let mut shift = 24;
        for i in addr.split(".") {
            let part = i.parse::<u32>();
            if part.is_err() {
                return Err(format!("IP must contain numbers {} ", addr));
            }
            let part_num = part.unwrap();
            if part_num >= 256 {
                return Err(format!("IP items has to lower than 256. {} ", addr));
            }
            ip = ip | (part_num << shift);
            shift -= 8;
        }
        return Ok(ip);
    }
    #[allow(dead_code)]
    pub fn is_valid_ipv4<S: Into<String>>(addr: S) -> bool {
        return IPAddress::split_to_u32(&addr.into()).is_ok();
    }


    // Checks if the given string is a valid IPv6 address
    //
    // Example:
    //
    //   IPAddress::valid_ipv6? "2002::1"
    //     //=> true
    //
    //   IPAddress::valid_ipv6? "2002::DEAD::BEEF"
    //     //=> false
    //
    pub fn split_on_colon(addr: &String) -> (Result<BigUint, String>, usize) {
        let parts = addr.trim().split(":").collect::<Vec<&str>>();
        let mut ip = BigUint::zero();
        if parts.get(0).unwrap().is_empty() {
            return (Ok(ip), 0);
        }
        let parts_len = parts.len();
        let mut shift : isize = ((parts_len - 1) * 16) as isize;
        for i in parts {
            //println!("{}={}", addr, i);
            let part = u64::from_str_radix(i, 16);
            if part.is_err() {
                return (Err(format!("IP must contain hex numbers {}->{}", addr, i)), 0);
            }
            let part_num = part.unwrap();
            if part_num >= 65536 {
                return (Err(format!("IP items has to lower than 65536. {}", addr)), 0);
            }
            ip = ip.add(part_num.to_biguint().unwrap().shl(shift as usize));
            shift -= 16;
        }
        return (Ok(ip), parts_len);
    }
    pub fn split_to_num(addr: &String) -> Result<BigUint, String> {
        //let mut ip = 0;
        let pre_post = addr.trim().split("::").collect::<Vec<&str>>();
        if pre_post.len() > 2 {
            return Err(format!("IPv6 only allow one :: {}", addr));
        }
        if !pre_post.is_empty() {
            //println!("{}=::={}", pre_post[0], pre_post[1]);
            let (pre, pre_parts) = IPAddress::split_on_colon(&String::from(*pre_post.get(0).unwrap()));
            if pre.is_err() {
                return pre;
            }
            let (post, post_parts) = IPAddress::split_on_colon(&String::from(*pre_post.get(1).unwrap()));
            if post.is_err() {
                return post;
            }
            return Ok((pre.unwrap() << ((128 - (pre_parts * 16) - (post_parts * 16)))) +
                      post.unwrap());
        }
        //println!("split_to_num:no double:{}", addr);
        let (ret, _) = IPAddress::split_on_colon(addr);
        return ret;
    }
    #[allow(dead_code)]
    pub fn is_valid_ipv6<S: Into<String>>(addr: S) -> bool {
        return IPAddress::split_to_num(&addr.into()).is_ok();
    }


    // private helper for summarize
    // assumes that networks is output from reduce_networks
    // means it should be sorted lowers first and uniq
    //
    #[allow(dead_code)]
    pub fn aggregate(networks: &Vec<IPAddress>) -> Vec<IPAddress> {
        let mut stack = networks.iter().map(|i| Box::new(i.network()) )
            .collect::<Vec<_>>();
        stack.sort_by(|a, b| a.cmp(b));
        let mut pos = 0;
        loop {
            let first = pos;
            if stack.len() >= pos {
                break;
            }
            pos = pos + 1;
            let second = pos;
            if stack.len() >= pos {
                break;
            }
            pos = pos + 1;
            //let mut firstUnwrap = first.unwrap();
            if stack[first].includes(&stack[second]) {
                if pos >= 2 {
                    pos = pos - 2;
                } else {
                    pos = 0;
                }
                stack.remove(pos + 1);
            } else {
                stack[first].prefix = stack[first].prefix.sub(1).unwrap();
                if stack[first].prefix.add(1).unwrap() != stack[second].prefix &&
                   stack[first].includes(&stack[second]) {
                    pos = pos - 2;
                    stack[pos] = stack[first].clone(); // kaputt
                    stack.remove(pos + 1);
                    pos = pos - 1; // backtrack
                } else {
                    stack[first].prefix = stack[first].prefix.from(1).unwrap(); //reset prefix
                    if pos > 0 {
                        pos = pos - 1; // do it with second as first
                    }
                }
            }
        }
        let mut ret = Vec::new();
        for i in 0..pos - 1 {
             ret.push(*stack[i].clone());
        }
        return ret;
    }

    pub fn parts(&self) -> Vec<u16> {
        return self.ip_bits.parts(&self.host_address);
    }

    pub fn parts_hex_str(&self) -> Vec<String> {
        let mut ret : Vec<String> = Vec::new();
        for i in self.parts() {
            ret.push(format!("{:04x}", i));
        }
        return ret;
    }


    pub fn dns_reverse(&self) -> String{
        let mut ret = String::new();
        let mut dot = "";
        for i in self.dns_parts() {
            ret.push_str(dot);
            ret.push_str(&self.ip_bits.dns_part_format(i));
            dot = ".";
        }
        ret.push_str(self.ip_bits.rev_domain);
        return ret;
    }


    pub fn dns_parts(&self) -> Vec<u8> {
        let mut ret : Vec<u8> = Vec::new();
        let mut num = self.host_address.clone();
        let mask = BigUint::one().shl(self.ip_bits.dns_bits);
        for _ in 0..self.ip_bits.bits/self.ip_bits.dns_bits {
            let part = num.clone().rem(&mask).to_u8().unwrap();
            num = num.shr(self.ip_bits.dns_bits);
            ret.push(part);
        }
        return ret;
    }

    pub fn dns_networks(&self) -> Vec<IPAddress> {
         let next_bit_mask = ((self.prefix.num+self.ip_bits.dns_bits-1)/self.ip_bits.dns_bits)
                    *self.ip_bits.dns_bits;
         if next_bit_mask <= 0 {
             return vec![self.network()];
         }
         let step_bit_net = BigUint::one().shl(self.ip_bits.bits-next_bit_mask);
         if step_bit_net == BigUint::zero() {
             return vec![self.network()];
         }
         let mut ret: Vec<IPAddress> = Vec::new();
         let mut step = self.network().host_address;
         let prefix = self.prefix.from(next_bit_mask).unwrap();
         while step <= self.broadcast().host_address {
           ret.push(self.from(&step, &prefix));
           step = step.add(&step_bit_net);
         }
         return ret;
      }


    // Summarization (or aggregation) is the process when two or more
    // networks are taken together to check if a supernet, including all
    // and only these networks, exists. If it exists then this supernet
    // is called the summarized (or aggregated) network.
    //
    // It is very important to understand that summarization can only
    // occur if there are no holes in the aggregated network, or, in other
    // words, if the given networks fill completely the address space
    // of the supernet. So the two rules are:
    //
    // 1) The aggregate network must contain +all+ the IP addresses of the
    //    original networks;
    // 2) The aggregate network must contain +only+ the IP addresses of the
    //    original networks;
    //
    // A few examples will help clarify the above. Let's consider for
    // instance the following two networks:
    //
    //   ip1 = IPAddress("172.16.10.0/24")
    //   ip2 = IPAddress("172.16.11.0/24")
    //
    // These two networks can be expressed using only one IP address
    // network if we change the prefix. Let Ruby do the work:
    //
    //   IPAddress::IPv4::summarize(ip1,ip2).to_s
    //     //=> "172.16.10.0/23"
    //
    // We note how the network "172.16.10.0/23" includes all the addresses
    // specified in the above networks, and (more important) includes
    // ONLY those addresses.
    //
    // If we summarized +ip1+ and +ip2+ with the following network:
    //
    //   "172.16.0.0/16"
    //
    // we would have satisfied rule //1 above, but not rule //2. So "172.16.0.0/16"
    // is not an aggregate network for +ip1+ and +ip2+.
    //
    // If it's not possible to compute a single aggregated network for all the
    // original networks, the method returns an array with all the aggregate
    // networks found. For example, the following four networks can be
    // aggregated in a single /22:
    //
    //   ip1 = IPAddress("10.0.0.1/24")
    //   ip2 = IPAddress("10.0.1.1/24")
    //   ip3 = IPAddress("10.0.2.1/24")
    //   ip4 = IPAddress("10.0.3.1/24")
    //
    //   IPAddress::IPv4::summarize(ip1,ip2,ip3,ip4).to_string
    //     //=> "10.0.0.0/22",
    //
    // But the following networks can't be summarized in a single network:
    //
    //   ip1 = IPAddress("10.0.1.1/24")
    //   ip2 = IPAddress("10.0.2.1/24")
    //   ip3 = IPAddress("10.0.3.1/24")
    //   ip4 = IPAddress("10.0.4.1/24")
    //
    //   IPAddress::IPv4::summarize(ip1,ip2,ip3,ip4).map{|i| i.to_string}
    //     //=> ["10.0.1.0/24","10.0.2.0/23","10.0.4.0/24"]
    //
    //
    //  Summarization (or aggregation) is the process when two or more
    //  networks are taken together to check if a supernet, including all
    //  and only these networks, exists. If it exists then this supernet
    //  is called the summarized (or aggregated) network.
    //
    //  It is very important to understand that summarization can only
    //  occur if there are no holes in the aggregated network, or, in other
    //  words, if the given networks fill completely the address space
    //  of the supernet. So the two rules are:
    //
    //  1) The aggregate network must contain +all+ the IP addresses of the
    //     original networks;
    //  2) The aggregate network must contain +only+ the IP addresses of the
    //     original networks;
    //
    //  A few examples will help clarify the above. Let's consider for
    //  instance the following two networks:
    //
    //    ip1 = IPAddress("2000:0::4/32")
    //    ip2 = IPAddress("2000:1::6/32")
    //
    //  These two networks can be expressed using only one IP address
    //  network if we change the prefix. Let Ruby do the work:
    //
    //    IPAddress::IPv6::summarize(ip1,ip2).to_s
    //      // => "2000:0::/31"
    //
    //  We note how the network "2000:0::/31" includes all the addresses
    //  specified in the above networks, and (more important) includes
    //  ONLY those addresses.
    //
    //  If we summarized +ip1+ and +ip2+ with the following network:
    //
    //    "2000::/16"
    //
    //  we would have satisfied rule // 1 above, but not rule // 2. So "2000::/16"
    //  is not an aggregate network for +ip1+ and +ip2+.
    //
    //  If it's not possible to compute a single aggregated network for all the
    //  original networks, the method returns an array with all the aggregate
    //  networks found. For example, the following four networks can be
    //  aggregated in a single /22:
    //
    //    ip1 = IPAddress("2000:0::/32")
    //    ip2 = IPAddress("2000:1::/32")
    //    ip3 = IPAddress("2000:2::/32")
    //    ip4 = IPAddress("2000:3::/32")
    //
    //    IPAddress::IPv6::summarize(ip1,ip2,ip3,ip4).to_string
    //      // => ""2000:3::/30",
    //
    //  But the following networks can't be summarized in a single network:
    //
    //    ip1 = IPAddress("2000:1::/32")
    //    ip2 = IPAddress("2000:2::/32")
    //    ip3 = IPAddress("2000:3::/32")
    //    ip4 = IPAddress("2000:4::/32")
    //
    //    IPAddress::IPv4::summarize(ip1,ip2,ip3,ip4).map{|i| i.to_string}
    //      // => ["2000:1::/32","2000:2::/31","2000:4::/32"]
    //
    #[allow(dead_code)]
    pub fn summarize(networks: &Vec<IPAddress>) -> Vec<IPAddress> {
        return IPAddress::aggregate(networks);
    }
    pub fn summarize_str<S: Into<String>>(netstr: Vec<S>) -> Result<Vec<IPAddress>, String> {
        let vec = IPAddress::to_ipaddress_vec(netstr);
        if vec.is_err() {
            return vec;
        }
        return Ok(IPAddress::aggregate(&vec.unwrap()));
    }

    #[allow(dead_code)]
    pub fn ip_same_kind(&self, oth: &IPAddress) -> bool {
        return self.ip_bits.version == oth.ip_bits.version
    }

    //  Returns true if the address is an unspecified address
    //
    //  See IPAddress::IPv6::Unspecified for more information
    //
    #[allow(dead_code)]
    pub fn is_unspecified(&self) -> bool {
        return self.host_address == BigUint::zero();
    }

    //  Returns true if the address is a loopback address
    //
    //  See IPAddress::IPv6::Loopback for more information
    //
    #[allow(dead_code)]
    pub fn is_loopback(&self) -> bool {
        return (self.vt_is_loopback)(self);
    }


    //  Returns true if the address is a mapped address
    //
    //  See IPAddress::IPv6::Mapped for more information
    //
    #[allow(dead_code)]
    pub fn is_mapped(&self) -> bool {
        return self.mapped.is_some() &&
            (self.host_address.clone() >> 32) == ((BigUint::one() << 1) - BigUint::one());
    }

    // //
    // //  Returns the address portion of the IPv4 object
    // //  as a string.
    // //
    // //    ip = IPAddress("172.16.100.4/22")
    // //
    // //    ip.address
    // //      // => "172.16.100.4"
    // //
    // pub fn address(&self) -> String {
    //   return (self.to_ip_str)(&self.host_address)
    // }

    //  Returns a string with the address portion of
    //  the IPv4 object
    //
    //    ip = IPAddress("172.16.100.4/22")
    //
    //    ip.to_s
    //      // => "172.16.100.4"
    //
    // pub fn to_s(&self) -> String {
    //   return self.address()
    // }
    // pub fn compressed(&self) -> String {
    //   return (self.to_ip_str_compressed)(&self.host_address)
    // }

    //  Returns the prefix portion of the IPv4 object
    //  as a IPAddress::Prefix32 object
    //
    //    ip = IPAddress("172.16.100.4/22")
    //
    //    ip.prefix
    //      // => 22
    //
    //    ip.prefix.class
    //      // => IPAddress::Prefix32
    //
    #[allow(dead_code)]
    pub fn prefix(&self) -> &Prefix {
        return &self.prefix;
    }



    // Checks if the argument is a valid IPv4 netmask
    // expressed in dotted decimal format.
    //
    //   IPAddress.valid_ipv4_netmask? "255.255.0.0"
    //     //=> true
    //
    #[allow(dead_code)]
    pub fn is_valid_netmask<S: Into<String>>(addr: S) -> bool {
        return IPAddress::parse_netmask_to_bits(addr.into()).is_ok();
    }

    pub fn netmask_to_bits(nm: &BigUint, bits: usize) -> Result<usize, String> {
        let mut prefix = 0;
        let mut addr = nm.clone();
        let mut in_host_part = true;
        let two = BigUint::from_u32(2).unwrap();
        for _ in 0..bits {
            let bit = addr.clone().rem(&two).to_usize().unwrap();
            if in_host_part && bit == 0 {
                prefix = prefix + 1;
            } else if in_host_part && bit == 1 {
                in_host_part = false;
            } else if !in_host_part && bit == 0 {
                return Err(format!("this is not a net mask {}", &nm));
            }
            addr = addr.shr(1);
        }
        return Ok(prefix);
    }


    pub fn parse_netmask_to_bits<S: Into<String>>(_netmask: S) -> Result<usize, String> {
        let my_str = _netmask.into();
        let is_number = my_str.parse();
        if is_number.is_ok() {
            return Ok(is_number.unwrap());
        }
        let my = IPAddress::parse(my_str.clone());
        if my.is_err() {
            return Err(format!("illegal netmask {}", &my.unwrap_err()));
        }
        let my_ip = my.unwrap();
        return IPAddress::netmask_to_bits(&my_ip.host_address, my_ip.ip_bits.bits);
    }


        //  Set a new prefix number for the object
        //
        //  This is useful if you want to change the prefix
        //  to an object created with IPv4::parse_u32 or
        //  if the object was created using the classful
        //  mask.
        //
        //    ip = IPAddress("172.16.100.4")
        //
        //    puts ip
        //      // => 172.16.100.4/16
        //
        //    ip.prefix = 22
        //
        //    puts ip
        //      // => 172.16.100.4/22
        //
        pub fn change_prefix(&self, num: usize) -> Result<IPAddress, String> {
            let prefix =  self.prefix.from(num);
            if prefix.is_err() {
                return Err(prefix.unwrap_err());
            }
            return Ok(self.from(&self.host_address, &prefix.unwrap()));
        }

    pub fn change_netmask<S: Into<String>>(&self, str: S) -> Result<IPAddress, String> {
        let my_str = str.into();
        let nm = IPAddress::parse_netmask_to_bits(my_str);
        if nm.is_err() {
            return Err(nm.unwrap_err());
        }
        return self.change_prefix(nm.unwrap());
    }

    // //
    // //  Returns the address as an array of decimal values
    // //
    // //    ip = IPAddress("172.16.100.4")
    // //
    // //    ip.octets
    // //      // => [172, 16, 100, 4]
    // //
    // pub fn octets(&self) {
    //   self.octets
    // }


    //  Returns a string with the IP address in canonical
    //  form.
    //
    //    ip = IPAddress("172.16.100.4/22")
    //
    //    ip.to_string
    //      // => "172.16.100.4/22"
    //
    #[allow(dead_code)]
    pub fn to_string(&self) -> String {
        let mut ret = String::new();
        ret.push_str(&self.to_s());
        ret.push_str("/");
        ret.push_str(&self.prefix.to_s());
        return ret;
    }

    pub fn to_s(&self) -> String {
        return self.ip_bits.as_compressed_string(&self.host_address);
    }

    #[allow(dead_code)]
    pub fn to_string_uncompressed(&self) -> String {
        let mut ret = String::new();
        ret.push_str(&self.to_s_uncompressed());
        ret.push_str("/");
        ret.push_str(&self.prefix.to_s());
        return ret;
    }
    #[allow(dead_code)]
    pub fn to_s_uncompressed(&self) -> String {
        return self.ip_bits.as_uncompressed_string(&self.host_address);
    }


    //  Returns the prefix as a string in IP format
    //
    //    ip = IPAddress("172.16.100.4/22")
    //
    //    ip.netmask
    //      // => "255.255.252.0"
    //
    // pub fn netmask(&self) -> String {
    //   return (self.to_ip_str_compressed)(&self.prefix.net_mask())
    // }

    //  Like IPv4// prefix=, this method allow you to
    //  change the prefix / netmask of an IP address
    //  object.
    //
    //    ip = IPAddress("172.16.100.4")
    //
    //    puts ip
    //      // => 172.16.100.4/16
    //
    //    ip.netmask = "255.255.252.0"
    //
    //    puts ip
    //      // => 172.16.100.4/22
    //
    // pub fn set_netmask(&self, addr: &String) {
    //     self.prefix = Prefix::parse_netmask_to_bits(addr)
    // }

    //  Returns the address portion in unsigned
    //  32 bits integer format.
    //
    //  This method is identical to the C function
    //  inet_pton to create a 32 bits address family
    //  structure.
    //
    //    ip = IPAddress("10.0.0.0/8")
    //
    //    ip.to_i
    //      // => 167772160
    //
    // pub fn to_i() -> BigUint {
    //   return self.host_address
    // }
    // //
    //  Returns the address portion of an IPv4 object
    //  in a network byte order format.
    //
    //    ip = IPAddress("172.16.10.1/24")
    //
    //    ip.data
    //      // => "\254\020\n\001"
    //
    //  It is usually used to include an IP address
    //  in a data packet to be sent over a socket
    //
    //    a = Socket.open(params) //  socket details here
    //    ip = IPAddress("10.1.1.0/24")
    //    binary_data = ["Address: "].pack("a*") + ip.data
    //
    //    //  Send binary data
    //    a.puts binary_data
    //
    // pub fn data(&self) {
    //   self.ip32
    // }

    //  Returns the octet specified by index
    //
    //    ip = IPAddress("172.16.100.50/24")
    //
    //    ip[0]
    //      // => 172
    //    ip[1]
    //      // => 16
    //    ip[2]
    //      // => 100
    //    ip[3]
    //      // => 50
    //
    // pub fn get(&self, index: u8) {
    //   self.octets.get(index)
    // }
    // pub fn octet(&self, index: u8) {
    //   self.octets.get(index)
    // }

    //  Returns the address portion of an IP in binary format,
    //  as a string containing a sequence of 0 and 1
    //
    //    ip = IPAddress("127.0.0.1")
    //
    //    ip.bits
    //      // => "01111111000000000000000000000001"
    //
    #[allow(dead_code)]
    pub fn bits(&self) -> String {
        return self.host_address.to_str_radix(2);
    }
    #[allow(dead_code)]
    pub fn to_hex(&self) -> String {
        return self.host_address.to_str_radix(16);
    }

    pub fn netmask(&self) -> IPAddress {
        self.from(&self.prefix.netmask(), &self.prefix)
    }

    //  Returns the broadcast address for the given IP.
    //
    //    ip = IPAddress("172.16.10.64/24")
    //
    //    ip.broadcast.to_s
    //      // => "172.16.10.255"
    //
    #[allow(dead_code)]
    pub fn broadcast(&self) -> IPAddress {
        return self.from(&self.network().host_address.add(self.size().sub(BigUint::one())), &self.prefix);
        // IPv4::parse_u32(self.broadcast_u32, self.prefix)
    }

    //  Checks if the IP address is actually a network
    //
    //    ip = IPAddress("172.16.10.64/24")
    //
    //    ip.network?
    //      // => false
    //
    //    ip = IPAddress("172.16.10.64/26")
    //
    //    ip.network?
    //      // => true
    //
    #[allow(dead_code)]
    pub fn is_network(&self) -> bool {
        return self.host_address == IPAddress::to_network(&self.host_address, self.prefix.num);
    }

    //  Returns a new IPv4 object with the network number
    //  for the given IP.
    //
    //    ip = IPAddress("172.16.10.64/24")
    //
    //    ip.network.to_s
    //      // => "172.16.10.0"
    //
    #[allow(dead_code)]
    pub fn network(&self) -> IPAddress {
        return self.from(&IPAddress::to_network(&self.host_address, self.prefix.num), &self.prefix);
    }
    #[allow(dead_code)]
    pub fn to_network(adr: &BigUint, prefix: usize) -> BigUint {
        let u = prefix.to_usize().unwrap();
        return (adr.clone() >> u) << u;
    }

    pub fn sub(&self, other: &IPAddress) -> BigUint {
        if self.host_address > other.host_address {
            return self.host_address.clone().sub(&other.host_address);
        }
        return other.host_address.clone().sub(&self.host_address);
    }

    pub fn add(&self, other: &IPAddress) -> Vec<IPAddress> {
        return IPAddress::aggregate(&[self.clone(), other.clone()].to_vec());
    }

    pub fn to_s_vec(vec: &Vec<IPAddress>) -> Vec<String> {
        let mut ret : Vec<String> = Vec::new();
        for i in vec {
            ret.push(i.to_s());
        }
        return ret;
    }

    pub fn to_string_vec(vec: &Vec<IPAddress>) -> Vec<String> {
        let mut ret : Vec<String> = Vec::new();
        for i in vec {
            ret.push(i.to_string());
        }
        return ret;
    }

    pub fn to_ipaddress_vec<S: Into<String>>(vec: Vec<S>) -> Result<Vec<IPAddress>, String> {
        let mut ret = Vec::new();
        for ipstr in vec {
            let ipa = IPAddress::parse(ipstr);
            if ipa.is_err() {
                return Err(ipa.unwrap_err());
            }
            ret.push(ipa.unwrap());
        }
        return Ok(ret);
    }

    //  Returns a new IPv4 object with the
    //  first host IP address in the range.
    //
    //  Example: given the 192.168.100.0/24 network, the first
    //  host IP address is 192.168.100.1.
    //
    //    ip = IPAddress("192.168.100.0/24")
    //
    //    ip.first.to_s
    //      // => "192.168.100.1"
    //
    //  The object IP doesn't need to be a network: the method
    //  automatically gets the network number from it
    //
    //    ip = IPAddress("192.168.100.50/24")
    //
    //    ip.first.to_s
    //      // => "192.168.100.1"
    //
    pub fn first(&self) -> IPAddress {
        return self.from(&self.network().host_address.add(&self.ip_bits.host_ofs), &self.prefix);
    }

    //  Like its sibling method IPv4// first, this method
    //  returns a new IPv4 object with the
    //  last host IP address in the range.
    //
    //  Example: given the 192.168.100.0/24 network, the last
    //  host IP address is 192.168.100.254
    //
    //    ip = IPAddress("192.168.100.0/24")
    //
    //    ip.last.to_s
    //      // => "192.168.100.254"
    //
    //  The object IP doesn't need to be a network: the method
    //  automatically gets the network number from it
    //
    //    ip = IPAddress("192.168.100.50/24")
    //
    //    ip.last.to_s
    //      // => "192.168.100.254"
    //
    #[allow(dead_code)]
    pub fn last(&self) -> IPAddress {
        return self.from(&self.broadcast().host_address.sub(&self.ip_bits.host_ofs), &self.prefix);
    }

    //  Iterates over all the hosts IP addresses for the given
    //  network (or IP address).
    //
    //    ip = IPAddress("10.0.0.1/29")
    //
    //    ip.each_host do |i|
    //      p i.to_s
    //    end
    //      // => "10.0.0.1"
    //      // => "10.0.0.2"
    //      // => "10.0.0.3"
    //      // => "10.0.0.4"
    //      // => "10.0.0.5"
    //      // => "10.0.0.6"
    //
    #[allow(dead_code)]
    pub fn each_host<F>(&self, func: F) where F : Fn(&IPAddress) {
        let mut i = self.first().host_address;
        while i <= self.last().host_address {
            func(&mut self.from(&i, &self.prefix));
            i = i.add(BigUint::one());
        }
    }

    //  Iterates over all the IP addresses for the given
    //  network (or IP address).
    //
    //  The object yielded is a new IPv4 object created
    //  from the iteration.
    //
    //    ip = IPAddress("10.0.0.1/29")
    //
    //    ip.each do |i|
    //      p i.address
    //    end
    //      // => "10.0.0.0"
    //      // => "10.0.0.1"
    //      // => "10.0.0.2"
    //      // => "10.0.0.3"
    //      // => "10.0.0.4"
    //      // => "10.0.0.5"
    //      // => "10.0.0.6"
    //      // => "10.0.0.7"
    //
    #[allow(dead_code)]
    pub fn each<F>(&self, func: F) where F : Fn(&IPAddress) {
        let mut i = self.network().host_address;
        while i <= self.broadcast().host_address {
            func(&self.from(&i, &self.prefix));
            i = i.add(BigUint::one());
        }
    }

    //  Spaceship operator to compare IPv4 objects
    //
    //  Comparing IPv4 addresses is useful to ordinate
    //  them into lists that match our intuitive
    //  perception of ordered IP addresses.
    //
    //  The first comparison criteria is the u32 value.
    //  For example, 10.100.100.1 will be considered
    //  to be less than 172.16.0.1, because, in a ordered list,
    //  we expect 10.100.100.1 to come before 172.16.0.1.
    //
    //  The second criteria, in case two IPv4 objects
    //  have identical addresses, is the prefix. An higher
    //  prefix will be considered greater than a lower
    //  prefix. This is because we expect to see
    //  10.100.100.0/24 come before 10.100.100.0/25.
    //
    //  Example:
    //
    //    ip1 = IPAddress "10.100.100.1/8"
    //    ip2 = IPAddress "172.16.0.1/16"
    //    ip3 = IPAddress "10.100.100.1/16"
    //
    //    ip1 < ip2
    //      // => true
    //    ip1 > ip3
    //      // => false
    //
    //    [ip1,ip2,ip3].sort.map{|i| i.to_string}
    //      // => ["10.100.100.1/8","10.100.100.1/16","172.16.0.1/16"]
    //

    //  Returns the number of IP addresses included
    //  in the network. It also counts the network
    //  address and the broadcast address.
    //
    //    ip = IPAddress("10.0.0.1/29")
    //
    //    ip.size
    //      // => 8
    //
    #[allow(dead_code)]
    pub fn size(&self) -> BigUint {
        return BigUint::one() << (self.prefix.host_prefix() as usize);
    }
    #[allow(dead_code)]
    pub fn is_same_kind(&self, oth: &IPAddress) -> bool {
        return self.is_ipv4() == oth.is_ipv4() &&
        self.is_ipv6() == oth.is_ipv6();
    }

    //  Checks whether a subnet includes the given IP address.
    //
    //  Accepts an IPAddress::IPv4 object.
    //
    //    ip = IPAddress("192.168.10.100/24")
    //
    //    addr = IPAddress("192.168.10.102/24")
    //
    //    ip.include? addr
    //      // => true
    //
    //    ip.include? IPAddress("172.16.0.48/16")
    //      // => false
    //
    #[allow(dead_code)]
    pub fn includes(&self, oth: &IPAddress) -> bool {
        return !self.is_same_kind(oth) && self.prefix.num <= oth.prefix.num &&
        self.network().host_address == IPAddress::to_network(&oth.host_address, self.prefix.num);
    }

    //  Checks whether a subnet includes all the
    //  given IPv4 objects.
    //
    //    ip = IPAddress("192.168.10.100/24")
    //
    //    addr1 = IPAddress("192.168.10.102/24")
    //    addr2 = IPAddress("192.168.10.103/24")
    //
    //    ip.include_all?(addr1,addr2)
    //      // => true
    //
    #[allow(dead_code)]
    pub fn includes_all(&self, oths: &[IPAddress]) -> bool {
        for oth in oths {
            if !self.includes(&oth) {
                return false;
            }
        }
        return true;
    }
    //  Checks if an IPv4 address objects belongs
    //  to a private network RFC1918
    //
    //  Example:
    //
    //    ip = IPAddress "10.1.1.1/24"
    //    ip.private?
    //      // => true
    //
    #[allow(dead_code)]
    pub fn is_private(&self) -> bool {
        return (self.vt_is_private)(self);
    }


    //  Returns the IP address in in-addr.arpa format
    //  for DNS Domain definition entries like SOA Records
    //
    //    ip = IPAddress("172.17.100.50/15")
    //
    //    ip.rev_domains
    //      // => ["16.172.in-addr.arpa","17.172.in-addr.arpa"]
    //
    pub fn rev_domains(&self) -> Vec<String> {
        let mut ret: Vec<String> = Vec::new();
        for net in self.dns_networks() {
            let mut domain = String::new();
            domain.push_str(&net.to_s());
            domain.push_str(self.ip_bits.rev_domain);
            ret.push(domain);
        }
        return ret;
    }
    //     // let mut net = vec![self.network()];
    //     // let mut cut = 4 - (self.prefix.num / 8);
    //     // if self.prefix.num <= 8 {
    //     //     //  edge case class a
    //     //     cut = 3;
    //     // } else if self.prefix.num > 24 {
    //     //     //  edge case class c
    //     //     cut = 1;
    //     //     net = [network.supernet(24)];
    //     // }
    //     // if self.prefix.num < 24 && (self.prefix.num % 8) != 0 {
    //     //     //  case class less
    //     //     cut = 3 - (self.prefix.num / 8);
    //     //     net = network.subnet(self.prefix.num + 1);
    //     // }
    //     // return net.map(|n| n.reverse.split('.')[cut..-1].join('.'));
    //     return Err("not implemented!");
    // }

    //  Splits a network into different subnets
    //
    //  If the IP Address is a network, it can be divided into
    //  multiple networks. If +self+ is not a network, this
    //  method will calculate the network from the IP and then
    //  subnet it.
    //
    //  If +subnets+ is an power of two number, the resulting
    //  networks will be divided evenly from the supernet.
    //
    //    network = IPAddress("172.16.10.0/24")
    //
    //    network / 4   //  implies map{|i| i.to_string}
    //      // => ["172.16.10.0/26",
    //           "172.16.10.64/26",
    //           "172.16.10.128/26",
    //           "172.16.10.192/26"]
    //
    //  If +num+ is any other number, the supernet will be
    //  divided into some networks with a even number of hosts and
    //  other networks with the remaining addresses.
    //
    //    network = IPAddress("172.16.10.0/24")
    //
    //    network / 3   //  implies map{|i| i.to_string}
    //      // => ["172.16.10.0/26",
    //           "172.16.10.64/26",
    //           "172.16.10.128/25"]
    //
    //  Returns an array of IPv4 objects
    //
    #[allow(dead_code)]
    pub fn split(&self, subnets: usize) -> Result<Vec<IPAddress>, String> {
        if subnets <= 1 || (1 << self.prefix.host_prefix()) <= subnets {
            return Err(format!("Value {} out of range", subnets));
        }
        let networks = self.subnet(self.newprefix(subnets).unwrap().num);
        if networks.is_err() {
            return networks;
        }
        let net = networks.unwrap();
        if net.len() != (subnets as usize) {
            return Ok(self.sum_first_found(&net));
        }
        return Ok(net);
    }
    // alias_method :/, :split

    //  Returns a new IPv4 object from the supernetting
    //  of the instance network.
    //
    //  Supernetting is similar to subnetting, except
    //  that you getting as a result a network with a
    //  smaller prefix (bigger host space). For example,
    //  given the network
    //
    //    ip = IPAddress("172.16.10.0/24")
    //
    //  you can supernet it with a new /23 prefix
    //
    //    ip.supernet(23).to_string
    //      // => "172.16.10.0/23"
    //
    //  However if you supernet it with a /22 prefix, the
    //  network address will change:
    //
    //    ip.supernet(22).to_string
    //      // => "172.16.8.0/22"
    //
    //  If +new_prefix+ is less than 1, returns 0.0.0.0/0
    //
    #[allow(dead_code)]
    pub fn supernet(&self, new_prefix: usize) -> Result<IPAddress, String> {
        if new_prefix >= self.prefix.num {
            return Err(format!("New prefix must be smaller than existing prefix: {} >= {}",
                               new_prefix,
                               self.prefix.num));
        }
        let mut new_ip = self.host_address.clone();
        for _ in new_prefix..self.prefix.num {
            new_ip = new_ip << 1;
        }
        return Ok(self.from(&new_ip, &self.prefix));
    }

    //  This method implements the subnetting function
    //  similar to the one described in RFC3531.
    //
    //  By specifying a new prefix, the method calculates
    //  the network number for the given IPv4 object
    //  and calculates the subnets associated to the new
    //  prefix.
    //
    //  For example, given the following network:
    //
    //    ip = IPAddress "172.16.10.0/24"
    //
    //  we can calculate the subnets with a /26 prefix
    //
    //    ip.subnets(26).map(&:to_string)
    //      // => ["172.16.10.0/26", "172.16.10.64/26",
    //           "172.16.10.128/26", "172.16.10.192/26"]
    //
    //  The resulting number of subnets will of course always be
    //  a power of two.
    //

    #[allow(dead_code)]
    pub fn subnet(&self, subprefix: usize) -> Result<Vec<IPAddress>, String> {
        if subprefix <= self.prefix.num || self.ip_bits.bits <= subprefix {
            return Err(format!("New prefix must be between {} and {}",
                               subprefix,
                               self.ip_bits.bits));
        }
        let mut ret = Vec::new();
        let mut net = self.network();
        net.prefix = net.prefix.from(subprefix).unwrap();
        for _ in 0..(1 << (subprefix - self.prefix.num)) {
            ret.push(net.clone());
            net = net.from(&net.host_address, &net.prefix);
            let size = net.size();
            net.host_address = net.host_address + size;
        }
        return Ok(ret);
    }


    //  Checks whether the ip address belongs to a
    //  RFC 791 CLASS A network, no matter
    //  what the subnet mask is.
    //
    //  Example:
    //
    //    ip = IPAddress("10.0.0.1/24")
    //
    //    ip.a?
    //      // => true
    //
    // pub fn is_class_a(&self) {
    //   return self.bits == 8 && 0 <= self.to_u32() && self.to_u32() < 0x80000000
    // }

    //  Checks whether the ip address belongs to a
    //  RFC 791 CLASS B network, no matter
    //  what the subnet mask is.
    //
    //  Example:
    //
    //    ip = IPAddress("172.16.10.1/24")
    //
    //    ip.b?
    //      // => true
    //
    // pub fn is_class_b(&self) {
    //   return self.bits == 16 && 0x80000000 <= self.to_u32() && self.to_u32() < 0xc0000000
    // }

    //  Checks whether the ip address belongs to a
    //  RFC 791 CLASS C network, no matter
    //  what the subnet mask is.
    //
    //  Example:
    //
    //    ip = IPAddress("192.168.1.1/30")
    //
    //    ip.c?
    //      // => true
    //
    // pub fn is_class_c(&self) {
    //   return self.bits == 24 && 0xc0000000 <= self.to_u32() && self.to_u32() <= u32::MAX
    // }

    //  Return the ip address in a format compatible
    //  with the IPv6 Mapped IPv4 addresses
    //
    //  Example:
    //
    //    ip = IPAddress("172.16.10.1/24")
    //
    //    ip.to_ipv6
    //      // => "ac10:0a01"
    //
    #[allow(dead_code)]
    pub fn to_ipv6(&self) -> IPAddress {
        return (self.vt_to_ipv6)(self);
    }

    //  Creates a new IPv4 object from an
    //  unsigned 32bits integer.
    //
    //    ip = IPAddress::IPv4::parse_u32(167772160)
    //
    //    ip.prefix = 8
    //    ip.to_string
    //      // => "10.0.0.0/8"
    //
    //  The +prefix+ parameter is optional:
    //
    //    ip = IPAddress::IPv4::parse_u32(167772160, 8)
    //
    //    ip.to_string
    //      // => "10.0.0.0/8"
    //
    // pub fn parse_u32(ip32: u32, prefix: u8) {
    //   IPv4::new(format!("{}/{}", IPv4::to_ipv4_str(ip32), prefix))
    // }

    //  Creates a new IPv4 object from binary data,
    //  like the one you get from a network stream.
    //
    //  For example, on a network stream the IP 172.16.0.1
    //  is represented with the binary "\254\020\n\001".
    //
    //    ip = IPAddress::IPv4::parse_data "\254\020\n\001"
    //    ip.prefix = 24
    //
    //    ip.to_string
    //      // => "172.16.10.1/24"
    //
    // pub fn self.parse_data(str, prefix=32)
    //   self.new(str.unpack("C4").join(".")+"/// {prefix}")
    // end


    //  Summarization (or aggregation) is the process when two or more
    //  networks are taken together to check if a supernet, including all
    //  and only these networks, exists. If it exists then this supernet
    //  is called the summarized (or aggregated) network.
    //
    //  It is very important to understand that summarization can only
    //  occur if there are no holes in the aggregated network, or, in other
    //  words, if the given networks fill completely the address space
    //  of the supernet. So the two rules are:
    //
    //  1) The aggregate network must contain +all+ the IP addresses of the
    //     original networks;
    //  2) The aggregate network must contain +only+ the IP addresses of the
    //     original networks;
    //
    //  A few examples will help clarify the above. Let's consider for
    //  instance the following two networks:
    //
    //    ip1 = IPAddress("172.16.10.0/24")
    //    ip2 = IPAddress("172.16.11.0/24")
    //
    //  These two networks can be expressed using only one IP address
    //  network if we change the prefix. Let Ruby do the work:
    //
    //    IPAddress::IPv4::summarize(ip1,ip2).to_s
    //      // => "172.16.10.0/23"
    //
    //  We note how the network "172.16.10.0/23" includes all the addresses
    //  specified in the above networks, and (more important) includes
    //  ONLY those addresses.
    //
    //  If we summarized +ip1+ and +ip2+ with the following network:
    //
    //    "172.16.0.0/16"
    //
    //  we would have satisfied rule // 1 above, but not rule // 2. So "172.16.0.0/16"
    //  is not an aggregate network for +ip1+ and +ip2+.
    //
    //  If it's not possible to compute a single aggregated network for all the
    //  original networks, the method returns an array with all the aggregate
    //  networks found. For example, the following four networks can be
    //  aggregated in a single /22:
    //
    //    ip1 = IPAddress("10.0.0.1/24")
    //    ip2 = IPAddress("10.0.1.1/24")
    //    ip3 = IPAddress("10.0.2.1/24")
    //    ip4 = IPAddress("10.0.3.1/24")
    //
    //    IPAddress::IPv4::summarize(ip1,ip2,ip3,ip4).to_string
    //      // => "10.0.0.0/22",
    //
    //  But the following networks can't be summarized in a single network:
    //
    //    ip1 = IPAddress("10.0.1.1/24")
    //    ip2 = IPAddress("10.0.2.1/24")
    //    ip3 = IPAddress("10.0.3.1/24")
    //    ip4 = IPAddress("10.0.4.1/24")
    //
    //    IPAddress::IPv4::summarize(ip1,ip2,ip3,ip4).map{|i| i.to_string}
    //      // => ["10.0.1.0/24","10.0.2.0/23","10.0.4.0/24"]
    //
    // pub fn self.summarize(args)
    //   IPAddress.summarize(args)
    // end

    //  Creates a new IPv4 address object by parsing the
    //  address in a classful way.
    //
    //  Classful addresses have a fixed netmask based on the
    //  class they belong to:
    //
    //  * Class A, from 0.0.0.0 to 127.255.255.255
    //  * Class B, from 128.0.0.0 to 191.255.255.255
    //  * Class C, D and E, from 192.0.0.0 to 255.255.255.254
    //
    //  Example:
    //
    //    ip = IPAddress::IPv4.parse_classful "10.0.0.1"
    //
    //    ip.netmask
    //      // => "255.0.0.0"
    //    ip.a?
    //      // => true
    //
    //  Note that classes C, D and E will all have a default
    //  prefix of /24 or 255.255.255.0
    //
    // pub fn self.parse_classful(ip: String) {
    //   if (IPAddress::valid_ipv4?(ip)) {
    //     address = ip.strip
    //   }
    //     raise ArgumentError, "Invalid IP // {ip.inspect}"
    //   end
    //   prefix = CLASSFUL.find{|h,k| h === ("%.8b" % address.to_i)}.last
    //   self.new "// {address}/// {prefix}"
    // }

    //  private methods
    //
    #[allow(dead_code)]
    fn newprefix(&self, num: usize) -> Result<Prefix, String> {
        for i in num..self.ip_bits.bits {
            let a = ((i as f64).log2() as usize) as f64;
            if a == (i as f64).log2() {
                return self.prefix.add(a as usize);
            }
        }
        return Err(format!("newprefix not found {}:{}", num, self.ip_bits.bits));
    }
    #[allow(dead_code)]
    fn sum_first_found(&self, arr: &Vec<IPAddress>) -> Vec<IPAddress> {
        let mut dup = arr.clone();
        dup.reverse();
        for i in 0..dup.len() {
            let a = IPAddress::summarize(&vec![dup[i].clone(), dup[i + 1].clone()]);
            if a.len() == 1 {
                for j in i..(i+1) {
                    dup[j] = a[j-i].clone();
                }
                break;
            }
        }
        dup.reverse();
        return dup;
    }


}


// IPAddress is a wrapper method built around
// IPAddress's library classes. Its purpouse is to
// make you indipendent from the type of IP address
// you're going to use.
//
// For example, instead of creating the three types
// of IP addresses using their own contructors
//
//   ip  = IPAddress::IPv4.new "172.16.10.1/24"
//   ip6 = IPAddress::IPv6.new "2001:db8::8:800:200c:417a/64"
//   ip_mapped = IPAddress::IPv6::Mapped "::ffff:172.16.10.1/128"
//
// you can just use the IPAddress wrapper:
//
//   ip  = IPAddress "172.16.10.1/24"
//   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
//   ip_mapped = IPAddress "::ffff:172.16.10.1/128"
//
// All the object created will be instances of the
// correct class:
//
//  ip.class
//    //=> IPAddress::IPv4
//  ip6.class
//    //=> IPAddress::IPv6
//  ip_mapped.class
//    //=> IPAddress::IPv6::Mapped
//
// def IPAddress(str)
//   IPAddress::parse str
// end

// Compatibility with Ruby 1.8
//
// if RUBY_VERSION =~ /1\.8/
//   class Hash // :nodoc:
//     alias :key :index
//   end
//   module Math // :nodoc:
//     def Math.log2(n)
//       log(n) / log(2)
//     end
//   end
// end
