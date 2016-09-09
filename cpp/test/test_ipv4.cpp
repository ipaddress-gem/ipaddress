
#include "chai.hpp"
#include "mocha.hpp"

#include "../src/ipaddress.hpp"

#include "../src/prefix32.hpp"
#include "../src/ipv4.hpp"
#include "../src/crunchy.hpp"

typedef struct S_IPv4Prefix {
  std::string ip;
  size_t  prefix;
} IPv4Prefix;

class IPv4Test {
  public:
  std::vector<std::pair<std::string, IPv4Prefix>> valid_ipv4;
  std::vector<std::pair<std::string, Crunchy>> decimal_values;

  std::vector<std::pair<std::string, std::string>> netmask_values;
  std::vector<std::pair<std::string, size_t>> classful;
  std::vector<std::pair<std::string, std::string>> networks;
  std::vector<std::pair<std::string, std::string>> broadcast;

  IPAddress ip = IPAddress::parse("172.16.10.1/24").unwrap();
  IPAddress network = IPAddress::parse("172.16.10.0/24").unwrap();
  std::vector<std::string> invalid_ipv4 = {"10.0.0.256", "10.0.0.0.0"};
  std::vector<std::string> valid_ipv4_range = {"10.0.0.1-254", "10.0.1-254.0", "10.1-254.0.0"};
  IPAddress class_a = IPAddress::parse("10.0.0.1/8").unwrap();
  IPAddress class_b = IPAddress::parse("172.16.0.1/16").unwrap();
  IPAddress class_c = IPAddress::parse("192.168.0.1/24").unwrap();
};


IPv4Test  setup() {
  IPv4Test ipv4t;
  ipv4t.valid_ipv4.push_back({"9.9/17",
      { "9.0.0.9", 17 }
  });
ipv4t.valid_ipv4.push_back({"100.1.100",
    { "100.1.0.100", 32 } });
ipv4t.valid_ipv4.push_back({"0.0.0.0/0",
    { "0.0.0.0", 0 }});
ipv4t.valid_ipv4.push_back({"10.0.0.0",
    { "10.0.0.0", 32 }});
ipv4t.valid_ipv4.push_back({"10.0.0.1",
    { "10.0.0.1", 32 }});
ipv4t.valid_ipv4.push_back({"10.0.0.1/24",
    { "10.0.0.1", 24 }});
ipv4t.valid_ipv4.push_back({"10.0.0.9/255.255.255.0",
    { "10.0.0.9", 24 }});

ipv4t.netmask_values.push_back({"0.0.0.0/0", "0.0.0.0"});
ipv4t.netmask_values.push_back({"10.0.0.0/8", "255.0.0.0"});
ipv4t.netmask_values.push_back({"172.16.0.0/16", "255.255.0.0"});
ipv4t.netmask_values.push_back({"192.168.0.0/24", "255.255.255.0"});
ipv4t.netmask_values.push_back({"192.168.100.4/30", "255.255.255.252"});

ipv4t.decimal_values.push_back({"0.0.0.0/0", Crunchy::parse("0").unwrap()});
ipv4t.decimal_values.push_back({"10.0.0.0/8", Crunchy::parse("167772160").unwrap()});
ipv4t.decimal_values.push_back({"172.16.0.0/16", Crunchy::parse("2886729728").unwrap()});
ipv4t.decimal_values.push_back({"192.168.0.0/24", Crunchy::parse("3232235520").unwrap()});
ipv4t.decimal_values.push_back({"192.168.100.4/30", Crunchy::parse("3232261124").unwrap()});


ipv4t.broadcast.push_back({"10.0.0.0/8", "10.255.255.255/8"});
ipv4t.broadcast.push_back({"172.16.0.0/16", "172.16.255.255/16"});
ipv4t.broadcast.push_back({"192.168.0.0/24", "192.168.0.255/24"});
ipv4t.broadcast.push_back({"192.168.100.4/30", "192.168.100.7/30"});

ipv4t.networks.push_back({"10.5.4.3/8", "10.0.0.0/8"});
ipv4t.networks.push_back({"172.16.5.4/16", "172.16.0.0/16"});
ipv4t.networks.push_back({"192.168.4.3/24", "192.168.4.0/24"});
ipv4t.networks.push_back({"192.168.100.5/30", "192.168.100.4/30"});


ipv4t.classful.push_back({"10.1.1.1", 8});
ipv4t.classful.push_back({"150.1.1.1", 16});
ipv4t.classful.push_back({"200.1.1.1", 24});
return ipv4t;
}


int main() {
  describe("Ipv4Test", []() {

      it("test_initialize", []() {
          for (auto i : setup().valid_ipv4) {
          //console.log(i[0]);
          auto ip = IPAddress::parse(i.first).unwrap();
          Chai::assert.isTrue(ip.is_ipv4() && !ip.is_ipv6());
          }
          Chai::assert.equal(32, setup().ip.prefix.ip_bits.bits);
          Chai::assert.isTrue(IPAddress::parse("1.f.13.1/-3").isErr());
          Chai::assert.isTrue(IPAddress::parse("10.0.0.0/8").isOk());
          });
      it("test_initialize_format_error", []() {
          for (auto i : setup().invalid_ipv4) {
          Chai::assert.isTrue(IPAddress::parse(i).isErr());
          }
          Chai::assert.isTrue(IPAddress::parse("10.0.0.0/asd").isErr());
          });
      it("test_initialize_without_prefix", []() {
          Chai::assert.isTrue(IPAddress::parse("10.10.0.0").isOk());
          auto ip = IPAddress::parse("10.10.0.0").unwrap();
          Chai::assert.isTrue(!ip.is_ipv6() && ip.is_ipv4());
          Chai::assert.equal(32, ip.prefix.num);
          });
      it("test_attributes", []() {
          for (auto i : setup().valid_ipv4) {
          auto arg = i.first;
          auto attr = i.second;
          auto ip = IPAddress::parse(arg).unwrap();
          // println!("test_attributes:{}:{:?}", arg, attr);
          Chai::assert.equal(attr.ip, ip.to_s());
          Chai::assert.equal(attr.prefix, ip.prefix.num);
          }
          });
      it("test_octets", []() {
          auto ip = IPAddress::parse("10.1.2.3/8").unwrap();
          Chai::assert.deepEqual(ip.parts(), {10, 1, 2, 3});
          });
      it("test_method_to_string", []() {
          for (auto i : setup().valid_ipv4) {
          auto arg = i.first;
          auto attr = i.second;
          auto ip = IPAddress::parse(arg).unwrap();
          std::stringstream s2;
          s2 << attr.ip << "/" << attr.prefix;
          Chai::assert.equal(s2.str(), ip.to_string());
          }
          });
      it("test_method_to_s", []() {
          for (auto i : setup().valid_ipv4) {
          auto arg = i.first;
          auto attr = i.second;
          auto ip = IPAddress::parse(arg).unwrap();
          Chai::assert.equal(attr.ip, ip.to_s());
          // auto ip_c = IPAddress::parse(arg);
          // Chai::assert.equal(attr.ip, ip.to_s());
          }
          });
      it("test_netmask", []() {
          for (auto i : setup().netmask_values) {
          auto addr = i.first;
          auto mask = i.second;
          auto ip = IPAddress::parse(addr).unwrap();
          Chai::assert.equal(ip.netmask().to_s(), mask);
          }
          });
      it("test_method_to_u32", []() {
          for (auto i : setup().decimal_values) {
          auto addr = i.first;
          auto my = i.second;
          auto ip = IPAddress::parse(addr).unwrap();
          Chai::assert.isTrue(ip.host_address.eq(my));
          }
          });
      it("test_method_is_network", []() {
          Chai::assert.equal(true, setup().network.is_network());
          Chai::assert.equal(false, setup().ip.is_network());
          });
      it("test_one_address_network", []() {
          auto network = IPAddress::parse("172.16.10.1/32").unwrap();
          Chai::assert.equal(false, network.is_network());
          });
      it("test_method_broadcast", []() {
          for (auto i : setup().broadcast) {
          auto addr = i.first;
          auto bcast = i.second;
          auto ip = IPAddress::parse(addr).unwrap();
          Chai::assert.equal(bcast, ip.broadcast().to_string());
          }
          }
        );
      it("test_method_network", []() {
          for (auto i : setup().networks) {
          auto addr = i.first;
          auto net = i.second;
          auto ip = IPAddress::parse(addr).unwrap();
          Chai::assert.equal(net, ip.network().to_string());
          }
          }

        );
      it("test_method_bits", []() {
          auto ip = IPAddress::parse("127.0.0.1").unwrap();
          Chai::assert.equal("01111111000000000000000000000001", ip.bits());
          }
        );
      it("test_method_first", []() {
          auto ip = IPAddress::parse("192.168.100.0/24").unwrap();
          Chai::assert.equal("192.168.100.1", ip.first().to_s());
          ip = IPAddress::parse("192.168.100.50/24").unwrap();
          Chai::assert.equal("192.168.100.1", ip.first().to_s());
          }
        );
      it("test_method_last", []() {
          auto ip = IPAddress::parse("192.168.100.0/24").unwrap();
          Chai::assert.equal("192.168.100.254", ip.last().to_s());
          ip = IPAddress::parse("192.168.100.50/24").unwrap();
          Chai::assert.equal("192.168.100.254", ip.last().to_s());
          }
        );
      it("test_method_each_host", []() {
          auto ip = IPAddress::parse("10.0.0.1/29").unwrap();
          std::vector<std::string> arr;
          ip.each_host([&arr](const IPAddress & i) { arr.push_back(i.to_s()); });

          Chai::assert.deepEqual(arr, {"10.0.0.1", "10.0.0.2", "10.0.0.3", "10.0.0.4", "10.0.0.5", "10.0.0.6"});
          }
        );
      it("test_method_each", []() {
          auto ip = IPAddress::parse("10.0.0.1/29").unwrap();
          std::vector<std::string> arr;
          ip.each([&arr](const IPAddress & i) { arr.push_back(i.to_s());});
          Chai::assert.deepEqual(arr, {"10.0.0.0", "10.0.0.1", "10.0.0.2", "10.0.0.3", "10.0.0.4", "10.0.0.5",
              "10.0.0.6", "10.0.0.7"});
          }
        );
      it("test_method_size", []() {
          auto ip = IPAddress::parse("10.0.0.1/29").unwrap();
          Chai::assert.isTrue(ip.size().eq(Crunchy::parse("8").unwrap()));
          }
        );
      it("test_method_network_u32", []() {
          Chai::assert.equal("2886732288", setup().ip.network().host_address.toString());
          }
        );
      it("test_method_broadcast_u32", []() {
          Chai::assert.equal("2886732543", setup().ip.broadcast().host_address.toString());
          }
        );
      it("test_method_include", []() {
          auto ip = IPAddress::parse("192.168.10.100/24").unwrap();
          auto addr = IPAddress::parse("192.168.10.102/24").unwrap();
          Chai::assert.equal(true, ip.includes(addr));
          Chai::assert.equal(false,
              ip.includes(IPAddress::parse("172.16.0.48").unwrap()));
          ip = IPAddress::parse("10.0.0.0/8").unwrap();
          Chai::assert.equal(true, ip.includes(IPAddress::parse("10.0.0.0/9").unwrap()));
          Chai::assert.equal(true, ip.includes(IPAddress::parse("10.1.1.1/32").unwrap()));
          Chai::assert.equal(true, ip.includes(IPAddress::parse("10.1.1.1/9").unwrap()));
          Chai::assert.equal(false,
              ip.includes(IPAddress::parse("172.16.0.0/16").unwrap()));
          Chai::assert.equal(false, ip.includes(IPAddress::parse("10.0.0.0/7").unwrap()));
          Chai::assert.equal(false, ip.includes(IPAddress::parse("5.5.5.5/32").unwrap()));
          Chai::assert.equal(false, ip.includes(IPAddress::parse("11.0.0.0/8").unwrap()));
          ip = IPAddress::parse("13.13.0.0/13").unwrap();
          Chai::assert.equal(false,
              ip.includes(IPAddress::parse("13.16.0.0/32").unwrap()));
          }
        );
      it("test_method_include_all", []() {
          auto ip = IPAddress::parse("192.168.10.100/24").unwrap();
          auto addr1 = IPAddress::parse("192.168.10.102/24").unwrap();
          auto addr2 = IPAddress::parse("192.168.10.103/24").unwrap();
          auto tmp = {addr1, addr2};
          Chai::assert.equal(true, ip.includes_all(tmp));
          Chai::assert.equal(false,
              ip.includes_all({addr1, IPAddress::parse("13.16.0.0/32").unwrap()}));
          }
        );
      it("test_method_ipv4", []() {
          Chai::assert.equal(true, setup().ip.is_ipv4());
          }
        );
      it("test_method_ipv6", []() {
          Chai::assert.equal(false, setup().ip.is_ipv6());
          }
        );
      it("test_method_private", []() {
          Chai::assert.equal(true,
              IPAddress::parse("192.168.10.50/24").unwrap().is_private());
          Chai::assert.equal(true,
              IPAddress::parse("192.168.10.50/16").unwrap().is_private());
          Chai::assert.equal(true,
              IPAddress::parse("172.16.77.40/24").unwrap().is_private());
          Chai::assert.equal(true,
              IPAddress::parse("172.16.10.50/14").unwrap().is_private());
          Chai::assert.equal(true,
              IPAddress::parse("10.10.10.10/10").unwrap().is_private());
          Chai::assert.equal(true, IPAddress::parse("10.0.0.0/8").unwrap().is_private());
          Chai::assert.equal(false,
              IPAddress::parse("192.168.10.50/12").unwrap().is_private());
          Chai::assert.equal(false, IPAddress::parse("3.3.3.3").unwrap().is_private());
          Chai::assert.equal(false, IPAddress::parse("10.0.0.0/7").unwrap().is_private());
          Chai::assert.equal(false,
              IPAddress::parse("172.32.0.0/12").unwrap().is_private());
          Chai::assert.equal(false,
              IPAddress::parse("172.16.0.0/11").unwrap().is_private());
          Chai::assert.equal(false,
              IPAddress::parse("192.0.0.2/24").unwrap().is_private());
      }
      );
      it("test_method_octet", []() {
          Chai::assert.equal(setup().ip.parts()[0], 172);
          Chai::assert.equal(setup().ip.parts()[1], 16);
          Chai::assert.equal(setup().ip.parts()[2], 10);
          Chai::assert.equal(setup().ip.parts()[3], 1);
          }
        );
      it("test_method_a", []() {
          Chai::assert.equal(true, Ipv4::is_class_a(setup().class_a));
          Chai::assert.equal(false, Ipv4::is_class_a(setup().class_b));
          Chai::assert.equal(false, Ipv4::is_class_a(setup().class_c));
          }
        );
      it("test_method_b", []() {
          Chai::assert.equal(true, Ipv4::is_class_b(setup().class_b));
          Chai::assert.equal(false, Ipv4::is_class_b(setup().class_a));
          Chai::assert.equal(false, Ipv4::is_class_b(setup().class_c));
          }
        );
      it("test_method_c", []() {
          Chai::assert.equal(true, Ipv4::is_class_c(setup().class_c));
          Chai::assert.equal(false, Ipv4::is_class_c(setup().class_a));
          Chai::assert.equal(false, Ipv4::is_class_c(setup().class_b));
          }
        );
      it("test_method_to_ipv6", []() {
          Chai::assert.equal("::ac10:a01", setup().ip.to_ipv6().to_s());
          }
        );
      it("test_method_reverse", []() {
          Chai::assert.equal(setup().ip.dns_reverse(), "10.16.172.in-addr.arpa");
          }
        );
      it("test_method_dns_rev_domains", []() {
          Chai::assert.deepEqual(IPAddress::parse("173.17.5.1/23").unwrap().dns_rev_domains(),
              {"4.17.173.in-addr.arpa", "5.17.173.in-addr.arpa"});
          Chai::assert.deepEqual(IPAddress::parse("173.17.1.1/15").unwrap().dns_rev_domains(),
              {"16.173.in-addr.arpa", "17.173.in-addr.arpa"});
          Chai::assert.deepEqual(IPAddress::parse("173.17.1.1/7").unwrap().dns_rev_domains(),
              {"172.in-addr.arpa", "173.in-addr.arpa"});
          Chai::assert.deepEqual(IPAddress::parse("173.17.1.1/29").unwrap().dns_rev_domains(),
              {
              "0.1.17.173.in-addr.arpa",
              "1.1.17.173.in-addr.arpa",
              "2.1.17.173.in-addr.arpa",
              "3.1.17.173.in-addr.arpa",
              "4.1.17.173.in-addr.arpa",
              "5.1.17.173.in-addr.arpa",
              "6.1.17.173.in-addr.arpa",
              "7.1.17.173.in-addr.arpa"
              });
          Chai::assert.deepEqual(IPAddress::parse("174.17.1.1/24").unwrap().dns_rev_domains(),
              {"1.17.174.in-addr.arpa"});
          Chai::assert.deepEqual(IPAddress::parse("175.17.1.1/16").unwrap().dns_rev_domains(),
              {"17.175.in-addr.arpa"});
          Chai::assert.deepEqual(IPAddress::parse("176.17.1.1/8").unwrap().dns_rev_domains(),
              {"176.in-addr.arpa"});
          Chai::assert.deepEqual(IPAddress::parse("177.17.1.1/0").unwrap().dns_rev_domains(),
              {"in-addr.arpa"});
          Chai::assert.deepEqual(IPAddress::parse("178.17.1.1/32").unwrap().dns_rev_domains(),
              {"1.1.17.178.in-addr.arpa"});
      }
      );
      it("test_method_compare", []() {
          auto ip1 = IPAddress::parse("10.1.1.1/8").unwrap();
          auto ip2 = IPAddress::parse("10.1.1.1/16").unwrap();
          auto ip3 = IPAddress::parse("172.16.1.1/14").unwrap();
          auto ip4 = IPAddress::parse("10.1.1.1/8").unwrap();

          // ip2 should be greater than ip1
          Chai::assert.equal(true, ip1.lt(ip2));
          Chai::assert.equal(false, ip1.gt(ip2));
          Chai::assert.equal(false, ip2.lt(ip1));
          // ip2 should be less than ip3
          Chai::assert.equal(true, ip2.lt(ip3));
          Chai::assert.equal(false, ip2.gt(ip3));
          // ip1 should be less than ip3
          Chai::assert.equal(true, ip1.lt(ip3));
          Chai::assert.equal(false, ip1.gt(ip3));
          Chai::assert.equal(false, ip3.lt(ip1));
          // ip1 should be equal to itself
          Chai::assert.equal(true, ip1.eq(ip1));
          // ip1 should be equal to ip4
          Chai::assert.equal(true, ip1.eq(ip4));
          // test sorting
          std::vector<IPAddress> res = { ip1, ip2, ip3 } ;
          std::sort (res.begin(), res.end(), [](const IPAddress &a, const IPAddress &b) {
            return a.lt(b);
          });
          Chai::assert.deepEqual(IPAddress::to_string_vec(res), {"10.1.1.1/8", "10.1.1.1/16", "172.16.1.1/14"});
          // test same prefix
          ip1 = IPAddress::parse("10.0.0.0/24").unwrap();
          ip2 = IPAddress::parse("10.0.0.0/16").unwrap();
          ip3 = IPAddress::parse("10.0.0.0/8").unwrap();
          {
            std::vector<IPAddress> res = { ip1, ip2, ip3 } ;
            std::sort (res.begin(), res.end(), [](const IPAddress &a, const IPAddress &b) {
              return a.lt(b);
            });
            Chai::assert.deepEqual(IPAddress::to_string_vec(res), {"10.0.0.0/8", "10.0.0.0/16", "10.0.0.0/24"});
          }
      }
      );
      it("test_method_minus", []() {
          auto ip1 = IPAddress::parse("10.1.1.1/8").unwrap();
          auto ip2 = IPAddress::parse("10.1.1.10/8").unwrap();
          Chai::assert.equal("9", ip2.sub(ip1).toString());
          Chai::assert.equal("9", ip1.sub(ip2).toString());
          }
        );
      it("test_method_plus", []() {
          auto ip1 = IPAddress::parse("172.16.10.1/24").unwrap();
          auto ip2 = IPAddress::parse("172.16.11.2/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(ip1.add(ip2)), {"172.16.10.0/23"});

          ip2 = IPAddress::parse("172.16.12.2/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(ip1.add(ip2)),
              {ip1.network().to_string(), ip2.network().to_string()});

          ip1 = IPAddress::parse("10.0.0.0/23").unwrap();
          ip2 = IPAddress::parse("10.0.2.0/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(ip1.add(ip2)),
              {"10.0.0.0/23", "10.0.2.0/24"});

          ip1 = IPAddress::parse("10.0.0.0/23").unwrap();
          ip2 = IPAddress::parse("10.0.2.0/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(ip1.add(ip2)),
              {"10.0.0.0/23", "10.0.2.0/24"});

          ip1 = IPAddress::parse("10.0.0.0/16").unwrap();
          ip2 = IPAddress::parse("10.0.2.0/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(ip1.add(ip2)), {"10.0.0.0/16"});

          ip1 = IPAddress::parse("10.0.0.0/23").unwrap();
          ip2 = IPAddress::parse("10.1.0.0/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(ip1.add(ip2)),
              {"10.0.0.0/23", "10.1.0.0/24"});
      }
      );
      it("test_method_netmask_equal", []() {
          auto ip = IPAddress::parse("10.1.1.1/16").unwrap();
          Chai::assert.equal(16, ip.prefix.get_prefix());
          auto ip2 = ip.change_netmask("255.255.255.0").unwrap();
          Chai::assert.equal(24, ip2.prefix.get_prefix());
          }
        );
      it("test_method_split", []() {
          Chai::assert.isTrue(setup().ip.split(0).isErr());
          Chai::assert.isTrue(setup().ip.split(257).isErr());

          Chai::assert.deepEqual(IPAddress::to_string_vec(setup().ip.split(1).unwrap()),
            {setup().ip.network().to_string()});

          Chai::assert.deepEqual(IPAddress::to_string_vec(setup().network.split(8).unwrap()),
              {"172.16.10.0/27",
              "172.16.10.32/27",
              "172.16.10.64/27",
              "172.16.10.96/27",
              "172.16.10.128/27",
              "172.16.10.160/27",
              "172.16.10.192/27",
              "172.16.10.224/27"});

          Chai::assert.deepEqual(IPAddress::to_string_vec(setup().network.split(7).unwrap()),
              {"172.16.10.0/27",
              "172.16.10.32/27",
              "172.16.10.64/27",
              "172.16.10.96/27",
              "172.16.10.128/27",
              "172.16.10.160/27",
              "172.16.10.192/26"});

          Chai::assert.deepEqual(IPAddress::to_string_vec(setup().network.split(6).unwrap()),
              {"172.16.10.0/27",
              "172.16.10.32/27",
              "172.16.10.64/27",
              "172.16.10.96/27",
              "172.16.10.128/26",
              "172.16.10.192/26"});
          Chai::assert.deepEqual(IPAddress::to_string_vec(setup().network.split(5).unwrap()),
              {"172.16.10.0/27",
              "172.16.10.32/27",
              "172.16.10.64/27",
              "172.16.10.96/27",
              "172.16.10.128/25"});
          Chai::assert.deepEqual(IPAddress::to_string_vec(setup().network.split(4).unwrap()),
              {"172.16.10.0/26", "172.16.10.64/26", "172.16.10.128/26", "172.16.10.192/26"});
          Chai::assert.deepEqual(IPAddress::to_string_vec(setup().network.split(3).unwrap()),
              {"172.16.10.0/26", "172.16.10.64/26", "172.16.10.128/25"});
          Chai::assert.deepEqual(IPAddress::to_string_vec(setup().network.split(2).unwrap()),
              {"172.16.10.0/25", "172.16.10.128/25"});
          Chai::assert.deepEqual(IPAddress::to_string_vec(setup().network.split(1).unwrap()),
              {"172.16.10.0/24"});
      }
      );
      it("test_method_subnet", []() {
          Chai::assert.isTrue(setup().network.subnet(23).isErr());
          Chai::assert.isTrue(setup().network.subnet(33).isErr());
          Chai::assert.isTrue(setup().ip.subnet(30).isOk());
          Chai::assert.deepEqual(IPAddress::to_string_vec(setup().network.subnet(26).unwrap()),
              {"172.16.10.0/26",
              "172.16.10.64/26",
              "172.16.10.128/26",
              "172.16.10.192/26"});
          Chai::assert.deepEqual(IPAddress::to_string_vec(setup().network.subnet(25).unwrap()),
              {"172.16.10.0/25", "172.16.10.128/25"});
          Chai::assert.deepEqual(IPAddress::to_string_vec(setup().network.subnet(24).unwrap()),
              {"172.16.10.0/24"});
          }
        );
      it("test_method_supernet", []() {
          Chai::assert.isTrue(setup().ip.supernet(24).isErr());
          Chai::assert.equal("0.0.0.0/0", setup().ip.supernet(0).unwrap().to_string());
          // Chai::assert.equal("0.0.0.0/0", setup().ip.supernet(-2).to_string());
          Chai::assert.equal("172.16.10.0/23",
              setup().ip.supernet(23).unwrap().to_string());
          Chai::assert.equal("172.16.8.0/22",
              setup().ip.supernet(22).unwrap().to_string());
          }
        );
      it("test_classmethod_parse_u32", []() {
          for (auto i : setup().decimal_values) {
          auto addr = i.first;
          auto my = i.second;
          auto ip = Ipv4::from_number(my, 32).unwrap();
          auto splitted = IPAddress::split_at_slash(addr);
          auto num = IPAddress::parse_dec_str(splitted.unwrap().netmask.unwrap()).unwrap();
          auto ip2 = ip.change_prefix(num).unwrap();
          Chai::assert.equal(ip2.to_string(), addr);
          }
          });
      it("test_classmethod_summarize", []() {
          // Should return self if only one network given
          Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize({setup().ip})),
              {setup().ip.network().to_string()});

          // Summarize homogeneous networks
          auto ip1 = IPAddress::parse("172.16.10.1/24").unwrap();
          auto ip2 = IPAddress::parse("172.16.11.2/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize({ip1, ip2})),
              {"172.16.10.0/23"});

          {
          auto ip1 = IPAddress::parse("10.0.0.1/24").unwrap();
          auto ip2 = IPAddress::parse("10.0.1.1/24").unwrap();
          auto ip3 = IPAddress::parse("10.0.2.1/24").unwrap();
          auto ip4 = IPAddress::parse("10.0.3.1/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize({ip1, ip2, ip3, ip4})),
                {"10.0.0.0/22"});
          }
          {
          auto ip1 = IPAddress::parse("10.0.0.1/24").unwrap();
          auto ip2 = IPAddress::parse("10.0.1.1/24").unwrap();
          auto ip3 = IPAddress::parse("10.0.2.1/24").unwrap();
          auto ip4 = IPAddress::parse("10.0.3.1/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize({ip4, ip3, ip2, ip1})),
              {"10.0.0.0/22"});
          }

          // Summarize non homogeneous networks
          ip1 = IPAddress::parse("10.0.0.0/23").unwrap();
          ip2 = IPAddress::parse("10.0.2.0/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize({ip1, ip2})),
            {"10.0.0.0/23", "10.0.2.0/24"});

          ip1 = IPAddress::parse("10.0.0.0/16").unwrap();
          ip2 = IPAddress::parse("10.0.2.0/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize({ip1, ip2})),
            {"10.0.0.0/16"});

          ip1 = IPAddress::parse("10.0.0.0/23").unwrap();
          ip2 = IPAddress::parse("10.1.0.0/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize({ip1, ip2})),
              {"10.0.0.0/23", "10.1.0.0/24"});

          ip1 = IPAddress::parse("10.0.0.0/23").unwrap();
          ip2 = IPAddress::parse("10.0.2.0/23").unwrap();
          auto ip3 = IPAddress::parse("10.0.4.0/24").unwrap();
          auto ip4 = IPAddress::parse("10.0.6.0/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize({ip1, ip2, ip3, ip4})),
              {"10.0.0.0/22", "10.0.4.0/24", "10.0.6.0/24"});
          {
            auto ip1 = IPAddress::parse("10.0.1.1/24").unwrap();
            auto ip2 = IPAddress::parse("10.0.2.1/24").unwrap();
            auto ip3 = IPAddress::parse("10.0.3.1/24").unwrap();
            auto ip4 = IPAddress::parse("10.0.4.1/24").unwrap();
            Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize({ip1, ip2, ip3, ip4})),
                {"10.0.1.0/24", "10.0.2.0/23", "10.0.4.0/24"});
          }
          {
            auto ip1 = IPAddress::parse("10.0.1.1/24").unwrap();
            auto ip2 = IPAddress::parse("10.0.2.1/24").unwrap();
            auto ip3 = IPAddress::parse("10.0.3.1/24").unwrap();
            auto ip4 = IPAddress::parse("10.0.4.1/24").unwrap();
            Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize({ip4, ip3, ip2, ip1})),
                {"10.0.1.0/24", "10.0.2.0/23", "10.0.4.0/24"});
          }

          ip1 = IPAddress::parse("10.0.1.1/24").unwrap();
          ip2 = IPAddress::parse("10.10.2.1/24").unwrap();
          ip3 = IPAddress::parse("172.16.0.1/24").unwrap();
          ip4 = IPAddress::parse("172.16.1.1/24").unwrap();
          Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize({ip1, ip2, ip3, ip4})),
              {"10.0.1.0/24", "10.10.2.0/24", "172.16.0.0/23"});

          auto ips = {IPAddress::parse("10.0.0.12/30").unwrap(),
               IPAddress::parse("10.0.100.0/24").unwrap()};
          Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize(ips)),
              {"10.0.0.12/30", "10.0.100.0/24"});

          ips = {IPAddress::parse("172.16.0.0/31").unwrap(),
              IPAddress::parse("10.10.2.1/32").unwrap()};
          Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize(ips)),
              {"10.10.2.1/32", "172.16.0.0/31"});

          ips = {IPAddress::parse("172.16.0.0/32").unwrap(),
              IPAddress::parse("10.10.2.1/32").unwrap()};
          Chai::assert.deepEqual(IPAddress::to_string_vec(IPAddress::summarize(ips)),
              {"10.10.2.1/32", "172.16.0.0/32"});
      });

      it("test_classmethod_parse_classful", []() {
          for (auto i : setup().classful) {
          auto ip = i.first;
          auto prefix = i.second;
          auto res = Ipv4::parse_classful(ip).unwrap();
          Chai::assert.equal(prefix, res.prefix.num);
          std::stringstream s2;
          s2 << ip << "/" << prefix;
          Chai::assert.equal(s2.str(), res.to_string());
          }
          Chai::assert.isTrue(Ipv4::parse_classful("192.168.256.257").isErr());
          });
  });
}
