
mod tests {
    struct IPv6UnspecifiedTest {
        ip: IPAddress,
        to_s: String,
        to_string: String,
        to_long_string: String,
        num: BigUint,
    }

    mod IPv6UnspecifiedTest {

        pub fn setup() {
            return IPv6UnspecifiedTest {
                ip: IPv6::unspecified(),
                to_s: String::from("::"),
                to_string: String::from("::/128"),
                to_string_uncompressed: String::from("0000:0000:0000:0000:0000:0000:0000:0000/128"),
                num: BigUint::zero(),
            };
        }
        pub fn test_attributes() {
            assert_eq!(setup().address, setup().ip.compressed());
            assert_eq!(128, setup().ip.prefix());
            assert_eq!(true, setup().ip.is_unspecified());
            assert_eq!(setup().to_s, setup().ip.to_s());
            assert_eq!(setup().to_string, setup().ip.to_string());
            assert_eq!(setup().to_string_uncompressed,
                       setup().ip.to_string_uncompressed());
            assert_eq!(setup().to_long_string, setup().ip.to_i());
        }

        pub fn test_method_ipv6() {
            assert_eq!(true, setup().ip.is_ipv6());
        }

    }
}
