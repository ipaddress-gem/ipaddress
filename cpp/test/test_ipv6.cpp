import { assert } from 'chai';
import * as Mocha from 'mocha';

import IPAddress from '../src/ipaddress';
import Prefix128 from '../src/prefix128';
import Ipv6 from '../src/ipv6';
import Crunchy from '../src/crunchy';


class IPv6Test {
    compress_addr: [string, string][] = [];
    valid_ipv6: [string, Crunchy][] = [];
    invalid_ipv6: string[] = [":1:2:3:4:5:6:7", ":1:2:3:4:5:6:7", "2002:516:2:200", "dd"];
    networks: [string, string][] = [];
    ip: IPAddress = IPAddress.parse("2001:db8::8:800:200c:417a/64");
    network: IPAddress = IPAddress.parse("2001:db8:8:800::/64");
    arr: number[] = [8193, 3512, 0, 0, 8, 2048, 8204, 16762];
    hex: string = "20010db80000000000080800200c417a";
}

describe("ipv6", () => {

    
    function assertArrayEqual(a: any[], b: any[]) : void {
        assert.equal(a.length, b.length, "length missmatch");
        for (let i = 0; i < a.length; ++i) {
            assert.equal(a[i], b[i]);
        }
    }


    function setup(): IPv6Test {
        let ip6t = new IPv6Test();
        ip6t.compress_addr.push(["2001:db8:0000:0000:0008:0800:200c:417a",
            "2001:db8::8:800:200c:417a"]);
        ip6t.compress_addr.push(["2001:db8:0:0:8:800:200c:417a", "2001:db8.8:800:200c:417a"]);
        ip6t.compress_addr.push(["ff01:0:0:0:0:0:0:101", "ff01::101"]);
        ip6t.compress_addr.push(["0:0:0:0:0:0:0:1", ".1"]);
        ip6t.compress_addr.push(["0:0:0:0:0:0:0:0", "."]);

        ip6t.valid_ipv6.push(["FEDC:BA98:7654:3210:FEDC:BA98:7654:3210",
            Crunchy.parse("338770000845734292534325025077361652240")
        ]);
        ip6t.valid_ipv6.push(["1080:0000:0000:0000:0008:0800:200C:417A",
            Crunchy.parse("21932261930451111902915077091070067066")
        ]);
        ip6t.valid_ipv6.push(["1080:0:0:0:8:800:200C:417A",
            Crunchy.parse("21932261930451111902915077091070067066")
        ]);
        ip6t.valid_ipv6.push(["1080:0::8:800:200C:417A",
            Crunchy.parse("21932261930451111902915077091070067066")
        ]);
        ip6t.valid_ipv6.push(["1080::8:800:200C:417A",
            Crunchy.parse("21932261930451111902915077091070067066")
        ]);
        ip6t.valid_ipv6.push(["FF01:0:0:0:0:0:0:43",
            Crunchy.parse("338958331222012082418099330867817087043")
        ]);
        ip6t.valid_ipv6.push(["FF01:0::0:0:43",
            Crunchy.parse("338958331222012082418099330867817087043")
        ]);
        ip6t.valid_ipv6.push(["FF01::43",
            Crunchy.parse("338958331222012082418099330867817087043")
        ]);
        ip6t.valid_ipv6.push(["0:0:0:0:0:0:0:1", Crunchy.parse("1")]);
        ip6t.valid_ipv6.push(["0:0:0::0:0:1", Crunchy.parse("1")]);
        ip6t.valid_ipv6.push(["::1", Crunchy.parse("1")]);
        ip6t.valid_ipv6.push(["0:0:0:0:0:0:0:0", Crunchy.parse("0")]);
        ip6t.valid_ipv6.push(["0:0:0::0:0:0", Crunchy.parse("0")]);
        ip6t.valid_ipv6.push(["::", Crunchy.parse("0")]);
        ip6t.valid_ipv6.push(["::/0", Crunchy.parse("0")]);
        ip6t.valid_ipv6.push(["1080:0:0:0:8:800:200C:417A",
            Crunchy.parse("21932261930451111902915077091070067066")
        ]);
        ip6t.valid_ipv6.push(["1080::8:800:200C:417A",
            Crunchy.parse("21932261930451111902915077091070067066")
        ]);

        ip6t.networks.push(["2001:db8:1:1:1:1:1:1/32", "2001:db8::/32"]);
        ip6t.networks.push(["2001:db8:1:1:1:1:1::/32", "2001:db8::/32"]);
        ip6t.networks.push(["2001:db8::1/64", "2001:db8::/64"]);
        return ip6t;
    }

    it("test_attribute_address", () => {
        let addr = "2001:0db8:0000:0000:0008:0800:200c:417a";
        assert.equal(addr, setup().ip.to_s_uncompressed());
    });
    it("test_initialize", () => {
        assert.equal(false, setup().ip.is_ipv4());

        for (let ip of setup().valid_ipv6) {
            assert.isOk(IPAddress.parse(ip[0]));
        }
        for (let ip of setup().invalid_ipv6) {
            assert.isNotOk(IPAddress.parse(ip));
        }
        assert.equal(64, setup().ip.prefix.num);

        assert.isOk(IPAddress.parse("::10.1.1.1"));
    });
    it("test_attribute_groups", () => {
        assertArrayEqual(setup().arr, setup().ip.parts())
    });
    it("test_method_hexs", () => {
        assertArrayEqual(setup().ip.parts_hex_str(),
            ["2001", "0db8", "0000", "0000", "0008", "0800", "200c", "417a"]);
    });

    it("test_method_to_i", () => {
        for (let i of setup().valid_ipv6) {
            let ip = i[0];
            let num = i[1];
            assert.isOk(num.eq(IPAddress.parse(ip).host_address));
        }
    });
    it("test_method_set_prefix", () => {
        let ip = IPAddress.parse("2001:db8::8:800:200c:417a");
        assert.equal(128, ip.prefix.num);
        assert.equal("2001:db8::8:800:200c:417a/128", ip.to_string());
        let nip = ip.change_prefix(64);
        assert.equal(64, nip.prefix.num);
        assert.equal("2001:db8::8:800:200c:417a/64", nip.to_string());
    });
    it("test_method_mapped", () => {
        assert.equal(false, setup().ip.is_mapped());
        let ip6 = IPAddress.parse("::ffff:1234:5678");
        assert.equal(true, ip6.is_mapped());
    });
    it("test_method_group", () => {
        let s = setup();
        assertArrayEqual(s.ip.parts(), s.arr);
    });
    it("test_method_ipv4()", () => {
        assert.equal(false, setup().ip.is_ipv4());
    });
    it("test_method_ipv6", () => {
        assert.equal(true, setup().ip.is_ipv6());
    });
    it("test_method_network_known", () => {
        assert.equal(true, setup().network.is_network());
        assert.equal(false, setup().ip.is_network());
    });
    it("test_method_network_u128", () => {
        assert.isOk(Ipv6.from_int(Crunchy.parse("42540766411282592856903984951653826560"), 64)
            .eq(setup().ip.network()));
    });
    it("test_method_broadcast_u128", () => {
        assert.isOk(Ipv6.from_int(Crunchy.parse("42540766411282592875350729025363378175"), 64)
            .eq(setup().ip.broadcast()));
    });
    it("test_method_size", () => {
        let ip = IPAddress.parse("2001:db8::8:800:200c:417a/64");
        assert.isOk(Crunchy.one().shl(64).eq(ip.size()));
        ip = IPAddress.parse("2001:db8::8:800:200c:417a/32");
        assert.isOk(Crunchy.one().shl(96).eq(ip.size()));
        ip = IPAddress.parse("2001:db8::8:800:200c:417a/120");
        assert.isOk(Crunchy.one().shl(8).eq(ip.size()));
        ip = IPAddress.parse("2001:db8::8:800:200c:417a/124");
        assert.isOk(Crunchy.one().shl(4).eq(ip.size()));
    });
    it("test_method_includes", () => {
        let ip = setup().ip;
        assert.equal(true, ip.includes(ip));
        // test prefix on same address
        let included = IPAddress.parse("2001:db8::8:800:200c:417a/128");
        let not_included = IPAddress.parse("2001:db8::8:800:200c:417a/46");
        assert.equal(true, ip.includes(included));
        assert.equal(false, ip.includes(not_included));
        // test address on same prefix
        included = IPAddress.parse("2001:db8::8:800:200c:0/64");
        not_included = IPAddress.parse("2001:db8:1::8:800:200c:417a/64");
        assert.equal(true, ip.includes(included));
        assert.equal(false, ip.includes(not_included));
        // general test
        included = IPAddress.parse("2001:db8::8:800:200c:1/128");
        not_included = IPAddress.parse("2001:db8:1::8:800:200c:417a/76");
        assert.equal(true, ip.includes(included));
        assert.equal(false, ip.includes(not_included));
    }
    );
    it("test_method_to_hex", () => {
        assert(setup().hex, setup().ip.to_hex());
    }
    );
    it("test_method_to_s", () => {
        assert.equal("2001:db8::8:800:200c:417a", setup().ip.to_s());
    }
    );
    it("test_method_to_string", () => {
        assert.equal("2001:db8::8:800:200c:417a/64", setup().ip.to_string());
    }
    );
    it("test_method_to_string_uncompressed", () => {
        let str = "2001:0db8:0000:0000:0008:0800:200c:417a/64";
        assert.equal(str, setup().ip.to_string_uncompressed());
    }
    );
    it("test_method_reverse", () => {
        let str = "f.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.2.0.0.0.5.0.5.0.e.f.f.3.ip6.arpa";
        assert.equal(str,
            IPAddress.parse("3ffe:505:2::f").dns_reverse());
    }
    );
    it("test_method_dns_rev_domains", () => {
        assertArrayEqual(IPAddress.parse("f000:f100::/3").dns_rev_domains(),
            ["e.ip6.arpa", "f.ip6.arpa"]);
        assertArrayEqual(IPAddress.parse("fea3:f120::/15").dns_rev_domains(),
            ["2.a.e.f.ip6.arpa", "3.a.e.f.ip6.arpa"]);
        assertArrayEqual(IPAddress.parse("3a03:2f80:f::/48").dns_rev_domains(),
            ["f.0.0.0.0.8.f.2.3.0.a.3.ip6.arpa"]);

        assertArrayEqual(IPAddress.parse("f000:f100::1234/125").dns_rev_domains(),
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
        assert.equal("1:1:1::1",
            IPAddress.parse("1:1:1:0:0:0:0:1").to_s());
        assert.equal("1:0:1::1",
            IPAddress.parse("1:0:1:0:0:0:0:1").to_s());
        assert.equal("1::1:1:1:2:3:1",
            IPAddress.parse("1:0:1:1:1:2:3:1").to_s());
        assert.equal("1::1:1:0:2:3:1",
            IPAddress.parse("1:0:1:1::2:3:1").to_s());
        assert.equal("1:0:0:1::1",
            IPAddress.parse("1:0:0:1:0:0:0:1").to_s());
        assert.equal("1::1:0:0:1",
            IPAddress.parse("1:0:0:0:1:0:0:1").to_s());
        assert.equal("1::1", IPAddress.parse("1:0:0:0:0:0:0:1").to_s());
        // assert.equal("1:1.1:2:0:0:1", IPAddress.parse("1:1:0:1:2.1").to_s
    }
    );
    it("test_method_unspecified", () => {
        assert.equal(true, IPAddress.parse("::").is_unspecified());
        assert.equal(false, setup().ip.is_unspecified());
    }
    );
    it("test_method_loopback", () => {
        assert.equal(true, IPAddress.parse("::1").is_loopback());
        assert.equal(false, setup().ip.is_loopback());
    }
    );
    it("test_method_network", () => {
        for (let i of setup().networks) {
            let addr = i[0];
            let net = i[1];
            let ip = IPAddress.parse(addr);
            assert.equal(net, ip.network().to_string());
        }
    });
    it("test_method_each", () => {
        let ip = IPAddress.parse("2001:db8::4/125");
        let arr: string[] = [];
        ip.each((i) => arr.push(i.to_s()));
        assertArrayEqual(arr, ["2001:db8::", "2001:db8::1", "2001:db8::2", "2001:db8::3",
            "2001:db8::4", "2001:db8::5", "2001:db8::6", "2001:db8::7"]);
    }
    );
    it("test_method_each_net", function(done) {
        this.timeout(30000);
        let test_addrs = ["0000:0000:0000:0000:0000:0000:0000:0000",
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
        for (let prefix = 0; prefix < 128; ++prefix) {
            let nr_networks = 1 << ((128 - prefix) % 4);
            for (let adr of test_addrs) {
                let net_adr = IPAddress.parse(`${adr}/${prefix}`);
                let ret = net_adr.dns_networks();
                assert.equal(ret[0].prefix.num % 4, 0);
                assert.equal(ret.length, nr_networks);
                assert.equal(net_adr.network().to_s(), ret[0].network().to_s());
                assert.equal(net_adr.broadcast().to_s(), ret[ret.length - 1].broadcast().to_s());
                //        puts "//{adr}///{prefix} //{nr_networks} //{ret}"
            }
        }
        let ret0: string[] = [];
        for (let i of IPAddress.parse("fd01:db8::4/3").dns_networks()) {
            ret0.push(i.to_string());
        }
        assertArrayEqual(ret0, ["e000::/4", "f000::/4"]);
        let ret1: string[] = [];
        for (let i of IPAddress.parse("3a03:2f80:f::/48").dns_networks()) {
            ret1.push(i.to_string());
        }
        assertArrayEqual(ret1, ["3a03:2f80:f::/48"]);
        done();
    });
    it("test_method_compare", () => {
        let ip1 = IPAddress.parse("2001:db8:1::1/64");
        let ip2 = IPAddress.parse("2001:db8:2::1/64");
        let ip3 = IPAddress.parse("2001:db8:1::2/64");
        let ip4 = IPAddress.parse("2001:db8:1::1/65");

        // ip2 should be greater than ip1
        assert.equal(true, ip2.gt(ip1));
        assert.equal(false, ip1.gt(ip2));
        assert.equal(false, ip2.lt(ip1));
        // ip3 should be less than ip2
        assert.equal(true, ip2.gt(ip3));
        assert.equal(false, ip2.lt(ip3));
        // ip1 should be less than ip3
        assert.equal(true, ip1.lt(ip3));
        assert.equal(false, ip1.gt(ip3));
        assert.equal(false, ip3.lt(ip1));
        // ip1 should be equal to itself
        assert.equal(true, ip1.eq(ip1));
        // ip4 should be greater than ip1
        assert.equal(true, ip1.lt(ip4));
        assert.equal(false, ip1.gt(ip4));
        // test sorting
        let r = [ip1, ip2, ip3, ip4].sort((a, b) => a.cmp(b));
        let ret: string[] = [];
        for (let i of r) {
            ret.push(i.to_string());
        }
        assertArrayEqual(ret, ["2001:db8:1::1/64", "2001:db8:1::1/65", "2001:db8:1::2/64", "2001:db8:2::1/64"]);
    }

        // it("test_classmethod_expand", () => {
        //   let compressed = "2001:db8:0:cd30.";
        //   let expanded = "2001:0db8:0000:cd30:0000:0000:0000:0000";
        //   assert.equal(expanded, @klass.expand(compressed));
        //   assert.equal(expanded, @klass.expand("2001:0db8:0.cd3"));
        //   assert.equal(expanded, @klass.expand("2001:0db8.cd30"));
        //   assert.equal(expanded, @klass.expand("2001:0db8.cd3"));
        // }
    );
    it("test_classmethod_compress", () => {
        let compressed = "2001:db8:0:cd30::";
        let expanded = "2001:0db8:0000:cd30:0000:0000:0000:0000";
        assert.equal(compressed, IPAddress.parse(expanded).to_s());
        assert.equal("2001:db8::cd3",
            IPAddress.parse("2001:0db8:0::cd3").to_s());
        assert.equal("2001:db8::cd30",
            IPAddress.parse("2001:0db8::cd30").to_s());
        assert.equal("2001:db8::cd3",
            IPAddress.parse("2001:0db8::cd3").to_s());
    });
    it("test_classhmethod_parse_u128", () => {
        for (let i of setup().valid_ipv6) {
            let ip = i[0];
            let num = i[1];
            //console.log(">>>>>>>>", i);
            assert.equal(IPAddress.parse(ip).to_s(), Ipv6.from_int(num, 128).to_s());
        }
    });
    it("test_classmethod_parse_hex", () => {
        assert.equal(setup().ip.to_string(),
            Ipv6.from_str(setup().hex, 16, 64).to_string());
    });
});
