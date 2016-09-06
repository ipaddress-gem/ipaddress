
import { assert } from 'chai';
import * as Mocha from 'mocha';

import Prefix32 from '../src/prefix32';
import IPAddress from '../src/ipaddress';
import Ipv4 from '../src/ipv4';
import Crunchy from '../src/crunchy';

class Prefix32Test {
    netmask0: string = "0.0.0.0";
    netmask8: string = "255.0.0.0";
    netmask16: string = "255.255.0.0";
    netmask24: string = "255.255.255.0";
    netmask30: string = "255.255.255.252";
    netmasks: string[] = [];
    prefix_hash: [string, number][] = [];
    octets_hash: [number[], number][] = [];
    u32_hash: [number, Crunchy][] = [];
}

describe("prefix32", () => {
    function assertArrayEqual(a: any[], b: any[]) : void {
        assert.equal(a.length, b.length, "length missmatch");
        for (let i = 0; i < a.length; ++i) {
            assert.equal(a[i], b[i]);
        }
    }
    function setup(): Prefix32Test {
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
        p32t.u32_hash.push([0, Crunchy.zero()]);
        p32t.u32_hash.push([8, Crunchy.parse("4278190080")]);
        p32t.u32_hash.push([16, Crunchy.parse("4294901760")]);
        p32t.u32_hash.push([24, Crunchy.parse("4294967040")]);
        p32t.u32_hash.push([30, Crunchy.parse("4294967292")]);
        return p32t;
    }

    it("test_attributes", () => {
        for (let e of setup().prefix_hash) {
            let prefix = Prefix32.create(e[1]);
            assert.equal(e[1], prefix.num);
        }
    });

    it("test_parse_netmask_to_prefix", () => {
        for (let e of setup().prefix_hash) {
            let netmask = e[0];
            let num = e[1];
            // console.log(e); 
            let prefix = IPAddress.parse_netmask_to_prefix(netmask);
            assert.equal(num, prefix);
        }
    });
    it("test_method_to_ip", () => {
        for (let hash of setup().prefix_hash) {
            let netmask = hash[0];
            let num = hash[1];
            let prefix = Prefix32.create(num);
            assert.equal(netmask, prefix.to_ip_str())
        }
    });
    it("test_method_to_s", () => {
        let prefix = Prefix32.create(8);
        assert.equal("8", prefix.to_s())
    });
    it("test_method_bits", () => {
        let prefix = Prefix32.create(16);
        assert.equal("11111111111111110000000000000000", prefix.bits())
    });
    it("test_method_to_u32", () => {
        for (let i of setup().u32_hash) {
            let num = i[0];
            let ip32 = i[1];
            assert.isTrue(ip32.eq(Prefix32.create(num).netmask()));
        }
    });
    it("test_method_plus", () => {
        let p1 = Prefix32.create(8);
        let p2 = Prefix32.create(10);
        assert.equal(18, p1.add_prefix(p2).num);
        assert.equal(12, p1.add(4).num)
    });
    it("test_method_minus", () => {
        let p1 = Prefix32.create(8);
        let p2 = Prefix32.create(24);
        assert.equal(16, p1.sub_prefix(p2).num);
        assert.equal(16, p2.sub_prefix(p1).num);
        assert.equal(20, p2.sub(4).num);
    });
    it("test_initialize", () => {
        assert.isNull(Prefix32.create(33));
        assert.isOk(Prefix32.create(8));
    });
    it("test_method_octets", () => {
        for (let i of setup().octets_hash) {
            let arr = i[0];
            let pref = i[1];
            let prefix = Prefix32.create(pref);
            assertArrayEqual(prefix.ip_bits.parts(prefix.netmask()), arr);
        }
    });
    it("test_method_brackets", () => {
        for (let i of setup().octets_hash) {
            let arr = i[0];
            let pref = i[1];
            let prefix = Prefix32.create(pref);
            for (let index=0; index < arr.length; ++index) {
                // console.log("xxxx", prefix.netmask());
                assert.equal(prefix.ip_bits.parts(prefix.netmask())[index], arr[index]);
            }
        }
    });
    it("test_method_hostmask", () => {
        let prefix = Prefix32.create(8);
        // console.log(">>>>", prefix.host_mask());
        assert.equal("0.255.255.255", Ipv4.from_number(prefix.host_mask(), 0).to_s());
    });
});
