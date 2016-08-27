import { assert } from 'chai';

import IPAddress from '../src/ipaddress';
import Prefix128 from '../src/prefix128';
import Ipv6Unspec from '../src/ipv6_unspec';
import Ipv6Mapped from '../src/ipv6_mapped';
import Crunchy from '../src/crunchy';

  class IPv6MappedTest {
        ip: IPAddress;
        s: string;
        sstr: string;
        string: string;
        u128: Crunchy;
        address: string;
        valid_mapped: {[id:string]: Crunchy} = {};
        valid_mapped_ipv6: {[id:string]: Crunchy} = {};
        valid_mapped_ipv6_conversion: {[id:string]: string} = {};
        constructor(obj: {[id:string]: any}) {
            this.ip = obj['ip'];
            this.s = obj['s'];
            this.sstr = obj['sstr'];
            this.string = obj['string'];
            this.u128 = obj['u128'];
            this.address = obj['address'];
        }
    }


describe("ipv6_mapped", () => {
  
    function setup(): IPv6MappedTest {
        let valid_mapped: {[id:string]: Crunchy} =  {};
        valid_mapped.insert("::13.1.68.3", Crunchy.from_str("281470899930115"));
        valid_mapped.insert("0:0:0:0:0:ffff:129.144.52.38", Crunchy.from_str("281472855454758"));
        valid_mapped.insert(" .ffff:129.144.52.38", Crunchy.from_str("281472855454758"));
        let valid_mapped_ipv6: {[id:string]: Crunchy} = {};
        valid_mapped_ipv6.insert(" .ffff:13.1.68.3", Crunchy.from_str("281470899930115"));
        valid_mapped_ipv6.insert("0:0:0:0:0:ffff:8190:3426", Crunchy.from_str("281472855454758"));
        valid_mapped_ipv6.insert(" .ffff:8190:3426", Crunchy.from_str("281472855454758"));
        let valid_mapped_ipv6_conversion:  {[id:string]: string} = {};
        valid_mapped_ipv6_conversion.insert(" .ffff:13.1.68.3", "13.1.68.3");
        valid_mapped_ipv6_conversion.insert("0:0:0:0:0:ffff:8190:3426", "129.144.52.38");
        valid_mapped_ipv6_conversion.insert(" .ffff:8190:3426", "129.144.52.38");
        return new IPv6MappedTest({
            ip: Ipv6Mapped.create("::172.16.10.1"),
            s: " ::ffff:172.16.10.1",
            sstr: " ::ffff:172.16.10.1/32",
            string: "0000:0000:0000:0000:0000:ffff:ac10:0a01/128",
            u128: Crunchy.parse("281473568475649"),
            address: "::ffff:ac10:a01/128",
            valid_mapped: valid_mapped,
            valid_mapped_ipv6: valid_mapped_ipv6,
            valid_mapped_ipv6_conversion: valid_mapped_ipv6_conversion,
        });
    }

    it("test_initialize", () => {
        let s = setup();
        assert.isOk!(IPAddress.parse("::172.16.10.1"));
    for (ip, u128) in s.valid_mapped {
        println!("-{}--{}", ip, u128);
        if IPAddress .parse(ip).is_err() {
            println!("{}", IPAddress .parse(ip));
            }
assert.isOk!(true, IPAddress .parse(ip));
assert.isOk!(u128, IPAddress .parse(ip).host_address);
        }
for (ip, u128) in s.valid_mapped_ipv6 {
    println!("===={}=={:x}", ip, u128);
    assert.isOk!(true, IPAddress .parse(ip));
    assert.isOk!(u128, IPAddress .parse(ip).host_address);
}
    });
it("test_mapped_from_ipv6_conversion", () => {
    for (ip6, ip4) in setup().valid_mapped_ipv6_conversion {
        println!("+{}--{}", ip6, ip4);
        assert.isOk!(ip4, IPAddress .parse(ip6).mapped.to_s());
        }
    });
it("test_attributes", () => {
    let s = setup();
    assert.isOk!(s.address, s.ip.to_string());
    assert.isOk!(128, s.ip.prefix.num);
    assert.isOk!(s.s, s.ip.to_s_mapped());
    assert.isOk!(s.sstr, s.ip.to_string_mapped());
    assert.isOk!(s.string, s.ip.to_string_uncompressed());
    assert.isOk!(s.u128, s.ip.host_address);
});
it("test_method_ipv6", () => {
    assert!(setup().ip.is_ipv6());
});
it("test_mapped", () => {
    assert!(setup().ip.is_mapped());
});
});
