
#include "chai.hpp"
#include "mocha.hpp"

#include "../src/crunchy.hpp"
#include "../src/ipaddress.hpp"
#include "../src/ipv6.hpp"
#include "../src/prefix128.hpp"

class IPv6Test {
public:
  std::vector<std::pair<std::string, std::string>> compress_addr;
  std::vector<std::pair<std::string, Crunchy>> valid_ipv6;
  std::vector<std::string> invalid_ipv6 = {":1:2:3:4:5:6:7", ":1:2:3:4:5:6:7",
                                           "2002:516:2:200", "dd"};
  std::vector<std::pair<std::string, std::string>> networks;
  IPAddress ip = IPAddress::parse("2001:db8::8:800:200c:417a/64").unwrap();
  IPAddress network = IPAddress::parse("2001:db8:8:800::/64").unwrap();
  std::vector<size_t> arr = {8193, 3512, 0, 0, 8, 2048, 8204, 16762};
  const char *hex = "20010db80000000000080800200c417a";
};

IPv6Test setup() {
  IPv6Test ip6t;
  ip6t.compress_addr.push_back(
      {"2001:db8:0000:0000:0008:0800:200c:417a", "2001:db8::8:800:200c:417a"});
  ip6t.compress_addr.push_back(
      {"2001:db8:0:0:8:800:200c:417a", "2001:db8.8:800:200c:417a"});
  ip6t.compress_addr.push_back({"ff01:0:0:0:0:0:0:101", "ff01::101"});
  ip6t.compress_addr.push_back({"0:0:0:0:0:0:0:1", ".1"});
  ip6t.compress_addr.push_back({"0:0:0:0:0:0:0:0", "."});

  ip6t.valid_ipv6.push_back(
      {"FEDC:BA98:7654:3210:FEDC:BA98:7654:3210",
       Crunchy::parse("338770000845734292534325025077361652240").unwrap()});
  ip6t.valid_ipv6.push_back(
      {"1080:0000:0000:0000:0008:0800:200C:417A",
       Crunchy::parse("21932261930451111902915077091070067066").unwrap()});
  ip6t.valid_ipv6.push_back(
      {"1080:0:0:0:8:800:200C:417A",
       Crunchy::parse("21932261930451111902915077091070067066").unwrap()});
  ip6t.valid_ipv6.push_back(
      {"1080:0::8:800:200C:417A",
       Crunchy::parse("21932261930451111902915077091070067066").unwrap()});
  ip6t.valid_ipv6.push_back(
      {"1080::8:800:200C:417A",
       Crunchy::parse("21932261930451111902915077091070067066").unwrap()});
  ip6t.valid_ipv6.push_back(
      {"FF01:0:0:0:0:0:0:43",
       Crunchy::parse("338958331222012082418099330867817087043").unwrap()});
  ip6t.valid_ipv6.push_back(
      {"FF01:0::0:0:43",
       Crunchy::parse("338958331222012082418099330867817087043").unwrap()});
  ip6t.valid_ipv6.push_back(
      {"FF01::43",
       Crunchy::parse("338958331222012082418099330867817087043").unwrap()});
  ip6t.valid_ipv6.push_back({"0:0:0:0:0:0:0:1", Crunchy::parse("1").unwrap()});
  ip6t.valid_ipv6.push_back({"0:0:0::0:0:1", Crunchy::parse("1").unwrap()});
  ip6t.valid_ipv6.push_back({"::1", Crunchy::parse("1").unwrap()});
  ip6t.valid_ipv6.push_back({"0:0:0:0:0:0:0:0", Crunchy::parse("0").unwrap()});
  ip6t.valid_ipv6.push_back({"0:0:0::0:0:0", Crunchy::parse("0").unwrap()});
  ip6t.valid_ipv6.push_back({"::", Crunchy::parse("0").unwrap()});
  ip6t.valid_ipv6.push_back({"::/0", Crunchy::parse("0").unwrap()});
  ip6t.valid_ipv6.push_back(
      {"1080:0:0:0:8:800:200C:417A",
       Crunchy::parse("21932261930451111902915077091070067066").unwrap()});
  ip6t.valid_ipv6.push_back(
      {"1080::8:800:200C:417A",
       Crunchy::parse("21932261930451111902915077091070067066").unwrap()});

  ip6t.networks.push_back({"2001:db8:1:1:1:1:1:1/32", "2001:db8::/32"});
  ip6t.networks.push_back({"2001:db8:1:1:1:1:1::/32", "2001:db8::/32"});
  ip6t.networks.push_back({"2001:db8::1/64", "2001:db8::/64"});
  return ip6t;
}

int main() {
  describe("ipv6", []() {

    it("test_attribute_address", []() {
      auto addr = "2001:0db8:0000:0000:0008:0800:200c:417a";
      Chai::assert.equal(addr, setup().ip.to_s_uncompressed());
    });
    it("test_initialize", []() {
      Chai::assert.equal(false, setup().ip.is_ipv4());

      for (auto ip : setup().valid_ipv6) {
        Chai::assert.isTrue(IPAddress::parse(ip.first).isOk());
      }
      for (auto ip : setup().invalid_ipv6) {
        Chai::assert.isFalse(IPAddress::parse(ip).isErr());
      }
      Chai::assert.equal(64, setup().ip.prefix.num);

      Chai::assert.isTrue(IPAddress::parse("::10.1.1.1").isOk());
    });
    it("test_attribute_groups", []() {
         Chai::assert.deepEqual(setup().arr, setup().ip.parts());
     });
    it("test_method_hexs", []() {
        Chai::assert.deepEqual(setup().ip.parts_hex_str(),
            {"2001", "0db8", "0000", "0000", "0008", "0800", "200c", "417a"});
  });

  it("test_method_to_i", []() {
    for (auto i : setup().valid_ipv6) {
      auto ip = i.first;
      auto num = i.second;
      Chai::assert.isTrue(num.eq(IPAddress::parse(ip).unwrap().host_address));
    }
  });
  it("test_method_set_prefix", []() {
    auto ip = IPAddress::parse("2001:db8::8:800:200c:417a").unwrap();
    Chai::assert.equal(128, ip.prefix.num);
    Chai::assert.equal("2001:db8::8:800:200c:417a/128", ip.to_string());
    auto nip = ip.change_prefix(64).unwrap();
    Chai::assert.equal(64, nip.prefix.num);
    Chai::assert.equal("2001:db8::8:800:200c:417a/64", nip.to_string());
  });
  it("test_method_mapped", []() {
    Chai::assert.equal(false, setup().ip.is_mapped());
    auto ip6 = IPAddress::parse("::ffff:1234:5678").unwrap();
    Chai::assert.equal(true, ip6.is_mapped());
  });
  it("test_method_group", []() {
    auto s = setup();
    Chai::assert.deepEqual(s.ip.parts(), s.arr);
  });
  it("test_method_ipv4()",
     []() { Chai::assert.equal(false, setup().ip.is_ipv4()); });
  it("test_method_ipv6",
     []() { Chai::assert.equal(true, setup().ip.is_ipv6()); });
  it("test_method_network_known", []() {
    Chai::assert.equal(true, setup().network.is_network());
    Chai::assert.equal(false, setup().ip.is_network());
  });
  it("test_method_network_u128", []() {
    Chai::assert.isTrue(
        Ipv6::from_int(Crunchy::parse("42540766411282592856903984951653826560").unwrap(),
                       64).unwrap()
            .eq(setup().ip.network()));
  });
  it("test_method_broadcast_u128", []() {
    Chai::assert.isTrue(
        Ipv6::from_int(Crunchy::parse("42540766411282592875350729025363378175").unwrap(),
                       64).unwrap()
            .eq(setup().ip.broadcast()));
  });
  it("test_method_size", []() {
    auto ip = IPAddress::parse("2001:db8::8:800:200c:417a/64").unwrap();
    Chai::assert.isTrue(Crunchy::one().shl(64).eq(ip.size()));
    ip = IPAddress::parse("2001:db8::8:800:200c:417a/32").unwrap();
    Chai::assert.isTrue(Crunchy::one().shl(96).eq(ip.size()));
    ip = IPAddress::parse("2001:db8::8:800:200c:417a/120").unwrap();
    Chai::assert.isTrue(Crunchy::one().shl(8).eq(ip.size()));
    ip = IPAddress::parse("2001:db8::8:800:200c:417a/124").unwrap();
    Chai::assert.isTrue(Crunchy::one().shl(4).eq(ip.size()));
  });
  it("test_method_includes", []() {
    auto ip = setup().ip;
    Chai::assert.equal(true, ip.includes(ip));
    // test prefix on same address
    auto included = IPAddress::parse("2001:db8::8:800:200c:417a/128").unwrap();
    auto not_included = IPAddress::parse("2001:db8::8:800:200c:417a/46").unwrap();
    Chai::assert.equal(true, ip.includes(included));
    Chai::assert.equal(false, ip.includes(not_included));
    // test address on same prefix
    included = IPAddress::parse("2001:db8::8:800:200c:0/64").unwrap();
    not_included = IPAddress::parse("2001:db8:1::8:800:200c:417a/64").unwrap();
    Chai::assert.equal(true, ip.includes(included));
    Chai::assert.equal(false, ip.includes(not_included));
    // general test
    included = IPAddress::parse("2001:db8::8:800:200c:1/128").unwrap();
    not_included = IPAddress::parse("2001:db8:1::8:800:200c:417a/76").unwrap();
    Chai::assert.equal(true, ip.includes(included));
    Chai::assert.equal(false, ip.includes(not_included));
  });
  it("test_method_to_hex",
     []() { Chai::assert.equal(setup().hex, setup().ip.to_hex()); });
  it("test_method_to_s", []() {
    Chai::assert.equal("2001:db8::8:800:200c:417a", setup().ip.to_s());
  });
  it("test_method_to_string", []() {
    Chai::assert.equal("2001:db8::8:800:200c:417a/64", setup().ip.to_string());
  });
  it("test_method_to_string_uncompressed", []() {
    auto str = "2001:0db8:0000:0000:0008:0800:200c:417a/64";
    Chai::assert.equal(str, setup().ip.to_string_uncompressed());
  });
  it("test_method_reverse", []() {
    auto str = "f.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.2.0.0.0.5.0.5.0.e.f.f."
               "3.ip6.arpa";
    Chai::assert.equal(str, IPAddress::parse("3ffe:505:2::f").unwrap().dns_reverse());
  });
    it("test_method_dns_rev_domains", []() {
        Chai::assert.deepEqual(IPAddress::parse("f000:f100::/3").unwrap().dns_rev_domains(),
            {"e.ip6.arpa", "f.ip6.arpa"});
        Chai::assert.deepEqual(IPAddress::parse("fea3:f120::/15").unwrap().dns_rev_domains(),
            {"2.a.e.f.ip6.arpa", "3.a.e.f.ip6.arpa"
});
        Chai::assert.deepEqual(IPAddress::parse("3a03:2f80:f::/48").unwrap().dns_rev_domains(),
            {"f.0.0.0.0.8.f.2.3.0.a.3.ip6.arpa"
        });

        Chai::assert.deepEqual(IPAddress::parse("f000:f100::1234/125").unwrap().dns_rev_domains(),
            {"0.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                "1.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                "2.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                "3.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                "4.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                "5.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                "6.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                "7.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa"
        });
        }
    );
    it("test_method_compressed", []() {
      Chai::assert.equal("1:1:1::1",
                         IPAddress::parse("1:1:1:0:0:0:0:1").unwrap().to_s());
      Chai::assert.equal("1:0:1::1",
                         IPAddress::parse("1:0:1:0:0:0:0:1").unwrap().to_s());
      Chai::assert.equal("1::1:1:1:2:3:1",
                         IPAddress::parse("1:0:1:1:1:2:3:1").unwrap().to_s());
      Chai::assert.equal("1::1:1:0:2:3:1",
                         IPAddress::parse("1:0:1:1::2:3:1").unwrap().to_s());
      Chai::assert.equal("1:0:0:1::1",
                         IPAddress::parse("1:0:0:1:0:0:0:1").unwrap().to_s());
      Chai::assert.equal("1::1:0:0:1",
                         IPAddress::parse("1:0:0:0:1:0:0:1").unwrap().to_s());
      Chai::assert.equal("1::1", IPAddress::parse("1:0:0:0:0:0:0:1").unwrap().to_s());
      // Chai::assert.equal("1:1.1:2:0:0:1",
      // IPAddress::parse("1:1:0:1:2.1").to_s
    });
    it("test_method_unspecified", []() {
      Chai::assert.equal(true, IPAddress::parse("::").unwrap().is_unspecified());
      Chai::assert.equal(false, setup().ip.is_unspecified());
    });
    it("test_method_loopback", []() {
      Chai::assert.equal(true, IPAddress::parse("::1").unwrap().is_loopback());
      Chai::assert.equal(false, setup().ip.is_loopback());
    });
    it("test_method_network", []() {
      for (auto i : setup().networks) {
        auto addr = i.first;
        auto net = i.second;
        auto ip = IPAddress::parse(addr).unwrap();
        Chai::assert.equal(net, ip.network().to_string());
      }
    });
    it("test_method_each", []() {
      auto ip = IPAddress::parse("2001:db8::4/125").unwrap();
      std::vector<std::string> arr;
      ip.each([&arr](const IPAddress &i) { arr.push_back(i.to_s()); });
        Chai::assert.deepEqual(arr, {"2001:db8::", "2001:db8::1", "2001:db8::2", "2001:db8::3",
            "2001:db8::4", "2001:db8::5", "2001:db8::6", "2001:db8::7"
    });
    }
    );
    it("test_method_each_net", []() {
      auto test_addrs = {
        "0000:0000:0000:0000:0000:0000:0000:0000",
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
        "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"
      };
      for (auto prefix = 0; prefix < 128; ++prefix) {
        auto nr_networks = 1 << ((128 - prefix) % 4);
        for (auto adr : test_addrs) {
          std::stringstream s2;
          s2 << adr << "/" << prefix;
          auto net_adr = IPAddress::parse(s2.str()).unwrap();
          auto ret = net_adr.dns_networks();
          Chai::assert.equal(ret[0].prefix.num % 4, 0);
          Chai::assert.equal(ret.size(), nr_networks);
          Chai::assert.equal(net_adr.network().to_s(), ret[0].network().to_s());
          Chai::assert.equal(net_adr.broadcast().to_s(),
                             ret[ret.size() - 1].broadcast().to_s());
          //        puts "//{adr}///{prefix} //{nr_networks} //{ret}"
        }
      }
      std::vector<std::string> ret0;
      for (auto i : IPAddress::parse("fd01:db8::4/3").unwrap().dns_networks()) {
        ret0.push_back(i.to_string());
      }
        Chai::assert.deepEqual(ret0, {"e000::/4", "f000::/4" });
    std::vector<std::string> ret1;
    for (auto i : IPAddress::parse("3a03:2f80:f::/48").unwrap().dns_networks()) {
      ret1.push_back(i.to_string());
    }
        Chai::assert.deepEqual(ret1, {"3a03:2f80:f::/48" });
        });
        it("test_method_compare", []() {
          auto ip1 = IPAddress::parse("2001:db8:1::1/64").unwrap();
          auto ip2 = IPAddress::parse("2001:db8:2::1/64").unwrap();
          auto ip3 = IPAddress::parse("2001:db8:1::2/64").unwrap();
          auto ip4 = IPAddress::parse("2001:db8:1::1/65").unwrap();

          // ip2 should be greater than ip1
          Chai::assert.equal(true, ip2.gt(ip1));
          Chai::assert.equal(false, ip1.gt(ip2));
          Chai::assert.equal(false, ip2.lt(ip1));
          // ip3 should be less than ip2
          Chai::assert.equal(true, ip2.gt(ip3));
          Chai::assert.equal(false, ip2.lt(ip3));
          // ip1 should be less than ip3
          Chai::assert.equal(true, ip1.lt(ip3));
          Chai::assert.equal(false, ip1.gt(ip3));
          Chai::assert.equal(false, ip3.lt(ip1));
          // ip1 should be equal to itself
          Chai::assert.equal(true, ip1.eq(ip1));
          // ip4 should be greater than ip1
          Chai::assert.equal(true, ip1.lt(ip4));
          Chai::assert.equal(false, ip1.gt(ip4));
          // test sorting
          std::vector<IPAddress> res = { ip1, ip2, ip3, ip4 } ;
          std::sort (res.begin(), res.end(), [](const IPAddress &a, const IPAddress &b) {
            return a.lt(b);
          });
        Chai::assert.deepEqual(IPAddress::to_string_vec(res),
        {"2001:db8:1::1/64", "2001:db8:1::1/65", "2001:db8:1::2/64", "2001:db8:2::1/64"});
        }

        // it("test_classmethod_expand", []() {
        //   auto compressed = "2001:db8:0:cd30.";
        //   auto expanded = "2001:0db8:0000:cd30:0000:0000:0000:0000";
        //   Chai::assert.equal(expanded, @klass.expand(compressed));
        //   Chai::assert.equal(expanded, @klass.expand("2001:0db8:0.cd3"));
        //   Chai::assert.equal(expanded, @klass.expand("2001:0db8.cd30"));
        //   Chai::assert.equal(expanded, @klass.expand("2001:0db8.cd3"));
        // }
    );
    it("test_classmethod_compress", []() {
      auto compressed = "2001:db8:0:cd30::";
      auto expanded = "2001:0db8:0000:cd30:0000:0000:0000:0000";
      Chai::assert.equal(compressed, IPAddress::parse(expanded).unwrap().to_s());
      Chai::assert.equal("2001:db8::cd3",
                         IPAddress::parse("2001:0db8:0::cd3").unwrap().to_s());
      Chai::assert.equal("2001:db8::cd30",
                         IPAddress::parse("2001:0db8::cd30").unwrap().to_s());
      Chai::assert.equal("2001:db8::cd3",
                         IPAddress::parse("2001:0db8::cd3").unwrap().to_s());
    });
    it("test_classhmethod_parse_u128", []() {
      for (auto i : setup().valid_ipv6) {
        auto ip = i.first;
        auto num = i.second;
        // console.log(">>>>>>>>", i);
        Chai::assert.equal(IPAddress::parse(ip).unwrap().to_s(),
                           Ipv6::from_int(num, 128).unwrap().to_s());
      }
    });
    it("test_classmethod_parse_hex", []() {
      Chai::assert.equal(setup().ip.to_string(),
                         Ipv6::from_str(setup().hex, 16, 64).unwrap().to_string());
    });
    });
  return exit();
    }
