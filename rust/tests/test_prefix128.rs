
extern crate ipaddress;
extern crate num;

// use std::collections::HashMap;

#[cfg(test)]
mod tests {
    use ipaddress::Prefix128;
    use std::collections::HashMap;
    use num::bigint::BigInt;


    struct Prefix128Test {
        u128_hash: HashMap<u8, BigInt>,
    }

    impl Prefix128Test {
        pub fn setup() {
            let mut p128t = Prefix128Test { u128_hash: HashMap::new() };
            p128t.u128_hash.insert(32,
                                   BigInt::parse_bytes(b"340282366841710300949110269838224261120",10).unwrap());
            p128t.u128_hash.insert(64,
                                   BigInt::parse_bytes(b"340282366920938463444927863358058659840",10).unwrap());
            p128t.u128_hash.insert(96,
                                   BigInt::parse_bytes(b"340282366920938463463374607427473244160",10).unwrap());
            p128t.u128_hash.insert(126,
                                   BigInt::parse_bytes(b"340282366920938463463374607431768211452",10).unwrap());
            return p128t;
        }

        #[test]
        pub fn test_initialize() {
            assert!(Prefix128::new(129).is());
            assert!(!Prefix128::new(64).is());
        }

        #[test]
        pub fn test_method_bits() {
            let prefix = Prefix128::new(64);
            let mut str = String::new();
            for i in (0..63) {
                str.push_str("1");
            }
            for i in (0..63) {
                str.push_str("0");
            }
            assert_eq!(str, prefix.bits)
        }
        #[test]
        pub fn test_method_to_u32() {
            for (num, u128) in Prefix128Test::setup().u128_hash {
                assert_eq!(u128, Prefix128::new(num).to_u128())
            }
        }
    }
}
