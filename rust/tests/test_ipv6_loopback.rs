mod tests {
    struct IPv6LoopbackTest {
        ip: IPAddress,
        s: String,
        n: String,
        string: String,
        n: BigUint,
    }

    impl IPv6LoopbackTest {
        fn setup() {
            return IPv6LoopbackTest {
                ip: IPAddress::IPv6::loopback(),
                s: "::1",
                n: "::1/128",
                string: "0000:0000:0000:0000:0000:0000:0000:0001/128",
                n: BigUint.one(),
            };
        }

        fn test_attributes() {
            let s = setup();
            assert_eq!(s.address, s.ip.compressed());
            assert_eq!(128, s.ip.prefix.num);
            assert_eq!(true, s.ip.is_loopback());
            assert_eq!(s.s, s.ip.to_s());
            assert_eq!(s.n, s.ip.to_string());
            assert_eq!(s.string, s.ip.to_string_uncompressed());
            assert_eq!(s.n, s.ip.address());
        }

        fn test_method_ipv6() {
            assert_eq!(true, setup().ip.is_ipv6());
        }
    }
}
