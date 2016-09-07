import { assert } from 'chai';
import * as Mocha from 'mocha';

import IPAddress from '../src/ipaddress';
import Prefix128 from '../src/prefix128';
import Ipv6Unspec from '../src/ipv6_unspec';
import Crunchy from '../src/crunchy';

class IPv6UnspecifiedTest {
    ip: IPAddress;
    to_s: string;
    to_string: string;
    to_string_uncompressed: string;
    num: Crunchy;
    constructor(obj: { [id: string]: any }) {
        this.ip = obj['ip'];
        this.to_s = obj['to_s'];
        this.to_string = obj['to_string'];
        this.to_string_uncompressed = obj['to_string_uncompressed'];
        this.num = obj['num'];
    }
}

describe("ipv6_unspec", () => {

    function setup(): IPv6UnspecifiedTest {
        return new IPv6UnspecifiedTest({
            ip: Ipv6Unspec.create(),
            to_s: "::",
            to_string: "::/128",
            to_string_uncompressed: "0000:0000:0000:0000:0000:0000:0000:0000/128",
            num: Crunchy.zero(),
        });
    }

    it("test_attributes", () => {
        assert.isOk(setup().ip.host_address.eq(setup().num));
        assert.equal(128, setup().ip.prefix.num);
        assert.equal(true, setup().ip.is_unspecified());
        assert.equal(setup().to_s, setup().ip.to_s());
        assert.equal(setup().to_string, setup().ip.to_string());
        assert.equal(setup().to_string_uncompressed,
            setup().ip.to_string_uncompressed());
    });
    it("test_method_ipv6", () => {
        assert.equal(true, setup().ip.is_ipv6());
    });
});
