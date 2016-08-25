
extern crate ipaddress;
extern crate num;

// use std::collections::HashMap;

#[cfg(test)]
mod tests {
    use ipaddress::prefix128;
    use std::collections::HashMap;
    use num::bigint::BigUint;


    pub struct Prefix128Test {
        pub u128_hash: HashMap<usize, BigUint>,
    }

    pub fn setup() -> Prefix128Test {
        let mut p128t = Prefix128Test { u128_hash: HashMap::new() };
        p128t.u128_hash.insert(32,
                               BigUint::parse_bytes(b"340282366841710300949110269838224261120",
                                                    10)
                                   .unwrap());
        p128t.u128_hash.insert(64,
                               BigUint::parse_bytes(b"340282366920938463444927863358058659840",
                                                    10)
                                   .unwrap());
        p128t.u128_hash.insert(96,
                               BigUint::parse_bytes(b"340282366920938463463374607427473244160",
                                                    10)
                                   .unwrap());
        p128t.u128_hash.insert(126,
                               BigUint::parse_bytes(b"340282366920938463463374607431768211452",
                                                    10)
                                   .unwrap());
        return p128t;
    }

    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_initialize() {
        assert!(prefix128::new(129).is_err());
        assert!(prefix128::new(64).is_ok());
    }

    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[allow(unused_variables)]
    #[test]
    pub fn test_method_bits() {
        let prefix = prefix128::new(64).unwrap();
        let mut str = String::new();
        for i in 0..64 {
            str.push_str("1");
        }
        for i in 0..64 {
            str.push_str("0");
        }
        assert_eq!(str, prefix.bits())
    }
    #[allow(dead_code)]
    #[allow(unused_attributes)]
    #[test]
    pub fn test_method_to_u32() {
        for (num, u128) in setup().u128_hash {
            assert_eq!(u128, prefix128::new(num).unwrap().netmask())
        }
    }
}
