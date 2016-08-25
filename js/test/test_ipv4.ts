extern crate ipaddress;
extern crate num;

#[cfg(test)]
mod tests {
    use num::bigint::BigUint;
    use num::ToPrimitive;
    use std::str::FromStr;
    use ipaddress::IPAddress;
    use ipaddress::ipv4;
    use std::collections::HashMap;
    use std::sync::{Arc, Mutex};
    use std::ops::Deref;

    #[derive(Debug)]
    struct IPv4Prefix {
        pub ip: String,
        pub prefix: usize,
    }

    struct IPv4Test {
        pub valid_ipv4: HashMap<&'static str, IPv4Prefix>,
        pub invalid_ipv4: Vec<&'static str>,
        pub valid_ipv4_range: Vec<&'static str>,
        pub netmask_values: HashMap<&'static str, &'static str>,
        pub decimal_values: HashMap<&'static str, u32>,
        pub ip: IPAddress,
        pub network: IPAddress,
        pub networks: HashMap<&'static str, &'static str>,
        pub broadcast: HashMap<&'static str, &'static str>,
        pub class_a: IPAddress,
        pub class_b: IPAddress,
        pub class_c: IPAddress,
        pub classful: HashMap<&'static str, usize>,
    }
    fn setup() -> IPv4Test {
        let mut ipv4t = IPv4Test {
            valid_ipv4: HashMap::new(),
            // , "10.0.0", "10.0"
            invalid_ipv4: vec!["10.0.0.256", "10.0.0.0.0"],
            valid_ipv4_range: vec!["10.0.0.1-254", "10.0.1-254.0", "10.1-254.0.0"],
            netmask_values: HashMap::new(),
            decimal_values: HashMap::new(),
            ip: ipv4::new("172.16.10.1/24").unwrap(),
            network: ipv4::new("172.16.10.0/24").unwrap(),
            networks: HashMap::new(),
            broadcast: HashMap::new(),
            class_a: ipv4::new("10.0.0.1/8").unwrap(),
            class_b: ipv4::new("172.16.0.1/16").unwrap(),
            class_c: ipv4::new("192.168.0.1/24").unwrap(),
            classful: HashMap::new(),
        };
        ipv4t.valid_ipv4.insert("9.9/17",
                                IPv4Prefix {
                                    ip: String::from_str("9.0.0.9").unwrap(),
                                    prefix: 17,
                                });
        ipv4t.valid_ipv4.insert("100.1.100",
                                IPv4Prefix {
                                    ip: String::from_str("100.1.0.100").unwrap(),
                                    prefix: 32,
                                });
        ipv4t.valid_ipv4.insert("0.0.0.0/0",
                                IPv4Prefix {
                                    ip: String::from_str("0.0.0.0").unwrap(),
                                    prefix: 0,
                                });
        ipv4t.valid_ipv4.insert("10.0.0.0",
                                IPv4Prefix {
                                    ip: String::from_str("10.0.0.0").unwrap(),
                                    prefix: 32,
                                });
        ipv4t.valid_ipv4.insert("10.0.0.1",
                                IPv4Prefix {
                                    ip: String::from_str("10.0.0.1").unwrap(),
                                    prefix: 32,
                                });
        ipv4t.valid_ipv4.insert("10.0.0.1/24",
                                IPv4Prefix {
                                    ip: String::from_str("10.0.0.1").unwrap(),
                                    prefix: 24,
                                });
        ipv4t.valid_ipv4.insert("10.0.0.9/255.255.255.0",
                                IPv4Prefix {
                                    ip: String::from_str("10.0.0.9").unwrap(),
                                    prefix: 24,
                                });

        ipv4t.netmask_values.insert("0.0.0.0/0", "0.0.0.0");
        ipv4t.netmask_values.insert("10.0.0.0/8", "255.0.0.0");
        ipv4t.netmask_values.insert("172.16.0.0/16", "255.255.0.0");
        ipv4t.netmask_values.insert("192.168.0.0/24", "255.255.255.0");
        ipv4t.netmask_values.insert("192.168.100.4/30", "255.255.255.252");

        ipv4t.decimal_values.insert("0.0.0.0/0", 0);
        ipv4t.decimal_values.insert("10.0.0.0/8", 167772160);
        ipv4t.decimal_values.insert("172.16.0.0/16", 2886729728);
        ipv4t.decimal_values.insert("192.168.0.0/24", 3232235520);
        ipv4t.decimal_values.insert("192.168.100.4/30", 3232261124);

        ipv4t.ip = IPAddress::parse("172.16.10.1/24").unwrap();
        ipv4t.network = IPAddress::parse("172.16.10.0/24").unwrap();

        ipv4t.broadcast.insert("10.0.0.0/8", "10.255.255.255/8");
        ipv4t.broadcast.insert("172.16.0.0/16", "172.16.255.255/16");
        ipv4t.broadcast.insert("192.168.0.0/24", "192.168.0.255/24");
        ipv4t.broadcast.insert("192.168.100.4/30", "192.168.100.7/30");

        ipv4t.networks.insert("10.5.4.3/8", "10.0.0.0/8");
        ipv4t.networks.insert("172.16.5.4/16", "172.16.0.0/16");
        ipv4t.networks.insert("192.168.4.3/24", "192.168.4.0/24");
        ipv4t.networks.insert("192.168.100.5/30", "192.168.100.4/30");

        ipv4t.class_a = IPAddress::parse("10.0.0.1/8").unwrap();
        ipv4t.class_b = IPAddress::parse("172.16.0.1/16").unwrap();
        ipv4t.class_c = IPAddress::parse("192.168.0.1/24").unwrap();

        ipv4t.classful.insert("10.1.1.1", 8);
        ipv4t.classful.insert("150.1.1.1", 16);
        ipv4t.classful.insert("200.1.1.1", 24);
        return ipv4t;
    }


    #[test]
    pub fn test_initialize() {
        let setup = setup();
        for i in setup.valid_ipv4.keys() {
            let ip = IPAddress::parse(i.to_string()).unwrap();
            assert!(ip.is_ipv4() && !ip.is_ipv6());
        }
        assert_eq!(32, setup.ip.prefix.ip_bits.bits);
        assert!(IPAddress::parse("1.f.13.1/-3").is_err());
        assert!(IPAddress::parse("10.0.0.0/8").is_ok());
    }
    #[test]
    pub fn test_initialize_format_error() {
        for i in setup().invalid_ipv4 {
            assert!(IPAddress::parse(i).is_err());
        }
        assert!(IPAddress::parse("10.0.0.0/asd").is_err());
    }
    #[test]
    pub fn test_initialize_without_prefix() {
        assert!(IPAddress::parse("10.10.0.0").is_ok());
        let ip = IPAddress::parse("10.10.0.0").unwrap();
        assert!(!ip.is_ipv6() && ip.is_ipv4());
        assert_eq!(32, ip.prefix.num);
    }
    #[test]
    pub fn test_attributes() {
        for (arg, attr) in setup().valid_ipv4 {
            let ip = IPAddress::parse(arg).unwrap();
            // println!("test_attributes:{}:{:?}", arg, attr);
            assert_eq!(attr.ip, ip.to_s());
            assert_eq!(attr.prefix, ip.prefix.num);
        }
    }
    #[test]
    pub fn test_octets() {
        let ip = IPAddress::parse("10.1.2.3/8").unwrap();
        assert_eq!(ip.parts(), [10, 1, 2, 3]);
    }
    #[test]
    pub fn test_method_to_string() {
        for (arg, attr) in setup().valid_ipv4 {
            let ip = IPAddress::parse(arg).unwrap();
            assert_eq!(format!("{}/{}", attr.ip, attr.prefix), ip.to_string());
        }
    }
    #[test]
    pub fn test_method_to_s() {
        for (arg, attr) in setup().valid_ipv4 {
            let ip = IPAddress::parse(arg).unwrap();
            assert_eq!(attr.ip, ip.to_s());
            // let ip_c = IPAddress::parse(arg).unwrap();
            // assert_eq!(attr.ip, ip.to_s());
        }
    }
    #[test]
    pub fn test_netmask() {
        for (addr, mask) in setup().netmask_values {
            let ip = IPAddress::parse(addr).unwrap();
            assert_eq!(ip.netmask().to_s(), mask);
        }
    }
    #[test]
    pub fn test_method_to_u32() {
        for (addr, int) in setup().decimal_values {
            let ip = IPAddress::parse(addr).unwrap();
            assert_eq!(ip.host_address.to_u32().unwrap(), int);
        }
    }
    #[test]
    pub fn test_method_is_network() {
        assert_eq!(true, setup().network.is_network());
        assert_eq!(false, setup().ip.is_network());
    }
    #[test]
    pub fn test_one_address_network() {
        let network = IPAddress::parse("172.16.10.1/32").unwrap();
        assert_eq!(false, network.is_network());
    }
    #[test]
    pub fn test_method_broadcast() {
        for (addr, bcast) in setup().broadcast {
            let ip = IPAddress::parse(addr).unwrap();
            assert_eq!(bcast, ip.broadcast().to_string());
        }
    }
    #[test]
    pub fn test_method_network() {
        for (addr, net) in setup().networks {
            let ip = IPAddress::parse(addr).unwrap();
            assert_eq!(net, ip.network().to_string());
        }
    }

    #[test]
    pub fn test_method_bits() {
        let ip = IPAddress::parse("127.0.0.1").unwrap();
        assert_eq!("01111111000000000000000000000001", ip.bits());
    }
    #[test]
    pub fn test_method_first() {
        let mut ip = IPAddress::parse("192.168.100.0/24").unwrap();
        assert_eq!("192.168.100.1", ip.first().to_s());
        ip = IPAddress::parse("192.168.100.50/24").unwrap();
        assert_eq!("192.168.100.1", ip.first().to_s());
    }
    #[test]
    pub fn test_method_last() {
        let mut ip = IPAddress::parse("192.168.100.0/24").unwrap();
        assert_eq!("192.168.100.254", ip.last().to_s());
        ip = IPAddress::parse("192.168.100.50/24").unwrap();
        assert_eq!("192.168.100.254", ip.last().to_s());
    }
    #[test]
    pub fn test_method_each_host() {
        let ip = IPAddress::parse("10.0.0.1/29").unwrap();
        let arr = Arc::new(Mutex::new(Vec::new()));
        ip.each_host(|i| arr.lock().unwrap().push(i.to_s()));
        assert_eq!(*arr.lock().unwrap().deref(),
                   ["10.0.0.1", "10.0.0.2", "10.0.0.3", "10.0.0.4", "10.0.0.5", "10.0.0.6"]);
    }
    #[test]
    pub fn test_method_each() {
        let ip = IPAddress::parse("10.0.0.1/29").unwrap();
        let arr = Arc::new(Mutex::new(Vec::new()));
        ip.each(|i| arr.lock().unwrap().push(i.to_s()));
        assert_eq!(*arr.lock().unwrap().deref(),
                   ["10.0.0.0", "10.0.0.1", "10.0.0.2", "10.0.0.3", "10.0.0.4", "10.0.0.5",
                    "10.0.0.6", "10.0.0.7"]);
    }
    #[test]
    pub fn test_method_size() {
        let ip = IPAddress::parse("10.0.0.1/29").unwrap();
        assert_eq!(ip.size(), BigUint::from_str("8").unwrap());
    }
    #[test]
    pub fn test_method_network_u32() {
        assert_eq!(2886732288,
                   setup().ip.network().host_address.to_u32().unwrap());
    }
    #[test]
    pub fn test_method_broadcast_u32() {
        assert_eq!(2886732543,
                   setup().ip.broadcast().host_address.to_u32().unwrap());
    }
    #[test]
    pub fn test_method_include() {
        let mut ip = IPAddress::parse("192.168.10.100/24").unwrap();
        let addr = IPAddress::parse("192.168.10.102/24").unwrap();
        assert_eq!(true, ip.includes(&addr));
        assert_eq!(false,
                   ip.includes(&IPAddress::parse("172.16.0.48").unwrap()));
        ip = IPAddress::parse("10.0.0.0/8").unwrap();
        assert_eq!(true, ip.includes(&IPAddress::parse("10.0.0.0/9").unwrap()));
        assert_eq!(true, ip.includes(&IPAddress::parse("10.1.1.1/32").unwrap()));
        assert_eq!(true, ip.includes(&IPAddress::parse("10.1.1.1/9").unwrap()));
        assert_eq!(false,
                   ip.includes(&IPAddress::parse("172.16.0.0/16").unwrap()));
        assert_eq!(false, ip.includes(&IPAddress::parse("10.0.0.0/7").unwrap()));
        assert_eq!(false, ip.includes(&IPAddress::parse("5.5.5.5/32").unwrap()));
        assert_eq!(false, ip.includes(&IPAddress::parse("11.0.0.0/8").unwrap()));
        ip = IPAddress::parse("13.13.0.0/13").unwrap();
        assert_eq!(false,
                   ip.includes(&IPAddress::parse("13.16.0.0/32").unwrap()));
    }
    #[test]
    pub fn test_method_include_all() {
        let ip = IPAddress::parse("192.168.10.100/24").unwrap();
        let addr1 = IPAddress::parse("192.168.10.102/24").unwrap();
        let addr2 = IPAddress::parse("192.168.10.103/24").unwrap();
        assert_eq!(true, ip.includes_all(&[addr1.clone(), addr2]));
        assert_eq!(false,
                   ip.includes_all(&[addr1, IPAddress::parse("13.16.0.0/32").unwrap()]));
    }
    #[test]
    pub fn test_method_ipv4() {
        assert_eq!(true, setup().ip.is_ipv4());
    }
    #[test]
    pub fn test_method_ipv6() {
        assert_eq!(false, setup().ip.is_ipv6());
    }
    #[test]
    pub fn test_method_private() {
        assert_eq!(true,
                   IPAddress::parse("192.168.10.50/24").unwrap().is_private());
        assert_eq!(true,
                   IPAddress::parse("192.168.10.50/16").unwrap().is_private());
        assert_eq!(true,
                   IPAddress::parse("172.16.77.40/24").unwrap().is_private());
        assert_eq!(true,
                   IPAddress::parse("172.16.10.50/14").unwrap().is_private());
        assert_eq!(true,
                   IPAddress::parse("10.10.10.10/10").unwrap().is_private());
        assert_eq!(true, IPAddress::parse("10.0.0.0/8").unwrap().is_private());
        assert_eq!(false,
                   IPAddress::parse("192.168.10.50/12").unwrap().is_private());
        assert_eq!(false, IPAddress::parse("3.3.3.3").unwrap().is_private());
        assert_eq!(false, IPAddress::parse("10.0.0.0/7").unwrap().is_private());
        assert_eq!(false,
                   IPAddress::parse("172.32.0.0/12").unwrap().is_private());
        assert_eq!(false,
                   IPAddress::parse("172.16.0.0/11").unwrap().is_private());
        assert_eq!(false,
                   IPAddress::parse("192.0.0.2/24").unwrap().is_private());
    }
    #[test]
    pub fn test_method_octet() {
        assert_eq!(*setup().ip.parts().get(0).unwrap(), 172);
        assert_eq!(*setup().ip.parts().get(1).unwrap(), 16);
        assert_eq!(*setup().ip.parts().get(2).unwrap(), 10);
        assert_eq!(*setup().ip.parts().get(3).unwrap(), 1);
    }
    #[test]
    pub fn test_method_a() {
        assert_eq!(true, ipv4::is_class_a(&setup().class_a));
        assert_eq!(false, ipv4::is_class_a(&setup().class_b));
        assert_eq!(false, ipv4::is_class_a(&setup().class_c));
    }
    #[test]
    pub fn test_method_b() {
        assert_eq!(true, ipv4::is_class_b(&setup().class_b));
        assert_eq!(false, ipv4::is_class_b(&setup().class_a));
        assert_eq!(false, ipv4::is_class_b(&setup().class_c));
    }
    #[test]
    pub fn test_method_c() {
        assert_eq!(true, ipv4::is_class_c(&setup().class_c));
        assert_eq!(false, ipv4::is_class_c(&setup().class_a));
        assert_eq!(false, ipv4::is_class_c(&setup().class_b));
    }
    #[test]
    pub fn test_method_to_ipv6() {
        assert_eq!("::ac10:a01", setup().ip.to_ipv6().to_s());
    }
    #[test]
    pub fn test_method_reverse() {
        assert_eq!(setup().ip.dns_reverse(), "10.16.172.in-addr.arpa");
    }
    #[test]
    pub fn test_method_dns_rev_domains() {
        assert_eq!(IPAddress::parse("173.17.5.1/23").unwrap().dns_rev_domains(),
                   ["4.17.173.in-addr.arpa", "5.17.173.in-addr.arpa"]);
        assert_eq!(IPAddress::parse("173.17.1.1/15").unwrap().dns_rev_domains(),
                   ["16.173.in-addr.arpa", "17.173.in-addr.arpa"]);
        assert_eq!(IPAddress::parse("173.17.1.1/7").unwrap().dns_rev_domains(),
                   ["172.in-addr.arpa", "173.in-addr.arpa"]);
        assert_eq!(IPAddress::parse("173.17.1.1/29").unwrap().dns_rev_domains(),
                   [
                       "0.1.17.173.in-addr.arpa",
                       "1.1.17.173.in-addr.arpa",
                       "2.1.17.173.in-addr.arpa",
                       "3.1.17.173.in-addr.arpa",
                       "4.1.17.173.in-addr.arpa",
                       "5.1.17.173.in-addr.arpa",
                       "6.1.17.173.in-addr.arpa",
                       "7.1.17.173.in-addr.arpa"
                    ]);
        assert_eq!(IPAddress::parse("174.17.1.1/24").unwrap().dns_rev_domains(),
                   ["1.17.174.in-addr.arpa"]);
        assert_eq!(IPAddress::parse("175.17.1.1/16").unwrap().dns_rev_domains(),
                   ["17.175.in-addr.arpa"]);
        assert_eq!(IPAddress::parse("176.17.1.1/8").unwrap().dns_rev_domains(),
                   ["176.in-addr.arpa"]);
        assert_eq!(IPAddress::parse("177.17.1.1/0").unwrap().dns_rev_domains(),
                   ["in-addr.arpa"]);
        assert_eq!(IPAddress::parse("178.17.1.1/32").unwrap().dns_rev_domains(),
                   ["1.1.17.178.in-addr.arpa"]);
    }
    #[test]
    pub fn test_method_compare() {
        let mut ip1 = IPAddress::parse("10.1.1.1/8").unwrap();
        let mut ip2 = IPAddress::parse("10.1.1.1/16").unwrap();
        let mut ip3 = IPAddress::parse("172.16.1.1/14").unwrap();
        let ip4 = IPAddress::parse("10.1.1.1/8").unwrap();

        // ip2 should be greater than ip1
        assert_eq!(true, ip1 < ip2);
        assert_eq!(false, ip1 > ip2);
        assert_eq!(false, ip2 < ip1);
        // ip2 should be less than ip3
        assert_eq!(true, ip2 < ip3);
        assert_eq!(false, ip2 > ip3);
        // ip1 should be less than ip3
        assert_eq!(true, ip1 < ip3);
        assert_eq!(false, ip1 > ip3);
        assert_eq!(false, ip3 < ip1);
        // ip1 should be equal to itself
        assert_eq!(true, ip1 == ip1);
        // ip1 should be equal to ip4
        assert_eq!(true, ip1 == ip4);
        // test sorting
        let mut res = [ip1, ip2, ip3];
        res.sort();
        assert_eq!(IPAddress::to_string_vec(&res.to_vec()),
                   ["10.1.1.1/8", "10.1.1.1/16", "172.16.1.1/14"]);
        // test same prefix
        ip1 = IPAddress::parse("10.0.0.0/24").unwrap();
        ip2 = IPAddress::parse("10.0.0.0/16").unwrap();
        ip3 = IPAddress::parse("10.0.0.0/8").unwrap();
        {
            let mut res = [ip1, ip2, ip3];
            res.sort();
            assert_eq!(IPAddress::to_string_vec(&res.to_vec()),
                       ["10.0.0.0/8", "10.0.0.0/16", "10.0.0.0/24"]);
        }
    }
    #[test]
    pub fn test_method_minus() {
        let ip1 = IPAddress::parse("10.1.1.1/8").unwrap();
        let ip2 = IPAddress::parse("10.1.1.10/8").unwrap();
        assert_eq!(9, ip2.sub(&ip1).to_u32().unwrap());
        assert_eq!(9, ip1.sub(&ip2).to_u32().unwrap());
    }
    #[test]
    pub fn test_method_plus() {
        let mut ip1 = IPAddress::parse("172.16.10.1/24").unwrap();
        let mut ip2 = IPAddress::parse("172.16.11.2/24").unwrap();
        assert_eq!(IPAddress::to_string_vec(&ip1.add(&ip2)), ["172.16.10.0/23"]);

        ip2 = IPAddress::parse("172.16.12.2/24").unwrap();
        assert_eq!(IPAddress::to_string_vec(&ip1.add(&ip2)),
                   [ip1.network().to_string(), ip2.network().to_string()]);

        ip1 = IPAddress::parse("10.0.0.0/23").unwrap();
        ip2 = IPAddress::parse("10.0.2.0/24").unwrap();
        assert_eq!(IPAddress::to_string_vec(&ip1.add(&ip2)),
                   ["10.0.0.0/23", "10.0.2.0/24"]);

        ip1 = IPAddress::parse("10.0.0.0/23").unwrap();
        ip2 = IPAddress::parse("10.0.2.0/24").unwrap();
        assert_eq!(IPAddress::to_string_vec(&ip1.add(&ip2)),
                   ["10.0.0.0/23", "10.0.2.0/24"]);

        ip1 = IPAddress::parse("10.0.0.0/16").unwrap();
        ip2 = IPAddress::parse("10.0.2.0/24").unwrap();
        assert_eq!(IPAddress::to_string_vec(&ip1.add(&ip2)), ["10.0.0.0/16"]);

        ip1 = IPAddress::parse("10.0.0.0/23").unwrap();
        ip2 = IPAddress::parse("10.1.0.0/24").unwrap();
        assert_eq!(IPAddress::to_string_vec(&ip1.add(&ip2)),
                   ["10.0.0.0/23", "10.1.0.0/24"]);
    }
    #[test]
    pub fn test_method_netmask_equal() {
        let ip = IPAddress::parse("10.1.1.1/16").unwrap();
        assert_eq!(16, ip.prefix.num);
        let ip2 = ip.change_netmask("255.255.255.0").unwrap();
        assert_eq!(24, ip2.prefix.num);
    }
    #[test]
    pub fn test_method_split() {
        assert!(setup().ip.split(0).is_err());
        assert!(setup().ip.split(257).is_err());

        assert_eq!(setup().ip.split(1).unwrap(), [setup().ip.network()]);

        assert_eq!(IPAddress::to_string_vec(&setup().network.split(8).unwrap()),
                   ["172.16.10.0/27",
                    "172.16.10.32/27",
                    "172.16.10.64/27",
                    "172.16.10.96/27",
                    "172.16.10.128/27",
                    "172.16.10.160/27",
                    "172.16.10.192/27",
                    "172.16.10.224/27"]);

        assert_eq!(IPAddress::to_string_vec(&setup().network.split(7).unwrap()),
                   ["172.16.10.0/27",
                    "172.16.10.32/27",
                    "172.16.10.64/27",
                    "172.16.10.96/27",
                    "172.16.10.128/27",
                    "172.16.10.160/27",
                    "172.16.10.192/26"]);

        assert_eq!(IPAddress::to_string_vec(&setup().network.split(6).unwrap()),
                   ["172.16.10.0/27",
                    "172.16.10.32/27",
                    "172.16.10.64/27",
                    "172.16.10.96/27",
                    "172.16.10.128/26",
                    "172.16.10.192/26"]);
        assert_eq!(IPAddress::to_string_vec(&setup().network.split(5).unwrap()),
                   ["172.16.10.0/27",
                    "172.16.10.32/27",
                    "172.16.10.64/27",
                    "172.16.10.96/27",
                    "172.16.10.128/25"]);
        assert_eq!(IPAddress::to_string_vec(&setup().network.split(4).unwrap()),
                   ["172.16.10.0/26", "172.16.10.64/26", "172.16.10.128/26", "172.16.10.192/26"]);
        assert_eq!(IPAddress::to_string_vec(&setup().network.split(3).unwrap()),
                   ["172.16.10.0/26", "172.16.10.64/26", "172.16.10.128/25"]);
        assert_eq!(IPAddress::to_string_vec(&setup().network.split(2).unwrap()),
                   ["172.16.10.0/25", "172.16.10.128/25"]);
        assert_eq!(IPAddress::to_string_vec(&setup().network.split(1).unwrap()),
                   ["172.16.10.0/24"]);
    }
    #[test]
    pub fn test_method_subnet() {
        assert!(setup().network.subnet(23).is_err());
        assert!(setup().network.subnet(33).is_err());
        assert!(setup().ip.subnet(30).is_ok());
        assert_eq!(IPAddress::to_string_vec(&setup().network.subnet(26).unwrap()),
                                 ["172.16.10.0/26",
                                  "172.16.10.64/26",
                                  "172.16.10.128/26",
                                  "172.16.10.192/26"]);
        assert_eq!(IPAddress::to_string_vec(&setup().network.subnet(25).unwrap()),
                   ["172.16.10.0/25", "172.16.10.128/25"]);
        assert_eq!(IPAddress::to_string_vec(&setup().network.subnet(24).unwrap()),
                   ["172.16.10.0/24"]);
    }
    #[test]
    pub fn test_method_supernet() {
        assert!(setup().ip.supernet(24).is_err());
        assert_eq!("0.0.0.0/0", setup().ip.supernet(0).unwrap().to_string());
        // assert_eq!("0.0.0.0/0", setup().ip.supernet(-2).unwrap().to_string());
        assert_eq!("172.16.10.0/23",
                   setup().ip.supernet(23).unwrap().to_string());
        assert_eq!("172.16.8.0/22",
                   setup().ip.supernet(22).unwrap().to_string());
    }
    #[test]
    pub fn test_classmethod_parse_u32() {
        for (addr, int) in setup().decimal_values {
            let ip = ipv4::from_u32(int, 32).unwrap();
            let splitted: Vec<&str> = addr.split("/").collect();
            let ip2 = ip.change_prefix(splitted[1].parse::<usize>().unwrap()).unwrap();
            assert_eq!(ip2.to_string(), addr);
        }
    }

    // pub fn test_classhmethod_extract() {
    //   let str = "foobar172.16.10.1barbaz";
    //   assert_eq!("172.16.10.1", IPAddress::extract(str).to_s
    // }
    #[test]
    pub fn test_classmethod_summarize() {

        // Should return self if only one network given
        assert_eq!(IPAddress::summarize(&vec![setup().ip]),
                   [setup().ip.network()]);

        // Summarize homogeneous networks
        let mut ip1 = IPAddress::parse("172.16.10.1/24").unwrap();
        let mut ip2 = IPAddress::parse("172.16.11.2/24").unwrap();
        assert_eq!(IPAddress::to_string_vec(&IPAddress::summarize(&vec![ip1, ip2])),
                   ["172.16.10.0/23"]);

        {
            let ip1 = IPAddress::parse("10.0.0.1/24").unwrap();
            let ip2 = IPAddress::parse("10.0.1.1/24").unwrap();
            let ip3 = IPAddress::parse("10.0.2.1/24").unwrap();
            let ip4 = IPAddress::parse("10.0.3.1/24").unwrap();
            assert_eq!(IPAddress::to_string_vec(&IPAddress::summarize(&vec![ip1, ip2, ip3, ip4])),
                       ["10.0.0.0/22"]);
        }
        {
            let ip1 = IPAddress::parse("10.0.0.1/24").unwrap();
            let ip2 = IPAddress::parse("10.0.1.1/24").unwrap();
            let ip3 = IPAddress::parse("10.0.2.1/24").unwrap();
            let ip4 = IPAddress::parse("10.0.3.1/24").unwrap();
            assert_eq!(IPAddress::to_string_vec(&IPAddress::summarize(&vec![ip4, ip3, ip2, ip1])),
                       ["10.0.0.0/22"]);
        }

        // Summarize non homogeneous networks
        ip1 = IPAddress::parse("10.0.0.0/23").unwrap();
        ip2 = IPAddress::parse("10.0.2.0/24").unwrap();
        assert_eq!(IPAddress::to_string_vec(&IPAddress::summarize(&vec![ip1, ip2])),
                   ["10.0.0.0/23", "10.0.2.0/24"]);

        ip1 = IPAddress::parse("10.0.0.0/16").unwrap();
        ip2 = IPAddress::parse("10.0.2.0/24").unwrap();
        assert_eq!(IPAddress::to_string_vec(&IPAddress::summarize(&vec![ip1, ip2])),
                   ["10.0.0.0/16"]);

        ip1 = IPAddress::parse("10.0.0.0/23").unwrap();
        ip2 = IPAddress::parse("10.1.0.0/24").unwrap();
        assert_eq!(IPAddress::to_string_vec(&IPAddress::summarize(&vec![ip1, ip2])),
                   ["10.0.0.0/23", "10.1.0.0/24"]);

        ip1 = IPAddress::parse("10.0.0.0/23").unwrap();
        ip2 = IPAddress::parse("10.0.2.0/23").unwrap();
        let mut ip3 = IPAddress::parse("10.0.4.0/24").unwrap();
        let mut ip4 = IPAddress::parse("10.0.6.0/24").unwrap();
        assert_eq!(IPAddress::to_string_vec(&IPAddress::summarize(&vec![ip1, ip2, ip3, ip4])),
                   ["10.0.0.0/22", "10.0.4.0/24", "10.0.6.0/24"]);
        {
            let ip1 = IPAddress::parse("10.0.1.1/24").unwrap();
            let ip2 = IPAddress::parse("10.0.2.1/24").unwrap();
            let ip3 = IPAddress::parse("10.0.3.1/24").unwrap();
            let ip4 = IPAddress::parse("10.0.4.1/24").unwrap();
            assert_eq!(IPAddress::to_string_vec(&IPAddress::summarize(&vec![ip1, ip2, ip3, ip4])),
                       ["10.0.1.0/24", "10.0.2.0/23", "10.0.4.0/24"]);
        }
        {
            let ip1 = IPAddress::parse("10.0.1.1/24").unwrap();
            let ip2 = IPAddress::parse("10.0.2.1/24").unwrap();
            let ip3 = IPAddress::parse("10.0.3.1/24").unwrap();
            let ip4 = IPAddress::parse("10.0.4.1/24").unwrap();
            assert_eq!(IPAddress::to_string_vec(&IPAddress::summarize(&vec![ip4, ip3, ip2, ip1])),
                       ["10.0.1.0/24", "10.0.2.0/23", "10.0.4.0/24"]);
        }

        ip1 = IPAddress::parse("10.0.1.1/24").unwrap();
        ip2 = IPAddress::parse("10.10.2.1/24").unwrap();
        ip3 = IPAddress::parse("172.16.0.1/24").unwrap();
        ip4 = IPAddress::parse("172.16.1.1/24").unwrap();
        assert_eq!(IPAddress::to_string_vec(&IPAddress::summarize(&vec![ip1, ip2, ip3, ip4])),
                   ["10.0.1.0/24", "10.10.2.0/24", "172.16.0.0/23"]);

        let mut ips = vec![IPAddress::parse("10.0.0.12/30").unwrap(),
                           IPAddress::parse("10.0.100.0/24").unwrap()];
        assert_eq!(IPAddress::to_string_vec(&IPAddress::summarize(&ips)),
                   ["10.0.0.12/30", "10.0.100.0/24"]);

        ips = vec![IPAddress::parse("172.16.0.0/31").unwrap(),
                   IPAddress::parse("10.10.2.1/32").unwrap()];
        assert_eq!(IPAddress::to_string_vec(&IPAddress::summarize(&ips)),
                   ["10.10.2.1/32", "172.16.0.0/31"]);

        ips = vec![IPAddress::parse("172.16.0.0/32").unwrap(),
                   IPAddress::parse("10.10.2.1/32").unwrap()];
        assert_eq!(IPAddress::to_string_vec(&IPAddress::summarize(&ips)),
                   ["10.10.2.1/32", "172.16.0.0/32"]);
    }

    #[test]
    pub fn test_classmethod_parse_classful() {
        for (ip, prefix) in setup().classful {
            let res = ipv4::parse_classful(ip).unwrap();
            assert_eq!(prefix, res.prefix.num);
            assert_eq!(format!("{}/{}", ip, prefix), res.to_string());
        }
        assert!(ipv4::parse_classful("192.168.256.257").is_err());
    }
}
