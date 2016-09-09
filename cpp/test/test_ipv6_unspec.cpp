#include "chai.hpp"
#include "mocha.hpp"

#include "../src/crunchy.hpp"
#include "../src/ipaddress.hpp"
#include "../src/ipv6_unspec.hpp"
#include "../src/prefix128.hpp"

class IPv6UnspecifiedTest {
public:
  IPAddress ip = Ipv6Unspec::create();
  const char *to_s = "::";
  const char *to_string = "::/128";
  const char *to_string_uncompressed =
      "0000:0000:0000:0000:0000:0000:0000:0000/128";
  Crunchy num = Crunchy::zero();
};

IPv6UnspecifiedTest setup() { return IPv6UnspecifiedTest(); }

int main() {
  describe("ipv6_unspec", []() {
    it("test_attributes", []() {
      Chai::assert.isTrue(setup().ip.host_address.eq(setup().num));
      Chai::assert.equal(128, setup().ip.prefix.num);
      Chai::assert.equal(true, setup().ip.is_unspecified());
      Chai::assert.equal(setup().to_s, setup().ip.to_s());
      Chai::assert.equal(setup().to_string, setup().ip.to_string());
      Chai::assert.equal(setup().to_string_uncompressed,
                         setup().ip.to_string_uncompressed());
    });
    it("test_method_ipv6", []() {
      Chai::assert.equal(true, setup().ip.is_ipv6());
    });
  });
}
