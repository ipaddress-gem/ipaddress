describe("ipv6_unspec", () => {
    class IPv6UnspecifiedTest {
        ip: IPAddress,
        to_s: String,
        to_string: String,
        to_string_uncompressed: String,
        num: BigUint,
    }

    function setup() : IPv6UnspecifiedTest {
        return IPv6UnspecifiedTest {
            ip: ipv6_unspec::new(),
            to_s: String::from("::"),
            to_string: String::from("::/128"),
            to_string_uncompressed: String::from("0000:0000:0000:0000:0000:0000:0000:0000/128"),
            num: BigUint::zero(),
        };
    }

    it("test_attributes", () => {
        assert_eq!(setup().ip.host_address, setup().num);
        assert_eq!(128, setup().ip.prefix().get_prefix());
        assert_eq!(true, setup().ip.is_unspecified());
        assert_eq!(setup().to_s, setup().ip.to_s());
        assert_eq!(setup().to_string, setup().ip.to_string());
        assert_eq!(setup().to_string_uncompressed,
                   setup().ip.to_string_uncompressed());
    });
    it("test_method_ipv6", () => {
        assert_eq!(true, setup().ip.is_ipv6());
    });
});
