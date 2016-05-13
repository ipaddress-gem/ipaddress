extern crate ipaddress;
extern crate num;


#[cfg(test)]
mod tests {
    use ipaddress::Prefix32;
    use std::collections::HashMap;
    use num::bigint::BigInt;

    struct Prefix32Test {
        netmask0: String,
        netmask8: String,
        netmask16: String,
        netmask24: String,
        netmask30: String,
        netmasks: Vec<String>,
        prefix_hash: HashMap<String, u8>,
        octets_hash: HashMap<Vec<u8>, u8>,
        u32_hash: HashMap<u8, u32>,
    }

    impl Prefix32Test {
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
            p32t.netmasks.push(p32t.netmask0);
            p32t.netmasks.push(p32t.netmask8);
            p32t.netmasks.push(p32t.netmask16);
            p32t.netmasks.push(p32t.netmask24);
            p32t.netmasks.push(p32t.netmask30);
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

        #[test]
        pub fn test_attributes() {
            for num in Prefix32Test::setup().prefix_hash.values() {
                let prefix = Prefix32::new(num);
                assert_eq!(num, prefix.num)
            }
        }

        #[test]
        pub fn test_parse_netmask() {
            for (netmask, num) in Prefix32Test::setup().prefix_hash {
                let prefix = Prefix32::parse_netmask(netmask);
                assert_eq!(num, prefix.prefix);
                match *prefix {
                    Prefix32 => {}
                    _ => assert!(false),
                }
            }
        }
        #[test]
        pub fn test_method_to_ip() {
            for (netmask, num) in Prefix32Test::setup().prefix_hash {
                let prefix = Prefix32::new(num);
                assert_eq!(netmask, prefix.to_ip())
            }
        }
        #[test]
        pub fn test_method_to_s() {
            let prefix = Prefix32::new(8);
            assert_eq!("8", prefix.to_s())
        }
        #[test]
        pub fn test_method_bits() {
            let prefix = Prefix32::new(16);
            assert_eq!("11111111111111110000000000000000", prefix.bits())
        }
        #[test]
        pub fn test_method_to_u32() {
            for (num, ip32) in Prefix32Test::setup().u32_hash {
                assert_eq!(ip32, Prefix32::new(num).to_u32())
            }
        }
        #[test]
        pub fn test_method_plus() {
            let p1 = Prefix32::new(8);
            let p2 = Prefix32::new(10);
            assert_eq!(18, p1 + p2);
            assert_eq!(12, p1 + 4)
        }
        #[test]
        pub fn test_method_minus() {
            let p1 = Prefix32::new(8);
            let p2 = Prefix32::new(24);
            assert_eq!(16, p1 - p2);
            assert_eq!(16, p2 - p1);
            assert_eq!(20, p2 - 4);
        }
        #[test]
        pub fn test_initialize() {
            assert!(Prefix32::new(33).is());
            assert!(!Prefix32::new(8).is());
            match *Prefix32::new(8) {
                Prefix32 => {}
                _ => assert!(false),
            }
        }

        #[test]
        pub fn test_method_octets() {
            for (arr, pref) in Prefix32Test::setup().octets_hash {
                let prefix = Prefix32::new(pref);
                assert_eq!(prefix.octets, arr);
            }
        }

        #[test]
        pub fn test_method_brackets() {
            for (arr, pref) in Prefix32Test::setup().octets_hash {
                let prefix = Prefix32::new(pref);
                for index in (0..arr.len()) {
                    let oct = arr.get(index);
                    assert_eq!(prefix.get(index), oct)
                }
            }
        }

        #[test]
        pub fn test_method_hostmask() {
            let prefix = Prefix32::new(8);
            assert_eq!("0.255.255.255", prefix.hostmask())
        }
    }
}
