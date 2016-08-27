
import { assert } from 'chai';

import Prefix32 from '../src/prefix32';
import IPAddress from '../src/ipaddress';

   class Prefix32Test {
        netmask0: string = "0.0.0.0";
        netmask8: string = "255.0.0.0";
        netmask16: string = "255.255.0.0";
        netmask24: string = "255.255.255.0";
        netmask30: string = "255.255.255.252";
        netmasks: string[] = [];
        prefix_hash: [] = [];
        octets_hash: [] = [];
        u32_hash: [] = [];
    }

describe("prefix32", () => {
    function setup() : Prefix32Test {
        let p32t = new Prefix32Test()
        p32t.netmasks.push(p32t.netmask0);
        p32t.netmasks.push(p32t.netmask8);
        p32t.netmasks.push(p32t.netmask16);
        p32t.netmasks.push(p32t.netmask24);
        p32t.netmasks.push(p32t.netmask30);
        p32t.prefix_hash.push(["0.0.0.0", 0]);
        p32t.prefix_hash.push(["255.0.0.0", 8]);
        p32t.prefix_hash.push(["255.255.0.0", 16]);
        p32t.prefix_hash.push(["255.255.255.0", 24]);
        p32t.prefix_hash.push(["255.255.255.252", 30]);
        p32t.octets_hash.push([[0, 0, 0, 0], 0]);
        p32t.octets_hash.push([[255, 0, 0, 0], 8]);
        p32t.octets_hash.push([[255, 255, 0, 0], 16]);
        p32t.octets_hash.push([[255, 255, 255, 0], 24]);
        p32t.octets_hash.push([[255, 255, 255, 252], 30]);
        p32t.u32_hash.push([0, 0]);
        p32t.u32_hash.push([8, 4278190080]);
        p32t.u32_hash.push([16, 4294901760]);
        p32t.u32_hash.push([24, 4294967040]);
        p32t.u32_hash.push([30, 4294967292]);
        return p32t;
    }

    it("test_attributes", () => {
        for (let e of setup().prefix_hash.values()) {
            let prefix = Prefix32.create(e[0]);
            assert.equal(e[0], prefix.num);
        }
    });

    it("test_parse_netmask_to_prefix", () => {
        for (let e of setup().prefix_hash) {
            let netmask = e[0];
            let num = e[1];
            let prefix = IPAddress.parse_netmask_to_prefix(netmask).unwrap();
            assert.equal(num, prefix);
        }
    });
    it("test_method_to_ip", () => {
        for (netmask, num) in setup().prefix_hash {
            let netmask = hash[0];
            let num = hash[1];
            let prefix = prefix32::new(num).unwrap();
            assert.equal(*netmask, prefix.to_ip_str())
        }
    });
    it("test_method_to_s", () => {
        let prefix = prefix32::new(8).unwrap();
        assert.equal("8", prefix.to_s())
    });
    it("test_method_bits", () => {
        let prefix = prefix32::new(16).unwrap();
        assert.equal("11111111111111110000000000000000", prefix.bits())
    });
    it("test_method_to_u32", () => {
        for (num, ip32) in setup().u32_hash {
            assert.equal(ip32,
                       prefix32::new(num).unwrap().netmask().to_u32().unwrap())
        }
    });
    it("test_method_plus", () => {
        let p1 = prefix32::new(8).unwrap();
        let p2 = prefix32::new(10).unwrap();
        assert.equal(18, p1.add_prefix(&p2).unwrap().num);
        assert.equal(12, p1.add(4).unwrap().num)
    });
    it("test_method_minus", () => {
        let p1 = prefix32::new(8).unwrap();
        let p2 = prefix32::new(24).unwrap();
        assert.equal(16, p1.sub_prefix(&p2).unwrap().num);
        assert.equal(16, p2.sub_prefix(&p1).unwrap().num);
        assert.equal(20, p2.sub(4).unwrap().num);
    });
    it("test_initialize", () => {
        assert!(prefix32::new(33).is_err());
        assert!(prefix32::new(8).is_ok());
    });
    it("test_method_octets", () => {
        for (arr, pref) in setup().octets_hash {
            let prefix = prefix32::new(pref).unwrap();
            assert.equal(prefix.ip_bits.parts(&prefix.netmask()), arr);
        }
    });
    it("test_method_brackets", () => {
        for (arr, pref) in setup().octets_hash {
            let prefix = prefix32::new(pref).unwrap();
            for index in 0..arr.len() {
                let oct = arr.get(index);
                assert.equal(prefix.ip_bits.parts(&prefix.netmask()).get(index), oct)
            }
        }
    });
    it("test_method_hostmask", () => {
        let prefix = prefix32::new(8).unwrap();
        assert.equal("0.255.255.255",
                   ipv4::from_u32(prefix.host_mask().to_u32().unwrap(), 0).unwrap().to_s());
    });
});
