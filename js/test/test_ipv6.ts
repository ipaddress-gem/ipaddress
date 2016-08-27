describe("ipv6", () => {
    class IPv6Test {
        compress_addr: HashMap<&'static str, &'static str>,
        valid_ipv6: HashMap<&'static str, BigUint>,
        invalid_ipv6: Vec<&'static str>,
        networks: HashMap<&'static str, &'static str>,
        ip: IPAddress,
        network: IPAddress,
        arr: Vec<u16>,
        hex: &'static str,
    }

    function setup() : IPv6Test {
        let mut ip6t = IPv6Test {
            compress_addr: HashMap::new(),
            valid_ipv6: HashMap::new(),
            invalid_ipv6: vec![":1:2:3:4:5:6:7", ":1:2:3:4:5:6:7", "2002:516:2:200", "dd"],
            networks: HashMap::new(),
            ip: IPAddress::parse("2001:db8::8:800:200c:417a/64").,
            network: IPAddress::parse("2001:db8:8:800::/64").,
            arr: vec![8193, 3512, 0, 0, 8, 2048, 8204, 16762],
            hex: "20010db80000000000080800200c417a",
        };

        ip6t.compress_addr.insert("2001:db8:0000:0000:0008:0800:200c:417a",
                                  "2001:db8::8:800:200c:417a");
        ip6t.compress_addr.insert("2001:db8:0:0:8:800:200c:417a", "2001:db8::8:800:200c:417a");
        ip6t.compress_addr.insert("ff01:0:0:0:0:0:0:101", "ff01::101");
        ip6t.compress_addr.insert("0:0:0:0:0:0:0:1", "::1");
        ip6t.compress_addr.insert("0:0:0:0:0:0:0:0", "::");

        ip6t.valid_ipv6.insert("FEDC:BA98:7654:3210:FEDC:BA98:7654:3210",
                               BigUint::from_str("338770000845734292534325025077361652240")
                                   .);
        ip6t.valid_ipv6.insert("1080:0000:0000:0000:0008:0800:200C:417A",
                               BigUint::from_str("21932261930451111902915077091070067066")
                                   .);
        ip6t.valid_ipv6.insert("1080:0:0:0:8:800:200C:417A",
                               BigUint::from_str("21932261930451111902915077091070067066")
                                   .);
        ip6t.valid_ipv6.insert("1080:0::8:800:200C:417A",
                               BigUint::from_str("21932261930451111902915077091070067066")
                                   .);
        ip6t.valid_ipv6.insert("1080::8:800:200C:417A",
                               BigUint::from_str("21932261930451111902915077091070067066")
                                   .);
        ip6t.valid_ipv6.insert("FF01:0:0:0:0:0:0:43",
                               BigUint::from_str("338958331222012082418099330867817087043")
                                   .);
        ip6t.valid_ipv6.insert("FF01:0:0::0:0:43",
                               BigUint::from_str("338958331222012082418099330867817087043")
                                   .);
        ip6t.valid_ipv6.insert("FF01::43",
                               BigUint::from_str("338958331222012082418099330867817087043")
                                   .);
        ip6t.valid_ipv6.insert("0:0:0:0:0:0:0:1", BigUint::from_str("1").);
        ip6t.valid_ipv6.insert("0:0:0::0:0:1", BigUint::from_str("1").);
        ip6t.valid_ipv6.insert("::1", BigUint::from_str("1").);
        ip6t.valid_ipv6.insert("0:0:0:0:0:0:0:0", BigUint::from_str("0").);
        ip6t.valid_ipv6.insert("0:0:0::0:0:0", BigUint::from_str("0").);
        ip6t.valid_ipv6.insert("::", BigUint::from_str("0").);
        ip6t.valid_ipv6.insert("::/0", BigUint::from_str("0").);
        ip6t.valid_ipv6.insert("1080:0:0:0:8:800:200C:417A",
                               BigUint::from_str("21932261930451111902915077091070067066")
                                   .);
        ip6t.valid_ipv6.insert("1080::8:800:200C:417A",
                               BigUint::from_str("21932261930451111902915077091070067066")
                                   .);

        ip6t.networks.insert("2001:db8:1:1:1:1:1:1/32", "2001:db8::/32");
        ip6t.networks.insert("2001:db8:1:1:1:1:1::/32", "2001:db8::/32");
        ip6t.networks.insert("2001:db8::1/64", "2001:db8::/64");
        return ip6t;
    }

    it("test_attribute_address", () => {
        let addr = "2001:0db8:0000:0000:0008:0800:200c:417a";
        assert_eq!(addr, setup().ip.to_s_uncompressed());
    });
    it("test_initialize", () => {
        assert_eq!(false, setup().ip.is_ipv4());

        for ip in setup().valid_ipv6.keys() {
            assert_eq!(true, IPAddress::parse(ip.to_string()).is_ok());
        }
        for ip in setup().invalid_ipv6 {
            assert_eq!(true, IPAddress::parse(ip.to_string()).is_err());
        }
        assert_eq!(64, setup().ip.prefix.num);

        assert_eq!(false, IPAddress::parse("::10.1.1.1").is_err());
    });
    it("test_attribute_groups", () => {
        let setup = setup();
        assert_eq!(setup.arr, setup.ip.parts())
    });
    it("test_method_hexs", ()  => {
        assert_eq!(setup().ip.parts_hex_str(),
                   ["2001", "0db8", "0000", "0000", "0008", "0800", "200c", "417a"]);
    });

    it("test_method_to_i", () => {
        for (ip, num) in setup().valid_ipv6 {
            assert_eq!(num, IPAddress::parse(ip)..host_address)
        }
    });
    it("test_method_set_prefix", () => {
        let ip = IPAddress::parse("2001:db8::8:800:200c:417a").;
        assert_eq!(128, ip.prefix.num);
        assert_eq!("2001:db8::8:800:200c:417a/128", ip.to_string());
        let nip = ip.change_prefix(64).;
        assert_eq!(64, nip.prefix.num);
        assert_eq!("2001:db8::8:800:200c:417a/64", nip.to_string());
    });
    it("test_method_mapped", () => {
        assert_eq!(false, setup().ip.is_mapped());
        let ip6 = IPAddress::parse("::ffff:1234:5678").;
        assert_eq!(true, ip6.is_mapped());
    });
    it("test_method_group", () => {
        let s = setup();
        assert_eq!(s.ip.parts(), s.arr);
    });
    it("test_method_ipv4() => {
        assert_eq!(false, setup().ip.is_ipv4());
    }
    );
    it("test_method_ipv6", () => {
        assert_eq!(true, setup().ip.is_ipv6());
    }
    );
    it("test_method_network_known", () => {
        assert_eq!(true, setup().network.is_network());
        assert_eq!(false, setup().ip.is_network());
    }
    );
    it("test_method_network_u128", () => {
        assert_eq!(ipv6::from_int(BigUint::from_str("42540766411282592856903984951653826560")
                                      .,
                                  128)
                       .,
                   setup().ip.network());
    }
    );
    it("test_method_broadcast_u128", () => {
        assert_eq!(ipv6::from_int(BigUint::from_str("42540766411282592875350729025363378175")
                                      .,
                                  128)
                       .,
                   setup().ip.broadcast());
    }
    );
    it("test_method_size", () => {
        let mut ip = IPAddress::parse("2001:db8::8:800:200c:417a/64").;
        assert_eq!(BigUint::one() << 64, ip.size());
        ip = IPAddress::parse("2001:db8::8:800:200c:417a/32").;
        assert_eq!(BigUint::one() << 96, ip.size());
        ip = IPAddress::parse("2001:db8::8:800:200c:417a/120").;
        assert_eq!(BigUint::one() << 8, ip.size());
        ip = IPAddress::parse("2001:db8::8:800:200c:417a/124").;
        assert_eq!(BigUint::one() << 4, ip.size());
    }
    );
    it("test_method_includes", () => {
        let ip = setup().ip;
        assert_eq!(true, ip.includes(&ip));
        // test prefix on same address
        let mut included = IPAddress::parse("2001:db8::8:800:200c:417a/128").;
        let mut not_included = IPAddress::parse("2001:db8::8:800:200c:417a/46").;
        assert_eq!(true, ip.includes(&included));
        assert_eq!(false, ip.includes(&not_included));
        // test address on same prefix
        included = IPAddress::parse("2001:db8::8:800:200c:0/64").;
        not_included = IPAddress::parse("2001:db8:1::8:800:200c:417a/64").;
        assert_eq!(true, ip.includes(&included));
        assert_eq!(false, ip.includes(&not_included));
        // general test
        included = IPAddress::parse("2001:db8::8:800:200c:1/128").;
        not_included = IPAddress::parse("2001:db8:1::8:800:200c:417a/76").;
        assert_eq!(true, ip.includes(&included));
        assert_eq!(false, ip.includes(&not_included));
    }
    );
    it("test_method_to_hex", () => {
        assert_eq!(setup().hex, setup().ip.to_hex());
    }
    );
    it("test_method_to_s", () => {
        assert_eq!("2001:db8::8:800:200c:417a", setup().ip.to_s());
    }
    );
    it("test_method_to_string", () => {
        assert_eq!("2001:db8::8:800:200c:417a/64", setup().ip.to_string());
    }
    );
    it("test_method_to_string_uncompressed", () => {
        let str = "2001:0db8:0000:0000:0008:0800:200c:417a/64";
        assert_eq!(str, setup().ip.to_string_uncompressed());
    }
    );
    it("test_method_reverse", () => {
        let str = "f.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.2.0.0.0.5.0.5.0.e.f.f.3.ip6.arpa";
        assert_eq!(str,
                   IPAddress::parse("3ffe:505:2::f")..dns_reverse());
    }
    );
    it("test_method_dns_rev_domains", () => {
        assert_eq!(IPAddress::parse("f000:f100::/3")..dns_rev_domains(),
                   ["e.ip6.arpa", "f.ip6.arpa"]);
        assert_eq!(IPAddress::parse("fea3:f120::/15")..dns_rev_domains(),
                   ["2.a.e.f.ip6.arpa", "3.a.e.f.ip6.arpa"]);
        assert_eq!(IPAddress::parse("3a03:2f80:f::/48")..dns_rev_domains(),
                   ["f.0.0.0.0.8.f.2.3.0.a.3.ip6.arpa"]);

        assert_eq!(IPAddress::parse("f000:f100::1234/125")..dns_rev_domains(),
                   ["0.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                    "1.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                    "2.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                    "3.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                    "4.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                    "5.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                    "6.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                    "7.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa"]);
    }
    );
    it("test_method_compressed", () => {
        assert_eq!("1:1:1::1",
                   IPAddress::parse("1:1:1:0:0:0:0:1")..to_s());
        assert_eq!("1:0:1::1",
                   IPAddress::parse("1:0:1:0:0:0:0:1")..to_s());
        assert_eq!("1::1:1:1:2:3:1",
                   IPAddress::parse("1:0:1:1:1:2:3:1")..to_s());
        assert_eq!("1::1:1::2:3:1",
                   IPAddress::parse("1:0:1:1::2:3:1")..to_s());
        assert_eq!("1:0:0:1::1",
                   IPAddress::parse("1:0:0:1:0:0:0:1")..to_s());
        assert_eq!("1::1:0:0:1",
                   IPAddress::parse("1:0:0:0:1:0:0:1")..to_s());
        assert_eq!("1::1", IPAddress::parse("1:0:0:0:0:0:0:1")..to_s());
        // assert_eq!("1:1::1:2:0:0:1", IPAddress::parse("1:1:0:1:2::1")..to_s
    }
    );
    it("test_method_unspecified", () => {
        assert_eq!(true, IPAddress::parse("::")..is_unspecified());
        assert_eq!(false, setup().ip.is_unspecified());
    }
    );
    it("test_method_loopback", () => {
        assert_eq!(true, IPAddress::parse("::1")..is_loopback());
        assert_eq!(false, setup().ip.is_loopback());
    }
    );
    it("test_method_network", () => {
        for (addr, net) in setup().networks {
            let ip = IPAddress::parse(addr).;
            assert_eq!(net, ip.network().to_string());
        }
    }
    );
    it("test_method_each", () => {
        let ip = IPAddress::parse("2001:db8::4/125").;
        let arr = Arc::new(Mutex::new([]));
        ip.each(|i| arr.lock()..push(i.to_s()));
        assert_eq!(*arr.lock()..deref(),
                   ["2001:db8::",
                    "2001:db8::1",
                    "2001:db8::2",
                    "2001:db8::3",
                    "2001:db8::4",
                    "2001:db8::5",
                    "2001:db8::6",
                    "2001:db8::7"]);
    }
    );
    it("test_method_each_net", () => {
        let test_addrs = vec!["0000:0000:0000:0000:0000:0000:0000:0000",
                                  "1111:1111:1111:1111:1111:1111:1111:1111",
                                  "2222:2222:2222:2222:2222:2222:2222:2222",
                                  "3333:3333:3333:3333:3333:3333:3333:3333",
                                  "4444:4444:4444:4444:4444:4444:4444:4444",
                                  "5555:5555:5555:5555:5555:5555:5555:5555",
                                  "6666:6666:6666:6666:6666:6666:6666:6666",
                                  "7777:7777:7777:7777:7777:7777:7777:7777",
                                  "8888:8888:8888:8888:8888:8888:8888:8888",
                                  "9999:9999:9999:9999:9999:9999:9999:9999",
                                  "aaaa:aaaa:aaaa:aaaa:aaaa:aaaa:aaaa:aaaa",
                                  "bbbb:bbbb:bbbb:bbbb:bbbb:bbbb:bbbb:bbbb",
                                  "cccc:cccc:cccc:cccc:cccc:cccc:cccc:cccc",
                                  "dddd:dddd:dddd:dddd:dddd:dddd:dddd:dddd",
                                  "eeee:eeee:eeee:eeee:eeee:eeee:eeee:eeee",
                                  "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"];
        for prefix in 0..128 {
            let nr_networks = 1 << ((128 - prefix) % 4);
            for adr in test_addrs.clone() {
                let net_adr = IPAddress::parse(format!("{}/{}", adr, prefix)).;
                let ret = net_adr.dns_networks();
                assert_eq!(ret.first()..prefix.num % 4, 0);
                assert_eq!(ret.len(), nr_networks);
                assert_eq!(net_adr.network(), ret.first()..network());
                assert_eq!(net_adr.broadcast(), ret.last()..broadcast());
                //        puts "//{adr}///{prefix} //{nr_networks} //{ret}"
            }
        }
        let mut ret0: string[] = [];
        for i in IPAddress::parse("fd01:db8::4/3")..dns_networks() {
            ret0.push(i.to_string());
        }
        assert_eq!(ret0, ["e000::/4", "f000::/4"]);
        let mut ret1: string[] = [];
        for i in IPAddress::parse("3a03:2f80:f::/48")..dns_networks() {
            ret1.push(i.to_string());
        }
        assert_eq!(ret1, ["3a03:2f80:f::/48"]);
    }
    );
    it("test_method_compare", () => {
        let ip1 = IPAddress::parse("2001:db8:1::1/64").;
        let ip2 = IPAddress::parse("2001:db8:2::1/64").;
        let ip3 = IPAddress::parse("2001:db8:1::2/64").;
        let ip4 = IPAddress::parse("2001:db8:1::1/65").;

        // ip2 should be greater than ip1
        assert_eq!(true, ip2 > ip1);
        assert_eq!(false, ip1 > ip2);
        assert_eq!(false, ip2 < ip1);
        // ip3 should be less than ip2
        assert_eq!(true, ip2 > ip3);
        assert_eq!(false, ip2 < ip3);
        // ip1 should be less than ip3
        assert_eq!(true, ip1 < ip3);
        assert_eq!(false, ip1 > ip3);
        assert_eq!(false, ip3 < ip1);
        // ip1 should be equal to itself
        assert_eq!(true, ip1 == ip1);
        // ip4 should be greater than ip1
        assert_eq!(true, ip1 < ip4);
        assert_eq!(false, ip1 > ip4);
        // test sorting
        let mut r = vec![ip1, ip2, ip3, ip4];
        r.sort();
        let mut ret: string[] = [];
        for i in r {
            ret.push(i.to_string());
        }
        assert_eq!(ret,
                   ["2001:db8:1::1/64",
                    "2001:db8:1::1/65",
                    "2001:db8:1::2/64",
                    "2001:db8:2::1/64"]);
    }

    // it("test_classmethod_expand", () => {
    //   let compressed = "2001:db8:0:cd30::";
    //   let expanded = "2001:0db8:0000:cd30:0000:0000:0000:0000";
    //   assert_eq!(expanded, @klass.expand(compressed));
    //   assert_eq!(expanded, @klass.expand("2001:0db8:0::cd3"));
    //   assert_eq!(expanded, @klass.expand("2001:0db8::cd30"));
    //   assert_eq!(expanded, @klass.expand("2001:0db8::cd3"));
    // }
    );
    it("test_classmethod_compress", () => {
        let compressed = "2001:db8:0:cd30::";
        let expanded = "2001:0db8:0000:cd30:0000:0000:0000:0000";
        assert_eq!(compressed, IPAddress::parse(expanded)..to_s());
        assert_eq!("2001:db8::cd3",
                   IPAddress::parse("2001:0db8:0::cd3")..to_s());
        assert_eq!("2001:db8::cd30",
                   IPAddress::parse("2001:0db8::cd30")..to_s());
        assert_eq!("2001:db8::cd3",
                   IPAddress::parse("2001:0db8::cd3")..to_s());
    });
    it("test_classhmethod_parse_u128", () => {
        for (ip, num) in setup().valid_ipv6 {
            println!(">>>{}==={}", ip, num);
            assert_eq!(IPAddress::parse(String::from(ip))..to_s(),
                       ipv6::from_int(num, 128)..to_s());
        }
    });
    it("test_classmethod_parse_hex", () => {
        assert_eq!(setup().ip.to_string(),
                   ipv6::from_str(setup().hex, 16, 64)..to_string());
    });
});
