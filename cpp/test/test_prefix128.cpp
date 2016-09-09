#include "chai.hpp"
#include "mocha.hpp"

#include "../src/prefix128.hpp"
#include "../src/crunchy.hpp"

class Prefix128Test {
    public:
      std::vector<std::pair<size_t, Crunchy>> u128_hash;
};


    Prefix128Test setup() {
        Prefix128Test p128t;
        p128t.u128_hash.push_back({32, Crunchy::parse("340282366841710300949110269838224261120").unwrap()});
        p128t.u128_hash.push_back({64, Crunchy::parse("340282366920938463444927863358058659840").unwrap()});
        p128t.u128_hash.push_back({96, Crunchy::parse("340282366920938463463374607427473244160").unwrap()});
        p128t.u128_hash.push_back({126, Crunchy::parse("340282366920938463463374607431768211452").unwrap()});
        return p128t;
    }

int main() {
describe("prefix128", []() {
    it("test_initialize", []() {
        Chai::assert.isTrue(Prefix128::create(129).isErr());
        Chai::assert.isTrue(Prefix128::create(64).isOk());
    });

    it("test_method_bits", []() {
        auto prefix = Prefix128::create(64).unwrap();
        std::stringstream str;
        for (auto i = 0; i < 64; ++i) {
            str << "1";
        }
        for (auto i = 0; i < 64; ++i) {
            str << "0";
        }
        Chai::assert.equal(str.str(), prefix.bits());
    });
    it("test_method_to_u32", []() {
        for (auto hash : setup().u128_hash) {
            Chai::assert.isTrue(hash.second.eq(Prefix128::create(hash.first).unwrap().netmask()));
        }
    });
});
  return exit();
}
