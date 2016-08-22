
// use core::fmt::Debug;
use prefix::Prefix;

// #[derive(Ord,PartialOrd,Eq,PartialEq,Debug,Copy,Clone)]
// pub struct Prefix128 {
// }
//
// impl Prefix128 {
//
//  Creates a new prefix object for 128 bits IPv6 addresses
//
//    prefix = IPAddressPrefix128.new 64
//      // => 64
//
//#[allow(unused_comparisons)]
pub fn new(num: usize) -> Result<Prefix, String> {
    if num <= 128 {
        //static _FROM: &'static (Fn(&Prefix, usize) -> Result<Prefix, String>) = &from;
        //static _TO_IP_STR: &'static (Fn(&Vec<u16>) -> String) = &Prefix128::to_ip_str;
        let ip_bits = ::ip_bits::v6();
        let bits = ip_bits.bits;
        return Ok(Prefix {
            num: num,
            ip_bits: ip_bits,
            net_mask: Prefix::new_netmask(num, bits),
            vt_from: from, // vt_to_ip_str: _TO_IP_STR
        });
    }
    return Err(format!("Prefix must be in range 0..128, got: {}", num));
}

#[allow(unused_variables)]
pub fn from(my: &Prefix, num: usize) -> Result<Prefix, String> {
    return new(num);
}

// pub fn to_ip_str(my: &Vec<u16>) -> String {
//     let mut out = String::from("");
//     let the_colon = String::from(":");
//     let mut colon = &String::from("");
//     for i in my {
//         out.push_str(&format!("{}{:04x}", colon, i));
//         colon = &the_colon;
//     }
//     return out;
// }

// }
