extern crate ipaddress;
extern crate num;

#[cfg(test)]
mod tests {
    use std::collections::HashMap;
    use num::bigint::BigUint;
    use std::str::FromStr;
    use ipaddress::ipv6_mapped;
    // use ipaddress::ipv6;
    use ipaddress::IPAddress;

    pub struct IPv6MappedTest {
        pub ip: IPAddress,
        pub s: &'static str,
        pub sstr: &'static str,
        pub string: &'static str,
        pub u128: BigUint,
        pub address: &'static str,
        pub valid_mapped: HashMap<&'static str, BigUint>,
        pub valid_mapped_ipv6: HashMap<&'static str, BigUint>,
        pub valid_mapped_ipv6_conversion: HashMap<&'static str, &'static str>,
    }

    pub fn setup() -> IPv6MappedTest {
        let mut valid_mapped = HashMap::new();
        valid_mapped.insert("::13.1.68.3", BigUint::from_str("281470899930115").unwrap());
        valid_mapped.insert("0:0:0:0:0:ffff:129.144.52.38",
                            BigUint::from_str("281472855454758").unwrap());
        valid_mapped.insert("::ffff:129.144.52.38",
                            BigUint::from_str("281472855454758").unwrap());
        let mut valid_mapped_ipv6 = HashMap::new();
        valid_mapped_ipv6.insert("::ffff:13.1.68.3", BigUint::from_str("281470899930115").unwrap());
        valid_mapped_ipv6.insert("0:0:0:0:0:ffff:8190:3426",
                                 BigUint::from_str("281472855454758").unwrap());
        valid_mapped_ipv6.insert("::ffff:8190:3426",
                                 BigUint::from_str("281472855454758").unwrap());
        let mut valid_mapped_ipv6_conversion = HashMap::new();
        valid_mapped_ipv6_conversion.insert("::ffff:13.1.68.3", "13.1.68.3");
        valid_mapped_ipv6_conversion.insert("0:0:0:0:0:ffff:8190:3426", "129.144.52.38");
        valid_mapped_ipv6_conversion.insert("::ffff:8190:3426", "129.144.52.38");
        return IPv6MappedTest {
            ip: ipv6_mapped::new("::172.16.10.1").unwrap(),
            s: "::ffff:172.16.10.1",
            sstr: "::ffff:172.16.10.1/32",
            string: "0000:0000:0000:0000:0000:ffff:ac10:0a01/128",
            u128: BigUint::from_str("281473568475649").unwrap(),
            address: "::ffff:ac10:a01/128",
            valid_mapped: valid_mapped,
            valid_mapped_ipv6: valid_mapped_ipv6,
            valid_mapped_ipv6_conversion: valid_mapped_ipv6_conversion,
        };
    }


    #[test]
    pub fn test_initialize() {
        let s = setup();
        assert_eq!(true, IPAddress::parse("::172.16.10.1").is_ok());
        for (ip, u128) in s.valid_mapped {
            println!("-{}--{}", ip, u128);
            if IPAddress::parse(ip).is_err() {
                println!("{}", IPAddress::parse(ip).unwrap_err());
            }
            assert_eq!(true, IPAddress::parse(ip).is_ok());
            assert_eq!(u128, IPAddress::parse(ip).unwrap().host_address);
        }
        for (ip, u128) in s.valid_mapped_ipv6 {
            println!("===={}=={:x}", ip, u128);
            assert_eq!(true, IPAddress::parse(ip).is_ok());
            assert_eq!(u128, IPAddress::parse(ip).unwrap().host_address);
        }
    }
    #[test]
    pub fn test_mapped_from_ipv6_conversion() {
        for (ip6, ip4) in setup().valid_mapped_ipv6_conversion {
            println!("+{}--{}", ip6, ip4);
            assert_eq!(ip4, IPAddress::parse(ip6).unwrap().mapped.unwrap().to_s());
        }
    }
    #[test]
    pub fn test_attributes() {
        let s = setup();
        assert_eq!(s.address, s.ip.to_string());
        assert_eq!(128, s.ip.prefix.num);
        assert_eq!(s.s, s.ip.to_s_mapped());
        assert_eq!(s.sstr, s.ip.to_string_mapped());
        assert_eq!(s.string, s.ip.to_string_uncompressed());
        assert_eq!(s.u128, s.ip.host_address);
    }
    #[test]
    fn test_method_ipv6() {
        assert!(setup().ip.is_ipv6());
    }
    #[test]
    fn test_mapped() {
        assert!(setup().ip.is_mapped());
    }
}
