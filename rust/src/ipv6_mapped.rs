
// use num::bigint::BigUint;
// use ip_bits::IpBits;
use ipaddress::IPAddress;

use num_integer::Integer;
// use core::fmt::Display;

use num_traits::cast::ToPrimitive;
use core::ops::Shr;

//  Ac
//  It is usually identified as a IPv4 mapped IPv6 address, a particular
//  IPv6 address which aids the transition from IPv4 to IPv6. The
//  structure of the address is
//
//    ::ffff:w.y.x.z
//
//  where w.x.y.z is a normal IPv4 address. For example, the following is
//  a mapped IPv6 address:
//
//    ::ffff:192.168.100.1
//
//  IPAddress is very powerful in handling mapped IPv6 addresses, as the
//  IPv4 portion is stored internally as a normal IPv4 object. Let's have
//  a look at some examples. To create a new mapped address, just use the
//  class builder itself
//
//    ip6 = IPAddress::IPv6::Mapped.new "::ffff:172.16.10.1/128"
//
//  or just use the wrapper method
//
//    ip6 = IPAddress "::ffff:172.16.10.1/128"
//
//  Let's check it's really a mapped address:
//
//    ip6.mapped?
//      // => true
//
//    ip6.to_string
//      // => "::FFFF:172.16.10.1/128"
//
//  Now with the +ipv4+ attribute, we can easily access the IPv4 portion
//  of the mapped IPv6 address:
//
//    ip6.ipv4.address
//      // => "172.16.10.1"
//
//  Internally, the IPv4 address is stored as two 16 bits
//  groups. Therefore all the usual methods for an IPv6 address are
//  working perfectly fine:
//
//    ip6.to_hex
//      // => "00000000000000000000ffffac100a01"
//
//    ip6.address
//      // => "0000:0000:0000:0000:0000:ffff:ac10:0a01"
//
//  A mapped IPv6 can also be created just by specify the address in the
//  following format:
//
//    ip6 = IPAddress "::172.16.10.1"
//
//  That is, two colons and the IPv4 address. However, as by RFC, the ffff
//  group will be automatically added at the beginning
//
//    ip6.to_string
//      => "::ffff:172.16.10.1/128"
//
//  making it a mapped IPv6 compatible address.
//
//
//  Creates a new IPv6 IPv4-mapped address
//
//    ip6 = IPAddress::IPv6::Mapped.new "::ffff:172.16.10.1/128"
//
//    ipv6.ipv4.class
//      // => IPAddress::IPv4
//
//  An IPv6 IPv4-mapped address can also be created using the
//  IPv6 only format of the address:
//
//    ip6 = IPAddress::IPv6::Mapped.new "::0d01:4403"
//
//    ip6.to_string
//      // => "::ffff:13.1.68.3"
//
pub fn new<S: Into<String>>(_str: S) -> Result<IPAddress, String> {
    let str = _str.into();
    let (ip, o_netmask) = IPAddress::split_at_slash(&str);
    let split_colon = ip.split(':').collect::<Vec<&str>>();
    // let mapped: Option<IPAddress> = None;
    let mut netmask = String::from("");
    if o_netmask.is_some() {
        netmask = o_netmask.unwrap();
    }
    let ipv4_str = String::from(*split_colon.iter().last().unwrap());
    if IPAddress::is_valid_ipv4(ipv4_str.clone()) {
        let ipv4 = IPAddress::parse(format!("{}{}", ipv4_str, netmask));
        if ipv4.is_err() {
            return ipv4;
        }
        //mapped = Some(ipv4.unwrap());
        let addr = ipv4.unwrap();
        let part_mod = ::ip_bits::v6().part_mod;
        let up_addr = addr.host_address.clone();
        let down_addr = addr.host_address.clone();
        return IPAddress::parse(format!("::ffff:{}:{}/{}",
            up_addr.shr(::ip_bits::v6().part_bits).mod_floor(&part_mod).to_u16().unwrap(),
            down_addr.mod_floor(&part_mod).to_u16().unwrap(),
            addr.prefix.num));
    } else if split_colon.len() >= 2 {
        //  IPv4 in hex form
        let last_2 = split_colon.get(split_colon.len() - 2);
        let last_1 = split_colon.get(split_colon.len() - 1);
        if !last_1.is_some() || !last_2.is_some() {
            return Err(format!("unknown hex mapped format:{}", str));
        }
        return IPAddress::parse(format!("::ffff:{}:{}{}", last_2.unwrap(), last_1.unwrap(), netmask));
    }
    return Err(format!("unknown mapped format:{}", str));
}

//  Similar to IPv6// to_s, but prints out the IPv4 address
//  in dotted decimal format
//
//    ip6 = IPAddress "::ffff:172.16.10.1/128"
//
//    ip6.to_s
//      // => "::ffff:172.16.10.1"
//
//  pub fn to_s
//    "::ffff:// {@ipv4.address}"
//  end

//  Similar to IPv6// to_string, but prints out the IPv4 address
//  in dotted decimal format
//
//
//    ip6 = IPAddress "::ffff:172.16.10.1/128"
//
//    ip6.to_string
//      // => "::ffff:172.16.10.1/128"
//
//  pub fn to_string
//    "::ffff:// {@ipv4.address}/// @prefix"
//  end

//  Checks if the IPv6 address is IPv4 mapped
//
//    ip6 = IPAddress "::ffff:172.16.10.1/128"
//
//    ip6.mapped?
//      // => true
//
//  pub fn mapped?
//    true
//  end