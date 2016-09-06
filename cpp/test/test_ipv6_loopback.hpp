import { assert } from 'chai';
import * as Mocha from 'mocha';

import IPAddress from '../src/ipaddress';
import Prefix128 from '../src/prefix128';
import Ipv6Loopback from '../src/ipv6_loopback';
import Crunchy from '../src/crunchy';

class IPv6LoopbackTest {
    ip: IPAddress;
    s: string;
    n: string;
    string: string;
    one: Crunchy;
    constructor(obj: { [id: string]: any }) {
        this.ip = obj['ip'];
        this.s = obj['s'];
        this.n = obj['n'];
        this.string = obj['string'];
        this.one = obj['one'];
    }
}


describe("ipv6_loopback", () => {
    function setup(): IPv6LoopbackTest {
        return new IPv6LoopbackTest({
            ip: Ipv6Loopback.create(),
            s: "::1",
            n: "::1/128",
            string: "0000:0000:0000:0000:0000:0000:0000:0001/128",
            one: Crunchy.one()
        });
    }

    it("test_attributes", () => {
        let s = setup();
        assert.equal(128, s.ip.prefix.num);
        assert.equal(true, s.ip.is_loopback());
        assert.equal(s.s, s.ip.to_s());
        assert.equal(s.n, s.ip.to_string());
        assert.equal(s.string, s.ip.to_string_uncompressed());
        assert.equal(s.one.toString(), s.ip.host_address.toString());
    });
    it("test_method_ipv6", () => {
        assert.equal(true, setup().ip.is_ipv6());
    });
});
