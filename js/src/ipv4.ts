
class Ipv4 {
public static from_u32(addr: u32, prefix: usize) : Result<IPAddress, String> {
    let prefix = prefix32::new(prefix);
    if prefix.is_err() {
        return Err(prefix.unwrap_err());
    }
    return Ok(IPAddress {
        ip_bits: ::ip_bits::v4(),
        host_address: BigUint::from_u32(addr).unwrap(),
        prefix: prefix.unwrap(),
        mapped: None,
        vt_is_private: ipv4_is_private,
        vt_is_loopback: ipv4_is_loopback,
        vt_to_ipv6: to_ipv6,
    });
}

public static new<S: Into<String>>(_str: S) : Result<IPAddress, String> {
    let str = _str.into();
    let (ip, netmask) = IPAddress::split_at_slash(&str);
    if !IPAddress::is_valid_ipv4(ip.clone()) {
        return Err(format!("Invalid IP {}", str));
    }
    let mut ip_prefix_num = Ok(32);
    if netmask.is_some() {
        //  netmask is defined
        ip_prefix_num = IPAddress::parse_netmask_to_prefix(netmask.unwrap());
        if ip_prefix_num.is_err() {
            return Err(ip_prefix_num.unwrap_err());
        }
        //if ip_prefix.ip_bits.version
    }
    let ip_prefix = prefix32::new(ip_prefix_num.unwrap());
    if ip_prefix.is_err() {
        return Err(ip_prefix.unwrap_err());
    }
    let split_u32 = IPAddress::split_to_u32(&ip);
    if split_u32.is_err() {
        return Err(split_u32.unwrap_err());
    }
    return Ok(IPAddress {
        ip_bits: ::ip_bits::v4(),
        host_address: BigUint::from_u32(split_u32.unwrap()).unwrap(),
        prefix: ip_prefix.unwrap(),
        mapped: None,
        vt_is_private: ipv4_is_private,
        vt_is_loopback: ipv4_is_loopback,
        vt_to_ipv6: to_ipv6,
    });
}

public static ipv4_is_private(my: IPAddress) : bool {
    return [IPAddress::parse("10.0.0.0/8").unwrap(),
     IPAddress::parse("172.16.0.0/12").unwrap(),
     IPAddress::parse("192.168.0.0/16").unwrap()]
     .iter().find(|i| i.includes(my)).is_some();
}

public static ipv4_is_loopback(my: IPAddress) : bool {
    return IPAddress::parse("127.0.0.0/8")
        .unwrap().includes(my);
}

public static to_ipv6(ia: IPAddress) : IPAddress {
        return IPAddress {
            ip_bits: ::ip_bits::v6(),
            host_address: ia.host_address.clone(),
            prefix: ::prefix128::new(ia.prefix.num).unwrap(),
            mapped: None,
            vt_is_private: ::ipv6::ipv6_is_private,
            vt_is_loopback: ::ipv6::ipv6_is_loopback,
            vt_to_ipv6: ::ipv6::to_ipv6
        }
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
public static is_class_a(my: IPAddress) : bool {
    return my.is_ipv4() && my.host_address < BigUint::from_u32(0x80000000).unwrap();
}

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
public static is_class_b(my: IPAddress) : bool {
    return my.is_ipv4() &&
        BigUint::from_u32(0x80000000).unwrap() <= my.host_address &&
        my.host_address < BigUint::from_u32(0xc0000000).unwrap();
}

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
public static is_class_c(my: IPAddress) : bool {
    return my.is_ipv4() &&
        BigUint::from_u32(0xc0000000).unwrap() <= my.host_address &&
        my.host_address < BigUint::from_u32(0xe0000000).unwrap();
}


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
public parse_classful<S: Into<String>>(ip_s: S) : Result<IPAddress, String> {
    let ip_si = ip_s.into();
    if !IPAddress::is_valid_ipv4(ip_si.clone()) {
        return Err(format!("Invalid IP {}", ip_si));
    }
    let o_ip = IPAddress::parse(ip_si.clone());
    if o_ip.is_err() {
        return o_ip;
    }
    let mut ip = o_ip.unwrap();
    if ::ipv4::is_class_a(&ip) {
        ip.prefix = ::prefix32::new(8).unwrap();
    } else if ::ipv4::is_class_b(&ip) {
        ip.prefix = ::prefix32::new(16).unwrap();
    } else if ::ipv4::is_class_c(&ip) {
        ip.prefix = ::prefix32::new(24).unwrap();
    }
    return Ok(ip);
}
}
