extern crate ipaddress;
extern crate num;

#[cfg(test)]
mod tests {
    use ipaddress::IPAddress;
    use ipaddress::ipv6_unspec;
    use num::bigint::BigUint;
    use num::Zero;

    pub struct IPv6UnspecifiedTest {
        pub ip: IPAddress,
        pub to_s: String,
        pub to_string: String,
        pub to_string_uncompressed: String,
        pub num: BigUint,
    }

    fn setup() -> IPv6UnspecifiedTest {
        return IPv6UnspecifiedTest {
            ip: ipv6_unspec::new(),
            to_s: String::from("::"),
            to_string: String::from("::/128"),
            to_string_uncompressed: String::from("0000:0000:0000:0000:0000:0000:0000:0000/128"),
            num: BigUint::zero(),
        };
    }

    #[test]
    pub fn test_attributes() {
        assert_eq!(setup().ip.host_address, setup().num);
        assert_eq!(128, setup().ip.prefix().get_prefix());
        assert_eq!(true, setup().ip.is_unspecified());
        assert_eq!(setup().to_s, setup().ip.to_s());
        assert_eq!(setup().to_string, setup().ip.to_string());
        assert_eq!(setup().to_string_uncompressed,
                   setup().ip.to_string_uncompressed());
    }
    #[test]
    pub fn test_method_ipv6() {
        assert_eq!(true, setup().ip.is_ipv6());
    }
}
