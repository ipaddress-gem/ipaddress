extern crate ipaddress;
extern crate num;

#[cfg(test)]
mod tests {
    use num::bigint::BigUint;
    use num::One;
    use ipaddress::ipv6_loopback;
    use ipaddress::IPAddress;

    struct IPv6LoopbackTest {
        ip: IPAddress,
        s: &'static str,
        n: &'static str,
        string: &'static str,
        one: BigUint,
    }
    #[allow(dead_code)]
    fn setup() -> IPv6LoopbackTest {
        return IPv6LoopbackTest {
            ip: ipv6_loopback::new(),
            s: "::1",
            n: "::1/128",
            string: "0000:0000:0000:0000:0000:0000:0000:0001/128",
            one: BigUint::one(),
        };
    }

    #[test]
    #[allow(dead_code)]
    fn test_attributes() {
        let s = setup();
        assert_eq!(128, s.ip.prefix.num);
        assert_eq!(true, s.ip.is_loopback());
        assert_eq!(s.s, s.ip.to_s());
        assert_eq!(s.n, s.ip.to_string());
        assert_eq!(s.string, s.ip.to_string_uncompressed());
        assert_eq!(s.one, s.ip.host_address);
    }
    #[test]
    #[allow(dead_code)]
    fn test_method_ipv6() {
        assert_eq!(true, setup().ip.is_ipv6());
    }
}
