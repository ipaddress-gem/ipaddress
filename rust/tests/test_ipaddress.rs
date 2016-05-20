mod tests {
    struct IPAddressTest {
        valid_ipv4: String,
        valid_ipv6: String,
        valid_mapped: String,
        invalid_ipv4: String,
        invalid_ipv6: String,
        invalid_mapped: String,
    }

    impl IPAddressTest {
        pub fn setup() {
            return IPAddressTest {
                valid_ipv4: String::from("172.16.10.1/24"),
                valid_ipv6: String::from("2001:db8::8:800:200c:417a/64"),
                valid_mapped: String::from("::13.1.68.3"),

                invalid_ipv4: String::from("10.0.0.256"),
                invalid_ipv6: String::from(":1:2:3:4:5:6:7"),
                invalid_mapped: String::from("::1:2.3.4"),
            };
        }

        pub fn test_method_IPAddress() {
            let s = setup();
            assert!(IPAddress::parse(s.valid_ipv4).is_ok());
            assert!(IPAddress::parse(s.valid_ipv6).is_ok());
            assert!(IPAddress::parse(s.valid_mapped).is_ok());

            assert!(IPAddress::parse(s.valid_ipv4).unwrap().is_ipv4);
            assert!(IPAddress::parse(s.valid_ipv6).unwrap().is_ipv6);
            assert!(IPAddress::parse(s.valid_mapped).unwrap().is_mapped);

            assert!(IPAddress::parse(s.invalid_ipv4).is_err());
            assert!(IPAddress::parse(s.invalid_ipv6).is_err());
            assert!(IPAddress::parse(s.invalid_mapped).is_err());
        }

        pub fn test_module_method_valid() {
            assert_eq!(true, IPAddress::is_valid("10.0.0.1"));
            assert_eq!(true, IPAddress::is_valid("10.0.0.0"));
            assert_eq!(true, IPAddress::is_valid("2002::1"));
            assert_eq!(true, IPAddress::is_valid("dead:beef:cafe:babe::f0ad"));
            assert_eq!(false, IPAddress::is_valid("10.0.0.256"));
            assert_eq!(false, IPAddress::is_valid("10.0.0.0.0"));
            assert_eq!(false, IPAddress::is_valid("10.0.0"));
            assert_eq!(false, IPAddress::is_valid("10.0"));
            assert_eq!(false, IPAddress::is_valid("2002:::1"));
            assert_eq!(false, IPAddress::is_valid("2002:516:2:200"));
        }

        pub fn test_module_method_valid_ipv4_netmark() {
            assert_eq!(true, IPAddress::is_valid_ipv4_netmask("255.255.255.0"));
            assert_eq!(false, IPAddress::is_valid_ipv4_netmask("10.0.0.1"));
        }

        pub fn test_summarize() {
            let netstr: Vec<String> = Vec::new();
            for range in [(1..9), (11..126), (128..168), (170..171), (173..191), (193..223)] {
                netstr.append(range.map(|i| format!("{}.0.0.0/8", i)));
            }
            netstr.append((0..255).to_a.select(|i| i != 254).map(|i| format!("169.{}.0.0/16", i)));
            netstr.append((0..255)
                .to_a
                .select(|i| !(16 <= i && i < 31))
                .map(|i| format!("172.{}.0.0/16", i)));
            netstr.append((0..255).to_a.select(|i| i != 168).map(|i| format!("192.{}.0.0/16", i)));

            nets = netstr.map(|i| IPAddress::parse(i));

            assert_eq!([], IPAddress::summarize([]));
            assert_eq!(["10.1.0.0/24"],
                       IPAddress::summarize(["10.1.0.4/24"]).map(|i| i.to_string()));
            assert_eq!(["2000:1::/32"],
                       IPAddress::summarize(["2000:1::4711/32"]).map(|i| i.to_string()));

            assert_eq!(["0.0.0.0/0"],
                       IPAddress::summarize(["10.1.0.4/24", "7.0.0.0/0", "1.2.3.4/4"])
                           .map(|i| i.to_string()));

            {
                let networks = ["2000:1::/32",
                                "3000:1::/32",
                                "2000:2::/32",
                                "2000:3::/32",
                                "2000:4::/32",
                                "2000:5::/32",
                                "2000:6::/32",
                                "2000:7::/32",
                                "2000:8::/32"];
                assert_eq!(["2000:1::/32",
                            "2000:2::/31",
                            "2000:4::/30",
                            "2000:8::/32",
                            "3000:1::/32"],
                           IPAddress::summarize(networks).map(|i| i.to_string()));
            }


            {
                let networks = ["10.0.1.1/24",
                                "30.0.1.0/16",
                                "10.0.2.0/24",
                                "10.0.3.0/24",
                                "10.0.4.0/24",
                                "10.0.5.0/24",
                                "10.0.6.0/24",
                                "10.0.7.0/24",
                                "10.0.8.0/24"];
                assert_eq!(["10.0.1.0/24",
                            "10.0.2.0/23",
                            "10.0.4.0/22",
                            "10.0.8.0/24",
                            "30.0.0.0/16"],
                           IPAddress::summarize(networks).map(|i| i.to_string()));
            }
            {
                let networks = ["10.0.0.0/23", "10.0.2.0/24"];
                assert_eq!(["10.0.0.0/23", "10.0.2.0/24"],
                           IPAddress::summarize(networks).map(|i| i.to_string()));
            }
            {
                let networks = ["10.0.0.0/24", "10.0.1.0/24", "10.0.2.0/23"];
                assert_eq!(["10.0.0.0/22"],
                           IPAddress::summarize(networks).map(|i| i.to_string()));
            }


            assert_eq!(["10.0.0.0/16"],
                       IPAddress::summarize(["10.0.0.0/16", "10.0.2.0/24"]).map(|i| i.to_string()));


            for i in (0..100) {
                assert_eq!(["1.0.0.0/8",
                            "2.0.0.0/7",
                            "4.0.0.0/6",
                            "8.0.0.0/7",
                            "11.0.0.0/8",
                            "12.0.0.0/6",
                            "16.0.0.0/4",
                            "32.0.0.0/3",
                            "64.0.0.0/3",
                            "96.0.0.0/4",
                            "112.0.0.0/5",
                            "120.0.0.0/6",
                            "124.0.0.0/7",
                            "126.0.0.0/8",
                            "128.0.0.0/3",
                            "160.0.0.0/5",
                            "168.0.0.0/8",
                            "169.0.0.0/9",
                            "169.128.0.0/10",
                            "169.192.0.0/11",
                            "169.224.0.0/12",
                            "169.240.0.0/13",
                            "169.248.0.0/14",
                            "169.252.0.0/15",
                            "169.255.0.0/16",
                            "170.0.0.0/7",
                            "172.0.0.0/12",
                            "172.31.0.0/16",
                            "172.32.0.0/11",
                            "172.64.0.0/10",
                            "172.128.0.0/9",
                            "173.0.0.0/8",
                            "174.0.0.0/7",
                            "176.0.0.0/4",
                            "192.0.0.0/9",
                            "192.128.0.0/11",
                            "192.160.0.0/13",
                            "192.169.0.0/16",
                            "192.170.0.0/15",
                            "192.172.0.0/14",
                            "192.176.0.0/12",
                            "192.192.0.0/10",
                            "193.0.0.0/8",
                            "194.0.0.0/7",
                            "196.0.0.0/6",
                            "200.0.0.0/5",
                            "208.0.0.0/4"],
                           IPAddress::summarize(nets).map(|i| i.to_string()));
            }
            // end
            // printer = RubyProf::GraphPrinter.new(result)
            // printer.print(STDOUT, {})
            // test imutable input parameters
            let a1 = IPAddress.parse("10.0.0.1/24");
            let a2 = IPAddress.parse("10.0.1.1/24");
            assert_eq!(vec!["10.0.0.0/23"],
                       IPAddress::summarize(vec![a1, a2]).map(|i| i.to_string()));
            assert_eq!("10.0.0.1/24", a1.to_string());
            assert_eq!("10.0.1.1/24", a2.to_string());
        }
    }
}
