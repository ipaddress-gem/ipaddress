
#include "chai.hpp"
#include "mocha.hpp"

#include "../src/prefix32.hpp"
#include "../src/ipaddress.hpp"
#include "../src/ipv4.hpp"
#include "../src/crunchy.hpp"

class Prefix32Test {
public:
    const char *netmask0 = "0.0.0.0";
    const char *netmask8 = "255.0.0.0";
    const char *netmask16 = "255.255.0.0";
    const char *netmask24 = "255.255.255.0";
    const char *netmask30 = "255.255.255.252";
    std::vector<std::string> netmasks;
    std::vector<std::pair<std::string, size_t>> prefix_hash;
    std::vector<std::pair<std::vector<size_t>, size_t>> octets_hash;
    std::vector<std::pair<size_t, Crunchy>> u32_hash;
};

    Prefix32Test setup() {
        Prefix32Test p32t;
        p32t.netmasks.push_back(p32t.netmask0);
        p32t.netmasks.push_back(p32t.netmask8);
        p32t.netmasks.push_back(p32t.netmask16);
        p32t.netmasks.push_back(p32t.netmask24);
        p32t.netmasks.push_back(p32t.netmask30);
        p32t.prefix_hash.push_back({"0.0.0.0", 0});
        p32t.prefix_hash.push_back({"255.0.0.0", 8});
        p32t.prefix_hash.push_back({"255.255.0.0", 16});
        p32t.prefix_hash.push_back({"255.255.255.0", 24});
        p32t.prefix_hash.push_back({"255.255.255.252", 30});
        p32t.octets_hash.push_back({{0, 0, 0, 0}, 0});
        p32t.octets_hash.push_back({{255, 0, 0, 0}, 8});
        p32t.octets_hash.push_back({{255, 255, 0, 0}, 16});
        p32t.octets_hash.push_back({{255, 255, 255, 0}, 24});
        p32t.octets_hash.push_back({{255, 255, 255, 252}, 30});
        p32t.u32_hash.push_back({0, Crunchy::zero()});
        p32t.u32_hash.push_back({8, Crunchy::parse("4278190080").unwrap()});
        p32t.u32_hash.push_back({16, Crunchy::parse("4294901760").unwrap()});
        p32t.u32_hash.push_back({24, Crunchy::parse("4294967040").unwrap()});
        p32t.u32_hash.push_back({30, Crunchy::parse("4294967292").unwrap()});
        return p32t;
    }

int main() {
describe("prefix32", []() {
    it("test_attributes", []() {
        for (auto e : setup().prefix_hash) {
            auto prefix = Prefix32::create(e.second).unwrap();
            Chai::assert.equal(e.second, prefix.num);
        }
    });

    it("test_parse_netmask_to_prefix", []() {
        for (auto e : setup().prefix_hash) {
            auto netmask = e.first;
            auto num = e.second;
            // console.log(e);
            auto prefix = IPAddress::parse_netmask_to_prefix(netmask).unwrap();
            std::cout << netmask << ":" << num << ":" << prefix;
            Chai::assert.equal(num, prefix);
        }
    });
    it("test_method_to_ip", []() {
        for (auto hash : setup().prefix_hash) {
            auto netmask = hash.first;
            auto num = hash.second;
            auto prefix = Prefix32::create(num).unwrap();
            Chai::assert.equal(netmask, prefix.to_ip_str());
        }
    });
    it("test_method_to_s", []() {
        auto prefix = Prefix32::create(8).unwrap();
        Chai::assert.equal("8", prefix.to_s());
    });
    it("test_method_bits", []() {
        auto prefix = Prefix32::create(16).unwrap();
        Chai::assert.equal("11111111111111110000000000000000", prefix.bits());
    });
    it("test_method_to_u32", []() {
        for (auto i : setup().u32_hash) {
            auto num = i.first;
            auto ip32 = i.second;
            Chai::assert.isTrue(ip32.eq(Prefix32::create(num).unwrap().netmask()));
        }
    });
    it("test_method_plus", []() {
        auto p1 = Prefix32::create(8).unwrap();
        auto p2 = Prefix32::create(10).unwrap();
        Chai::assert.equal(18, p1.add_prefix(p2).unwrap().get_prefix());
        Chai::assert.equal(12, p1.add(4).unwrap().get_prefix());
    });
    it("test_method_minus", []() {
        auto p1 = Prefix32::create(8).unwrap();
        auto p2 = Prefix32::create(24).unwrap();
        Chai::assert.equal(16, p1.sub_prefix(p2).unwrap().get_prefix());
        Chai::assert.equal(16, p2.sub_prefix(p1).unwrap().get_prefix());
        Chai::assert.equal(20, p2.sub(4).unwrap().get_prefix());
    });
    it("test_initialize", []() {
        Chai::assert.isTrue(Prefix32::create(33).isErr());
        Chai::assert.isTrue(Prefix32::create(8).isOk());
    });
    it("test_method_octets", []() {
        for (auto i : setup().octets_hash) {
            auto arr = i.first;
            auto pref = i.second;
            auto prefix = Prefix32::create(pref).unwrap();
            Chai::assert.deepEqual(prefix.ip_bits.parts(prefix.netmask()), arr);
        }
    });
    it("test_method_brackets", []() {
        for (auto i : setup().octets_hash) {
            auto arr = i.first;
            auto pref = i.second;
            auto prefix = Prefix32::create(pref).unwrap();
            for (size_t index=0; index < arr.size(); ++index) {
                // console.log("xxxx", prefix.netmask());
                Chai::assert.equal(prefix.ip_bits.parts(prefix.netmask())[index], arr[index]);
            }
        }
    });
    it("test_method_hostmask", []() {
        auto prefix = Prefix32::create(8).unwrap();
        // console.log(">>>>", prefix.host_mask());
        Chai::assert.equal("0.255.255.255", Ipv4::from_number(prefix.host_mask(), 0).unwrap().to_s());
    });
});
  return exit();
}
