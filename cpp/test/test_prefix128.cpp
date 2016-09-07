import { assert } from 'chai';
import * as Mocha from 'mocha';

import Prefix128 from '../src/prefix128';
import Crunchy from '../src/crunchy';

class Prefix128Test {
    public u128_hash: any[] = [];
}

describe("prefix128", () => {

    function setup(): Prefix128Test {
        let p128t = new Prefix128Test();
        p128t.u128_hash.push([32, Crunchy.parse("340282366841710300949110269838224261120")]);
        p128t.u128_hash.push([64, Crunchy.parse("340282366920938463444927863358058659840")]);
        p128t.u128_hash.push([96, Crunchy.parse("340282366920938463463374607427473244160")]);
        p128t.u128_hash.push([126, Crunchy.parse("340282366920938463463374607431768211452")]);
        return p128t;
    }

    it("test_initialize", () => {
        assert.isNotOk(Prefix128.create(129));
        assert.isOk(Prefix128.create(64));
    });

    it("test_method_bits", () => {
        let prefix = Prefix128.create(64);
        let str = "";
        for (let i = 0; i < 64; ++i) {
            str += "1";
        }
        for (let i = 0; i < 64; ++i) {
            str += "0";
        }
        assert.equal(str, prefix.bits())
    });
    it("test_method_to_u32", () => {
        for (let hash of setup().u128_hash) {
            assert.isOk(hash[1].eq(Prefix128.create(hash[0]).netmask()))
        }
    });
});
