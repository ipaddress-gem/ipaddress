#include "chai.hpp"
#include "mocha.hpp"

#include "../src/crunchy.hpp"
#include "../src/ipaddress.hpp"
#include "../src/ipv6_mapped.hpp"
#include "../src/ipv6_unspec.hpp"
#include "../src/prefix128.hpp"

class IPv6MappedTest {
public:
  IPAddress ip = Ipv6Mapped::create("::172.16.10.1").unwrap();
  const char *s = "::ffff:172.16.10.1";
  const char *sstr = "::ffff:172.16.10.1/32";
  const char *string = "0000:0000:0000:0000:0000:ffff:ac10:0a01/128";
  Crunchy u128 = Crunchy::parse("281473568475649").unwrap();
  const char *address = "::ffff:ac10:a01/128";

  std::vector<std::pair<std::string, Crunchy>> valid_mapped;
  std::vector<std::pair<std::string, Crunchy>> valid_mapped_ipv6;
  std::vector<std::pair<std::string, std::string>> valid_mapped_ipv6_conversion;
};

IPv6MappedTest setup() {
  IPv6MappedTest ipv6;
  ipv6.valid_mapped.push_back(
      {"::13.1.68.3", Crunchy::parse("281470899930115").unwrap()});
  ipv6.valid_mapped.push_back({"0:0:0:0:0:ffff:129.144.52.38",
                               Crunchy::parse("281472855454758").unwrap()});
  ipv6.valid_mapped.push_back(
      {"::ffff:129.144.52.38", Crunchy::parse("281472855454758").unwrap()});
  ipv6.valid_mapped_ipv6.push_back(
      {"::ffff:13.1.68.3", Crunchy::parse("281470899930115").unwrap()});
  ipv6.valid_mapped_ipv6.push_back(
      {"0:0:0:0:0:ffff:8190:3426", Crunchy::parse("281472855454758").unwrap()});
  ipv6.valid_mapped_ipv6.push_back(
      {"::ffff:8190:3426", Crunchy::parse("281472855454758").unwrap()});
  ipv6.valid_mapped_ipv6_conversion.push_back(
      {"::ffff:13.1.68.3", "13.1.68.3"});
  ipv6.valid_mapped_ipv6_conversion.push_back(
      {"0:0:0:0:0:ffff:8190:3426", "129.144.52.38"});
  ipv6.valid_mapped_ipv6_conversion.push_back(
      {"::ffff:8190:3426", "129.144.52.38"});
  return ipv6;
}

int main() {
  describe("ipv6_mapped", []() {
    it("test_initialize", []() {
      auto s = setup();
      Chai::assert.isTrue(IPAddress::parse("::172.16.10.1").isOk());
      for (auto i : s.valid_mapped) {
            auto ip = i.first;
            auto u128 = i.second;
            // println("-{}--{}", ip, u128);
            Chai::assert.isTrue(IPAddress::parse(ip).isOk());
            Chai::assert.equal(
                u128.toString(),
                IPAddress::parse(ip).unwrap().host_address.toString());
      }
      for (auto i : s.valid_mapped_ipv6) {
        auto ip = i.first;
        auto u128 = i.second;
        // println("===={}=={:x}", ip, u128);
        Chai::assert.isTrue(IPAddress::parse(ip).isOk());
        Chai::assert.equal(
            u128.toString(),
            IPAddress::parse(ip).unwrap().host_address.toString());
      }
    });
    it("test_mapped_from_ipv6_conversion", []() {
      for (auto i : setup().valid_mapped_ipv6_conversion) {
        auto ip6 = i.first;
        auto ip4 = i.second;
        Chai::assert.equal(ip4, IPAddress::parse(ip6).unwrap().mapped.unwrap().to_s());
      }
    });
    it("test_attributes", []() {
      auto s = setup();
      Chai::assert.equal(s.address, s.ip.to_string());
      Chai::assert.equal(128, s.ip.prefix.num);
      Chai::assert.equal(s.s, s.ip.to_s_mapped());
      Chai::assert.equal(s.sstr, s.ip.to_string_mapped());
      Chai::assert.equal(s.string, s.ip.to_string_uncompressed());
      Chai::assert.equal(s.u128.toString(), s.ip.host_address.toString());
    });
    it("test_method_ipv6", []() { Chai::assert.isTrue(setup().ip.is_ipv6()); });
    it("test_mapped", []() { Chai::assert.isTrue(setup().ip.is_mapped()); });
  });
}
