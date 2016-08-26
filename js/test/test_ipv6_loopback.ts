describe("ipv6_loopback", () => {
    class IPv6LoopbackTest {
        ip: IPAddress,
        s: string,
        n: string,
        string: string,
        one: Crunchy
    }
    function setup() : IPv6LoopbackTest {
        return IPv6LoopbackTest {
            ip: ipv6_loopback::new(),
            s: "::1",
            n: "::1/128",
            string: "0000:0000:0000:0000:0000:0000:0000:0001/128",
            one: BigUint::one(),
        };
    }

    it("test_attributes", () => {
        let s = setup();
        assert_eq!(128, s.ip.prefix.num);
        assert_eq!(true, s.ip.is_loopback());
        assert_eq!(s.s, s.ip.to_s());
        assert_eq!(s.n, s.ip.to_string());
        assert_eq!(s.string, s.ip.to_string_uncompressed());
        assert_eq!(s.one, s.ip.host_address);
    });
    it("test_method_ipv6", () => {
        assert_eq!(true, setup().ip.is_ipv6());
    });
});
