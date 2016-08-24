extern crate ipaddress;
extern crate num;
extern crate num_traits;


#[cfg(test)]
mod tests {
    use ipaddress::IPAddress;
    use ipaddress::prefix32;
    use ipaddress::ipv4;
    use std::collections::HashMap;
    // use num::bigint::BigUint;
    use num_traits::cast::ToPrimitive;


    #[allow(dead_code)]
    pub struct Prefix32Test {
        netmask0: String,
        netmask8: String,
        netmask16: String,
        netmask24: String,
        netmask30: String,
        netmasks: Vec<String>,
        prefix_hash: HashMap<String, usize>,
        octets_hash: HashMap<Vec<u16>, usize>,
        u32_hash: HashMap<usize, u32>,
    }

    pub fn setup() -> Prefix32Test {
        let mut p32t = Prefix32Test {
            netmask0: String::from("0.0.0.0"),
            netmask8: String::from("255.0.0.0"),
            netmask16: String::from("255.255.0.0"),
            netmask24: String::from("255.255.255.0"),
            netmask30: String::from("255.255.255.252"),
            netmasks: Vec::new(),
            prefix_hash: HashMap::new(),
            octets_hash: HashMap::new(),
            u32_hash: HashMap::new(),
        };
        p32t.netmasks.push(p32t.netmask0.clone());
        p32t.netmasks.push(p32t.netmask8.clone());
        p32t.netmasks.push(p32t.netmask16.clone());
        p32t.netmasks.push(p32t.netmask24.clone());
        p32t.netmasks.push(p32t.netmask30.clone());
        p32t.prefix_hash.insert(String::from("0.0.0.0"), 0);
        p32t.prefix_hash.insert(String::from("255.0.0.0"), 8);
        p32t.prefix_hash.insert(String::from("255.255.0.0"), 16);
        p32t.prefix_hash.insert(String::from("255.255.255.0"), 24);
        p32t.prefix_hash.insert(String::from("255.255.255.252"), 30);
        p32t.octets_hash.insert(vec![0, 0, 0, 0], 0);
        p32t.octets_hash.insert(vec![255, 0, 0, 0], 8);
        p32t.octets_hash.insert(vec![255, 255, 0, 0], 16);
        p32t.octets_hash.insert(vec![255, 255, 255, 0], 24);
        p32t.octets_hash.insert(vec![255, 255, 255, 252], 30);
        p32t.u32_hash.insert(0, 0);
        p32t.u32_hash.insert(8, 4278190080);
        p32t.u32_hash.insert(16, 4294901760);
        p32t.u32_hash.insert(24, 4294967040);
        p32t.u32_hash.insert(30, 4294967292);
        return p32t;
    }

    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_attributes() {
        for num in setup().prefix_hash.values() {
            let prefix = prefix32::new(*num).unwrap();
            assert_eq!(*num, prefix.num)
        }
    }

    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_parse_netmask_to_prefix() {
        for (netmask, num) in setup().prefix_hash {
            let prefix = IPAddress::parse_netmask_to_prefix(netmask).unwrap();
            assert_eq!(num, prefix);
        }
    }
    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_method_to_ip() {
        for (netmask, num) in setup().prefix_hash {
            let prefix = prefix32::new(num).unwrap();
            assert_eq!(*netmask, prefix.to_ip_str())
        }
    }
    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_method_to_s() {
        let prefix = prefix32::new(8).unwrap();
        assert_eq!("8", prefix.to_s())
    }
    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_method_bits() {
        let prefix = prefix32::new(16).unwrap();
        assert_eq!("11111111111111110000000000000000", prefix.bits())
    }
    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_method_to_u32() {
        for (num, ip32) in setup().u32_hash {
            assert_eq!(ip32,
                       prefix32::new(num).unwrap().netmask().to_u32().unwrap())
        }
    }
    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_method_plus() {
        let p1 = prefix32::new(8).unwrap();
        let p2 = prefix32::new(10).unwrap();
        assert_eq!(18, p1.add_prefix(&p2).unwrap().num);
        assert_eq!(12, p1.add(4).unwrap().num)
    }
    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_method_minus() {
        let p1 = prefix32::new(8).unwrap();
        let p2 = prefix32::new(24).unwrap();
        assert_eq!(16, p1.sub_prefix(&p2).unwrap().num);
        assert_eq!(16, p2.sub_prefix(&p1).unwrap().num);
        assert_eq!(20, p2.sub(4).unwrap().num);
    }
    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_initialize() {
        assert!(prefix32::new(33).is_err());
        assert!(prefix32::new(8).is_ok());
    }
    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_method_octets() {
        for (arr, pref) in setup().octets_hash {
            let prefix = prefix32::new(pref).unwrap();
            assert_eq!(prefix.ip_bits.parts(&prefix.netmask()), arr);
        }
    }
    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_method_brackets() {
        for (arr, pref) in setup().octets_hash {
            let prefix = prefix32::new(pref).unwrap();
            for index in 0..arr.len() {
                let oct = arr.get(index);
                assert_eq!(prefix.ip_bits.parts(&prefix.netmask()).get(index), oct)
            }
        }
    }
    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_method_hostmask() {
        let prefix = prefix32::new(8).unwrap();
        assert_eq!("0.255.255.255",
                   ipv4::from_u32(prefix.host_mask().to_u32().unwrap(), 0).unwrap().to_s());
    }
}
