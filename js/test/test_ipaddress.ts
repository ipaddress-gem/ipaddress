import { assert } from 'chai';
import * as Mocha from 'mocha';

import IPAddress from '../src/ipaddress';
import Prefix32 from '../src/prefix32';
import Crunchy from '../src/crunchy';

class IPAddressTest {
    valid_ipv4: string = "172.16.10.1/24";
    valid_ipv6: string = "2001:db8::8:800:200c:417a/64";
    valid_mapped: string = "::13.1.68.3";
    invalid_ipv4: string = "10.0.0.256";
    invalid_ipv6: string = ":1:2:3:4:5:6:7";
    invalid_mapped: string = "::1:2.3.4";
}

class RangeItem {
    start: number;
    target: number;
}
function Range(start: number, target: number) {
    let ri = new RangeItem();
    ri.start = start;
    ri.target = target;
    return ri;
}

describe('IPAddressTest', () => {
    function assertArrayEqual(a: any[], b: any[]): void {
        assert.equal(a.length, b.length, "length missmatch");
        for (let i = 0; i < a.length; ++i) {
            assert.equal(a[i], b[i]);
        }
    }


    function setup(): IPAddressTest {
        return new IPAddressTest();
    }

    it("test_method_ipaddress", () => {
        assert.isOk(IPAddress.parse(setup().valid_ipv4));
        assert.isOk(IPAddress.parse(setup().valid_ipv6));
        assert.isOk(IPAddress.parse(setup().valid_mapped));

        assert.isOk(IPAddress.parse(setup().valid_ipv4).is_ipv4());
        assert.isOk(IPAddress.parse(setup().valid_ipv6).is_ipv6());
        assert.isOk(IPAddress.parse(setup().valid_mapped).is_mapped());

        assert.isNull(IPAddress.parse(setup().invalid_ipv4));
        assert.isNull(IPAddress.parse(setup().invalid_ipv6));
        assert.isNull(IPAddress.parse(setup().invalid_mapped));
    });

    it("test_module_method_valid", () => {
        assert.equal(true, IPAddress.is_valid("10.0.0.1"));
        assert.equal(true, IPAddress.is_valid("10.0.0.0"));
        assert.equal(true, IPAddress.is_valid("2002::1"));
        assert.equal(true, IPAddress.is_valid("dead:beef:cafe:babe::f0ad"));
        assert.equal(false, IPAddress.is_valid("10.0.0.256"));
        assert.equal(false, IPAddress.is_valid("10.0.0.0.0"));
        assert.equal(true, IPAddress.is_valid("10.0.0"));
        assert.equal(true, IPAddress.is_valid("10.0"));
        assert.equal(false, IPAddress.is_valid("2002:516:2:200"));
        assert.equal(false, IPAddress.is_valid("2002:::1"));
    });

    it("test_module_method_valid_ipv4_netmark", () => {
        assert.equal(true, IPAddress.is_valid_netmask("255.255.255.0"));
        assert.equal(false, IPAddress.is_valid_netmask("10.0.0.1"));
    });

    it("test_summarize", function() {
        this.timeout(15000);
        let netstr: string[] = [];
        for (let range of [Range(1, 10), Range(11, 127), Range(128, 169), Range(170, 172),
            Range(173, 192), Range(193, 224)]) {
            for (let i = range.start; i < range.target; ++i) {
                netstr.push(`${i}.0.0.0/8`);
            }
        }
        for (let i = 0; i < 256; ++i) {
            if (i != 254) {
                netstr.push(`169.${i}.0.0/16`);
            }
        }
        for (let i = 0; i < 256; ++i) {
            if (i < 16 || 31 < i) {
                netstr.push(`172.${i}.0.0/16`);
            }
        }
        for (let i = 0; i < 256; ++i) {
            if (i != 168) {
                netstr.push(`192.${i}.0.0/16`);
            }
        }
        let ip_addresses: IPAddress[] = [];
        for (let net of netstr) {
            ip_addresses.push(IPAddress.parse(net));
        }

        let empty_vec: string[] = [];
        assert.equal(IPAddress.summarize_str(empty_vec).length, 0);
        assertArrayEqual(IPAddress.to_string_vec(IPAddress.summarize_str(["10.1.0.4/24"])
        ),
            ["10.1.0.0/24"]);
        assertArrayEqual(IPAddress.to_string_vec(IPAddress.summarize_str(["2000:1::4711/32"])
        ),
            ["2000:1::/32"]);

        assertArrayEqual(IPAddress.to_string_vec(IPAddress.summarize_str(["10.1.0.4/24",
            "7.0.0.0/0",
            "1.2.3.4/4"])
        ),
            ["0.0.0.0/0"]);
        assertArrayEqual(IPAddress.to_string_vec(IPAddress.summarize_str(["2000:1::/32",
            "3000:1::/32",
            "2000:2::/32",
            "2000:3::/32",
            "2000:4::/32",
            "2000:5::/32",
            "2000:6::/32",
            "2000:7::/32",
            "2000:8::/32"])
        ),
            ["2000:1::/32", "2000:2::/31", "2000:4::/30", "2000:8::/32", "3000:1::/32"]);

        assertArrayEqual(IPAddress.to_string_vec(IPAddress.summarize_str(["10.0.1.1/24",
            "30.0.1.0/16",
            "10.0.2.0/24",
            "10.0.3.0/24",
            "10.0.4.0/24",
            "10.0.5.0/24",
            "10.0.6.0/24",
            "10.0.7.0/24",
            "10.0.8.0/24"])
        ),
            ["10.0.1.0/24", "10.0.2.0/23", "10.0.4.0/22", "10.0.8.0/24", "30.0.0.0/16"]);

        assertArrayEqual(IPAddress.to_string_vec(IPAddress.summarize_str(["10.0.0.0/23",
            "10.0.2.0/24"])
        ),
            ["10.0.0.0/23", "10.0.2.0/24"]);
        assertArrayEqual(IPAddress.to_string_vec(IPAddress.summarize_str(["10.0.0.0/24",
            "10.0.1.0/24",
            "10.0.2.0/23"])
        ),
            ["10.0.0.0/22"]);


        assertArrayEqual(IPAddress.to_string_vec(IPAddress.summarize_str(["10.0.0.0/16",
            "10.0.2.0/24"])
        ),
            ["10.0.0.0/16"]);


        let cnt = 10;
        for (let _ = 0; _ < cnt; ++_) {
            assert.deepEqual(IPAddress.to_string_vec(IPAddress.summarize(ip_addresses)),
                ["1.0.0.0/8",
                    "2.0.0.0/7",
                    "4.0.0.0/6",
                    "8.0.0.0/7",
                    "11.0.0.0/8",
                    "12.0.0.0/6",
                    "16.0.0.0/4",
                    "32.0.0.0/3",
                    "64.0.0.0/3",
                    "96.0.0.0/4",
                    "112.0.0.0/5",
                    "120.0.0.0/6",
                    "124.0.0.0/7",
                    "126.0.0.0/8",
                    "128.0.0.0/3",
                    "160.0.0.0/5",
                    "168.0.0.0/8",
                    "169.0.0.0/9",
                    "169.128.0.0/10",
                    "169.192.0.0/11",
                    "169.224.0.0/12",
                    "169.240.0.0/13",
                    "169.248.0.0/14",
                    "169.252.0.0/15",
                    "169.255.0.0/16",
                    "170.0.0.0/7",
                    "172.0.0.0/12",
                    "172.32.0.0/11",
                    "172.64.0.0/10",
                    "172.128.0.0/9",
                    "173.0.0.0/8",
                    "174.0.0.0/7",
                    "176.0.0.0/4",
                    "192.0.0.0/9",
                    "192.128.0.0/11",
                    "192.160.0.0/13",
                    "192.169.0.0/16",
                    "192.170.0.0/15",
                    "192.172.0.0/14",
                    "192.176.0.0/12",
                    "192.192.0.0/10",
                    "193.0.0.0/8",
                    "194.0.0.0/7",
                    "196.0.0.0/6",
                    "200.0.0.0/5",
                    "208.0.0.0/4"]);
        }
        // end
        // printer = RubyProf.GraphPrinter.new(result)
        // printer.print(STDOUT, {})
        // test imutable input parameters
        let a1 = IPAddress.parse("10.0.0.1/24");
        let a2 = IPAddress.parse("10.0.1.1/24");
        assertArrayEqual(IPAddress.to_string_vec(IPAddress.summarize([a1.clone(), a2.clone()])),
            ["10.0.0.0/23"]);
        assert.equal("10.0.0.1/24", a1.to_string());
        assert.equal("10.0.1.1/24", a2.to_string());
    });
})