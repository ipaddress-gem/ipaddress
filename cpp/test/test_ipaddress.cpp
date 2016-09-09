
#include "chai.hpp"
#include "mocha.hpp"

#include "../src/crunchy.hpp"
#include "../src/ipaddress.hpp"
#include "../src/prefix32.hpp"

class IPAddressTest {
public:
  const char *valid_ipv4 = "172.16.10.1/24";
  const char *valid_ipv6 = "2001:db8::8:800:200c:417a/64";
  const char *valid_mapped = "::13.1.68.3";
  const char *invalid_ipv4 = "10.0.0.256";
  const char *invalid_ipv6 = ":1:2:3:4:5:6:7";
  const char *invalid_mapped = "::1:2.3.4";
};

typedef struct S_RangeItem {
  size_t start;
  size_t target;
} RangeItem;

RangeItem Range(size_t start, size_t target) {
  RangeItem ri = {start, target};
  return ri;
}

void assertArrayEqual(std::vector<std::string> a, std::vector<std::string> b) {
  Chai::assert.deepEqual(a, b);
}

IPAddressTest setup() { return IPAddressTest(); }

int main() {
  describe("IPAddressTest", []() {


    it("test_method_ipaddress", []() {
        std::cout << "-1" << std::endl;
      Chai::assert.isTrue(IPAddress::parse(setup().valid_ipv4).isOk(), "valid_ipv4");
        std::cout << "-2" << std::endl;
      Chai::assert.isTrue(IPAddress::parse(setup().valid_ipv6).isOk(), "valid_ipv6");
        std::cout << "-3" << IPAddress::parse(setup().valid_mapped).text() << std::endl;
      Chai::assert.isTrue(IPAddress::parse(setup().valid_mapped).isOk(), "valid_mapped");
        std::cout << "-4" << std::endl;

      Chai::assert.isTrue(IPAddress::parse(setup().valid_ipv4).unwrap().is_ipv4(), "isv4");
        std::cout << "-5" << std::endl;
      Chai::assert.isTrue(IPAddress::parse(setup().valid_ipv6).unwrap().is_ipv6(), "isv6");
        std::cout << "-6" << std::endl;
      Chai::assert.isTrue(IPAddress::parse(setup().valid_mapped).unwrap().is_mapped(), "ismapped");
        std::cout << "-7" << std::endl;

      Chai::assert.isTrue(IPAddress::parse(setup().invalid_ipv4).isErr(), "invalid_ipv4");
        std::cout << "-8" << std::endl;
      Chai::assert.isTrue(IPAddress::parse(setup().invalid_ipv6).isErr(), "invalid_ipv6");
        std::cout << "-9" << std::endl;
      Chai::assert.isTrue(IPAddress::parse(setup().invalid_mapped).isErr(), "invalid_mapped");
        std::cout << "-A" << std::endl;
    });

    it("test_module_method_valid", []() {
      Chai::assert.equal(true, IPAddress::is_valid("10.0.0.1"));
      Chai::assert.equal(true, IPAddress::is_valid("10.0.0.0"));
      Chai::assert.equal(true, IPAddress::is_valid("2002::1"));
      Chai::assert.equal(true, IPAddress::is_valid("dead:beef:cafe:babe::f0ad"));
      Chai::assert.equal(false, IPAddress::is_valid("10.0.0.256"));
      Chai::assert.equal(false, IPAddress::is_valid("10.0.0.0.0"));
      Chai::assert.equal(true, IPAddress::is_valid("10.0.0"));
      Chai::assert.equal(true, IPAddress::is_valid("10.0"));
      Chai::assert.equal(false, IPAddress::is_valid("2002:516:2:200"));
      Chai::assert.equal(false, IPAddress::is_valid("2002:::1"));
    });

    it("test_module_method_valid_ipv4_netmark", []() {
      Chai::assert.equal(true, IPAddress::is_valid_netmask("255.255.255.0"), "255.255.255.0 should ok");
      Chai::assert.equal(false, IPAddress::is_valid_netmask("10.0.0.1"), "10.0.0.1 this is bad");
    });

    it("test_summarize", []() {
      std::vector<std::string> netstr;
      for (auto range : {Range(1, 10), Range(11, 127), Range(128, 169),
                        Range(170, 172), Range(173, 192), Range(193, 224)}) {
        for (auto i = range.start; i < range.target; ++i) {
          std::stringstream s2;
          s2 << i << ".0.0.0/8";
          netstr.push_back(s2.str());
        }
      }
      for (auto i = 0; i < 256; ++i) {
        if (i != 254) {
          std::stringstream s2;
          s2 << "169." << i << ".0.0/16";
          netstr.push_back(s2.str());
        }
      }
      for (auto i = 0; i < 256; ++i) {
        if (i < 16 || 31 < i) {
          std::stringstream s2;
          s2 << "172." << i << ".0.0/16";
          netstr.push_back(s2.str());
        }
      }
      for (auto i = 0; i < 256; ++i) {
        if (i != 168) {
          std::stringstream s2;
          s2 << "192." << i << ".0.0/16";
          netstr.push_back(s2.str());
        }
      }
      std::vector<IPAddress> ip_addresses;
      for (auto net : netstr) {
        ip_addresses.push_back(IPAddress::parse(net).unwrap());
      }

      Chai::assert.equal(IPAddress::summarize_str({}).unwrap().size(), 0);
      assertArrayEqual(
          IPAddress::to_string_vec(IPAddress::summarize_str({"10.1.0.4/24"}).unwrap()),
          {"10.1.0.0/24"});
      assertArrayEqual(
          IPAddress::to_string_vec(IPAddress::summarize_str({"2000:1::4711/32"}).unwrap()),
          {"2000:1::/32"});

      assertArrayEqual(IPAddress::to_string_vec(IPAddress::summarize_str(
              { "10.1.0.4/24", "7.0.0.0/0", "1.2.3.4/4" }).unwrap()),
              {"0.0.0.0/0"});
      assertArrayEqual(IPAddress::to_string_vec(IPAddress::summarize_str({
        "2000:1::/32", "3000:1::/32", "2000:2::/32", "2000:3::/32",
        "2000:4::/32", "2000:5::/32", "2000:6::/32", "2000:7::/32",
        "2000:8::/32"
      }).unwrap()),
          {
                         "2000:1::/32", "2000:2::/31", "2000:4::/30",
                         "2000:8::/32", "3000:1::/32"
                         });

      assertArrayEqual(IPAddress::to_string_vec(IPAddress::summarize_str({
        "10.0.1.1/24", "30.0.1.0/16", "10.0.2.0/24", "10.0.3.0/24",
        "10.0.4.0/24", "10.0.5.0/24", "10.0.6.0/24", "10.0.7.0/24",
        "10.0.8.0/24"
      }).unwrap()),
          {
                         "10.0.1.0/24", "10.0.2.0/23", "10.0.4.0/22",
                         "10.0.8.0/24", "30.0.0.0/16"
                         });

      assertArrayEqual(IPAddress::to_string_vec(IPAddress::summarize_str(
              { "10.0.0.0/23", "10.0.2.0/24" }).unwrap()),
          { "10.0.0.0/23", "10.0.2.0/24" });
      assertArrayEqual(IPAddress::to_string_vec(IPAddress::summarize_str(
              { "10.0.0.0/24", "10.0.1.0/24", "10.0.2.0/23" }).unwrap()),
          {"10.0.0.0/22"});

      assertArrayEqual(IPAddress::to_string_vec(IPAddress::summarize_str(
              { "10.0.0.0/16", "10.0.2.0/24" }).unwrap()),
          {"10.0.0.0/16"});

      size_t cnt = 100;
      for (size_t _ = 0; _ < cnt; ++_) {
        Chai::assert.deepEqual(
            IPAddress::to_string_vec(IPAddress::summarize(ip_addresses)), {
              "1.0.0.0/8",      "2.0.0.0/7",      "4.0.0.0/6",
              "8.0.0.0/7",      "11.0.0.0/8",     "12.0.0.0/6",
              "16.0.0.0/4",     "32.0.0.0/3",     "64.0.0.0/3",
              "96.0.0.0/4",     "112.0.0.0/5",    "120.0.0.0/6",
              "124.0.0.0/7",    "126.0.0.0/8",    "128.0.0.0/3",
              "160.0.0.0/5",    "168.0.0.0/8",    "169.0.0.0/9",
              "169.128.0.0/10", "169.192.0.0/11", "169.224.0.0/12",
              "169.240.0.0/13", "169.248.0.0/14", "169.252.0.0/15",
              "169.255.0.0/16", "170.0.0.0/7",    "172.0.0.0/12",
              "172.32.0.0/11",  "172.64.0.0/10",  "172.128.0.0/9",
              "173.0.0.0/8",    "174.0.0.0/7",    "176.0.0.0/4",
              "192.0.0.0/9",    "192.128.0.0/11", "192.160.0.0/13",
              "192.169.0.0/16", "192.170.0.0/15", "192.172.0.0/14",
              "192.176.0.0/12", "192.192.0.0/10", "193.0.0.0/8",
              "194.0.0.0/7",    "196.0.0.0/6",    "200.0.0.0/5",
              "208.0.0.0/4"
            });
      }
      // end
      // printer = RubyProf.GraphPrinter.new(result)
      // printer.print(STDOUT, {})
      // test imutable input parameters
      auto a1 = IPAddress::parse("10.0.0.1/24").unwrap();
      auto a2 = IPAddress::parse("10.0.1.1/24").unwrap();
      assertArrayEqual(IPAddress::to_string_vec(
                           IPAddress::summarize({ a1, a2 })),
                       {"10.0.0.0/23"});
      Chai::assert.equal("10.0.0.1/24", a1.to_string());
      Chai::assert.equal("10.0.1.1/24", a2.to_string());
    });
  });
}
