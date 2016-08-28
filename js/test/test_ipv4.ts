
import { assert } from 'chai';

import IPAddress from '../src/ipaddress';
import Prefix32 from '../src/prefix32';
import Ipv4 from '../src/ipv4';
import Crunchy from '../src/crunchy';

class IPv4Prefix {
    ip: string;
    prefix: number;
    constructor(obj: { [id: string]: any }) {
        this.ip = obj['ip'];
        this.prefix = obj['prefix'];
    }
}

class IPv4Test {
    valid_ipv4: [string, IPv4Prefix][] = [];
    invalid_ipv4: string[] = ["10.0.0.256", "10.0.0.0.0"];
    valid_ipv4_range: string[] = ["10.0.0.1-254", "10.0.1-254.0", "10.1-254.0.0"];
    netmask_values: [string, string][] = [];
    decimal_values: [string, number][] = [];
    ip: IPAddress = IPAddress.parse("172.16.10.1/24");
    network: IPAddress = IPAddress.parse("172.16.10.0/24");
    networks: [string, string][] = [];
    broadcast: [string, string][] = [];
    class_a: IPAddress = IPAddress.parse("10.0.0.1/8");
    class_b: IPAddress = IPAddress.parse("172.16.0.1/16");
    class_c: IPAddress = IPAddress.parse("192.168.0.1/24");
    classful: [string, number][] = [];
}

describe('Ipv4Test', () => {

    function setup(): IPv4Test {
        let ipv4t = new IPv4Test();
        ipv4t.valid_ipv4.push(["9.9/17",
            new IPv4Prefix({
                ip: ("9.0.0.9"),
                prefix: 17,
            })]);
        ipv4t.valid_ipv4.push(["100.1.100",
            new IPv4Prefix({
                ip: ("100.1.0.100"),
                prefix: 32,
            })]);
        ipv4t.valid_ipv4.push(["0.0.0.0/0",
            new IPv4Prefix({
                ip: ("0.0.0.0"),
                prefix: 0,
            })]);
        ipv4t.valid_ipv4.push(["10.0.0.0",
            new IPv4Prefix({
                ip: ("10.0.0.0"),
                prefix: 32,
            })]);
        ipv4t.valid_ipv4.push(["10.0.0.1",
            new IPv4Prefix({
                ip: ("10.0.0.1"),
                prefix: 32,
            })]);
        ipv4t.valid_ipv4.push(["10.0.0.1/24",
            new IPv4Prefix({
                ip: ("10.0.0.1"),
                prefix: 24,
            })]);
        ipv4t.valid_ipv4.push(["10.0.0.9/255.255.255.0",
            new IPv4Prefix({
                ip: ("10.0.0.9"),
                prefix: 24,
            })]);

        ipv4t.netmask_values.push(["0.0.0.0/0", "0.0.0.0"]);
        ipv4t.netmask_values.push(["10.0.0.0/8", "255.0.0.0"]);
        ipv4t.netmask_values.push(["172.16.0.0/16", "255.255.0.0"]);
        ipv4t.netmask_values.push(["192.168.0.0/24", "255.255.255.0"]);
        ipv4t.netmask_values.push(["192.168.100.4/30", "255.255.255.252"]);

        ipv4t.decimal_values.push(["0.0.0.0/0", 0]);
        ipv4t.decimal_values.push(["10.0.0.0/8", 167772160]);
        ipv4t.decimal_values.push(["172.16.0.0/16", 2886729728]);
        ipv4t.decimal_values.push(["192.168.0.0/24", 3232235520]);
        ipv4t.decimal_values.push(["192.168.100.4/30", 3232261124]);


        ipv4t.broadcast.push(["10.0.0.0/8", "10.255.255.255/8"]);
        ipv4t.broadcast.push(["172.16.0.0/16", "172.16.255.255/16"]);
        ipv4t.broadcast.push(["192.168.0.0/24", "192.168.0.255/24"]);
        ipv4t.broadcast.push(["192.168.100.4/30", "192.168.100.7/30"]);

        ipv4t.networks.push(["10.5.4.3/8", "10.0.0.0/8"]);
        ipv4t.networks.push(["172.16.5.4/16", "172.16.0.0/16"]);
        ipv4t.networks.push(["192.168.4.3/24", "192.168.4.0/24"]);
        ipv4t.networks.push(["192.168.100.5/30", "192.168.100.4/30"]);


        ipv4t.classful.push(["10.1.1.1", 8]);
        ipv4t.classful.push(["150.1.1.1", 16]);
        ipv4t.classful.push(["200.1.1.1", 24]);
        return ipv4t;
    }


    it("test_initialize", () => {
        for (let i of setup().valid_ipv4) {
            let ip = IPAddress.parse(i[0]);
            assert.isOk(ip.is_ipv4() && !ip.is_ipv6());
        }
        assert.equal(32, setup().ip.prefix.ip_bits.bits);
        assert.isFalse(IPAddress.parse("1.f.13.1/-3"));
        assert.isOk(IPAddress.parse("10.0.0.0/8"));
    });
    it("test_initialize_format_error", () => {
        for (let i of setup().invalid_ipv4) {
            assert.isFalse(IPAddress.parse(i[0]));
        }
        assert.isFalse(IPAddress.parse("10.0.0.0/asd"));
    });
    it("test_initialize_without_prefix", () => {
        assert.isOk(IPAddress.parse("10.10.0.0"));
        let ip = IPAddress.parse("10.10.0.0");
        assert.isOk(!ip.is_ipv6() && ip.is_ipv4());
        assert.equal(32, ip.prefix.num);
    });
    it("test_attributes", () => {
        for (let i of setup().valid_ipv4) {
            let arg = i[0];
            let attr = i[1];
            let ip = IPAddress.parse(arg);
            // println!("test_attributes:{}:{:?}", arg, attr);
            assert.equal(attr.ip, ip.to_s());
            assert.equal(attr.prefix, ip.prefix.num);
        }
    });
    it("test_octets", () => {
        let ip = IPAddress.parse("10.1.2.3/8");
        assert.equal(ip.parts(), [10, 1, 2, 3]);
    });
    it("test_method_to_string", () => {
        for (let i of setup().valid_ipv4) {
            let arg = i[0];
            let attr = i[1];
            let ip = IPAddress.parse(arg);
            assert.equal(`${attr.ip}/{attr.prefix}`, ip.to_string());
        }
    });
    it("test_method_to_s", () => {
        for (let i of setup().valid_ipv4) {
            let arg = i[0];
            let attr = i[1];
            let ip = IPAddress.parse(arg);
            assert.equal(attr.ip, ip.to_s());
            // let ip_c = IPAddress.parse(arg);
            // assert.equal(attr.ip, ip.to_s());
        }
    });
    it("test_netmask", () => {
        for (let i of setup().netmask_values) {
            let addr = i[0];
            let mask = i[1];
            let ip = IPAddress.parse(addr);
            assert.equal(ip.netmask().to_s(), mask);
        }
    });
    it("test_method_to_u32", () => {
        for (let i of setup().decimal_values) {
            let addr = i[0];
            let int = i[1];
            let ip = IPAddress.parse(addr);
            assert.equal(ip.host_address.toNumber(), int);
        }
    });
    it("test_method_is_network", () => {
        assert.equal(true, setup().network.is_network());
        assert.equal(false, setup().ip.is_network());
    });
    it("test_one_address_network", () => {
        let network = IPAddress.parse("172.16.10.1/32");
        assert.equal(false, network.is_network());
    });
    it("test_method_broadcast", () => {
        for (let i of setup().broadcast) {
            let addr = i[0];
            let bcast = i[1];
            let ip = IPAddress.parse(addr);
            assert.equal(bcast, ip.broadcast().to_string());
        }
    }
    );
    it("test_method_network", () => {
        for (let i of setup().networks) {
            let addr = i[0];
            let net = i[1];
            let ip = IPAddress.parse(addr);
            assert.equal(net, ip.network().to_string());
        }
    }

    );
    it("test_method_bits", () => {
        let ip = IPAddress.parse("127.0.0.1");
        assert.equal("01111111000000000000000000000001", ip.bits());
    }
    );
    it("test_method_first", () => {
        let ip = IPAddress.parse("192.168.100.0/24");
        assert.equal("192.168.100.1", ip.first().to_s());
        ip = IPAddress.parse("192.168.100.50/24");
        assert.equal("192.168.100.1", ip.first().to_s());
    }
    );
    it("test_method_last", () => {
        let ip = IPAddress.parse("192.168.100.0/24");
        assert.equal("192.168.100.254", ip.last().to_s());
        ip = IPAddress.parse("192.168.100.50/24");
        assert.equal("192.168.100.254", ip.last().to_s());
    }
    );
    it("test_method_each_host", () => {
        let ip = IPAddress.parse("10.0.0.1/29");
        let arr = string[];
        ip.each_host(i => arr.push(i.to_s()));
        assert.equal(arr, ["10.0.0.1", "10.0.0.2", "10.0.0.3", "10.0.0.4", "10.0.0.5", "10.0.0.6"]);
    }
    );
    it("test_method_each", () => {
        let ip = IPAddress.parse("10.0.0.1/29");
        let arr = string[];
        ip.each(i => arr.push(i.to_s()));
        assert.equal(arr, ["10.0.0.0", "10.0.0.1", "10.0.0.2", "10.0.0.3", "10.0.0.4", "10.0.0.5",
                "10.0.0.6", "10.0.0.7"]);
    }
    );
    it("test_method_size", () => {
        let ip = IPAddress.parse("10.0.0.1/29");
        assert.equal(ip.size(), Crunchy.parse("8"));
    }
    );
    it("test_method_network_u32", () => {
        assert.equal(2886732288,
            setup().ip.network().host_address.toNumber());
    }
    );
    it("test_method_broadcast_u32", () => {
        assert.equal(2886732543,
            setup().ip.broadcast().host_address.toNumber());
    }
    );
    it("test_method_include", () => {
        let ip = IPAddress.parse("192.168.10.100/24");
        let addr = IPAddress.parse("192.168.10.102/24");
        assert.equal(true, ip.includes(addr));
        assert.equal(false,
            ip.includes(IPAddress.parse("172.16.0.48")));
        ip = IPAddress.parse("10.0.0.0/8");
        assert.equal(true, ip.includes(IPAddress.parse("10.0.0.0/9")));
        assert.equal(true, ip.includes(IPAddress.parse("10.1.1.1/32")));
        assert.equal(true, ip.includes(IPAddress.parse("10.1.1.1/9")));
        assert.equal(false,
            ip.includes(IPAddress.parse("172.16.0.0/16")));
        assert.equal(false, ip.includes(IPAddress.parse("10.0.0.0/7")));
        assert.equal(false, ip.includes(IPAddress.parse("5.5.5.5/32")));
        assert.equal(false, ip.includes(IPAddress.parse("11.0.0.0/8")));
        ip = IPAddress.parse("13.13.0.0/13");
        assert.equal(false,
            ip.includes(IPAddress.parse("13.16.0.0/32")));
    }
    );
    it("test_method_include_all", () => {
        let ip = IPAddress.parse("192.168.10.100/24");
        let addr1 = IPAddress.parse("192.168.10.102/24");
        let addr2 = IPAddress.parse("192.168.10.103/24");
        assert.equal(true, ip.includes_all([addr1.clone(), addr2]));
        assert.equal(false,
            ip.includes_all([addr1, IPAddress.parse("13.16.0.0/32")]));
    }
    );
    it("test_method_ipv4", () => {
        assert.equal(true, setup().ip.is_ipv4());
    }
    );
    it("test_method_ipv6", () => {
        assert.equal(false, setup().ip.is_ipv6());
    }
    );
    it("test_method_private", () => {
        assert.equal(true,
            IPAddress.parse("192.168.10.50/24").is_private());
        assert.equal(true,
            IPAddress.parse("192.168.10.50/16").is_private());
        assert.equal(true,
            IPAddress.parse("172.16.77.40/24").is_private());
        assert.equal(true,
            IPAddress.parse("172.16.10.50/14").is_private());
        assert.equal(true,
            IPAddress.parse("10.10.10.10/10").is_private());
        assert.equal(true, IPAddress.parse("10.0.0.0/8").is_private());
        assert.equal(false,
            IPAddress.parse("192.168.10.50/12").is_private());
        assert.equal(false, IPAddress.parse("3.3.3.3").is_private());
        assert.equal(false, IPAddress.parse("10.0.0.0/7").is_private());
        assert.equal(false,
            IPAddress.parse("172.32.0.0/12").is_private());
        assert.equal(false,
            IPAddress.parse("172.16.0.0/11").is_private());
        assert.equal(false,
            IPAddress.parse("192.0.0.2/24").is_private());
    }
    );
    it("test_method_octet", () => {
        assert.equal(setup().ip.parts()[0], 172);
        assert.equal(setup().ip.parts()[1], 16);
        assert.equal(setup().ip.parts()[2], 10);
        assert.equal(setup().ip.parts()[3], 1);
    }
    );
    it("test_method_a", () => {
        assert.equal(true, Ipv4.is_class_a(setup().class_a));
        assert.equal(false, Ipv4.is_class_a(setup().class_b));
        assert.equal(false, Ipv4.is_class_a(setup().class_c));
    }
    );
    it("test_method_b", () => {
        assert.equal(true, Ipv4.is_class_b(setup().class_b));
        assert.equal(false, Ipv4.is_class_b(setup().class_a));
        assert.equal(false, Ipv4.is_class_b(setup().class_c));
    }
    );
    it("test_method_c", () => {
        assert.equal(true, Ipv4.is_class_c(setup().class_c));
        assert.equal(false, Ipv4.is_class_c(setup().class_a));
        assert.equal(false, Ipv4.is_class_c(setup().class_b));
    }
    );
    it("test_method_to_ipv6", () => {
        assert.equal(".ac10:a01", setup().ip.to_ipv6().to_s());
    }
    );
    it("test_method_reverse", () => {
        assert.equal(setup().ip.dns_reverse(), "10.16.172.in-addr.arpa");
    }
    );
    it("test_method_dns_rev_domains", () => {
        assert.equal(IPAddress.parse("173.17.5.1/23").dns_rev_domains(),
            ["4.17.173.in-addr.arpa", "5.17.173.in-addr.arpa"]);
        assert.equal(IPAddress.parse("173.17.1.1/15").dns_rev_domains(),
            ["16.173.in-addr.arpa", "17.173.in-addr.arpa"]);
        assert.equal(IPAddress.parse("173.17.1.1/7").dns_rev_domains(),
            ["172.in-addr.arpa", "173.in-addr.arpa"]);
        assert.equal(IPAddress.parse("173.17.1.1/29").dns_rev_domains(),
            [
                "0.1.17.173.in-addr.arpa",
                "1.1.17.173.in-addr.arpa",
                "2.1.17.173.in-addr.arpa",
                "3.1.17.173.in-addr.arpa",
                "4.1.17.173.in-addr.arpa",
                "5.1.17.173.in-addr.arpa",
                "6.1.17.173.in-addr.arpa",
                "7.1.17.173.in-addr.arpa"
            ]);
        assert.equal(IPAddress.parse("174.17.1.1/24").dns_rev_domains(),
            ["1.17.174.in-addr.arpa"]);
        assert.equal(IPAddress.parse("175.17.1.1/16").dns_rev_domains(),
            ["17.175.in-addr.arpa"]);
        assert.equal(IPAddress.parse("176.17.1.1/8").dns_rev_domains(),
            ["176.in-addr.arpa"]);
        assert.equal(IPAddress.parse("177.17.1.1/0").dns_rev_domains(),
            ["in-addr.arpa"]);
        assert.equal(IPAddress.parse("178.17.1.1/32").dns_rev_domains(),
            ["1.1.17.178.in-addr.arpa"]);
    }
    );
    it("test_method_compare", () => {
        let ip1 = IPAddress.parse("10.1.1.1/8");
        let ip2 = IPAddress.parse("10.1.1.1/16");
        let ip3 = IPAddress.parse("172.16.1.1/14");
        let ip4 = IPAddress.parse("10.1.1.1/8");

        // ip2 should be greater than ip1
        assert.equal(true, ip1.lt(ip2));
        assert.equal(false, ip1.gt(ip2));
        assert.equal(false, ip2.lt(ip1));
        // ip2 should be less than ip3
        assert.equal(true, ip2.lt(ip3));
        assert.equal(false, ip2.gt(ip3));
        // ip1 should be less than ip3
        assert.equal(true, ip1.lt(ip3));
        assert.equal(false, ip1.gt(ip3));
        assert.equal(false, ip3.lt(ip1));
        // ip1 should be equal to itself
        assert.equal(true, ip1.eq(ip1));
        // ip1 should be equal to ip4
        assert.equal(true, ip1.eq(ip4));
        // test sorting
        let res = [ip1, ip2, ip3].sort((a,b) => a.cmp(b));
        assert.equal(IPAddress.to_string_vec(res), ["10.1.1.1/8", "10.1.1.1/16", "172.16.1.1/14"]);
        // test same prefix
        ip1 = IPAddress.parse("10.0.0.0/24");
        ip2 = IPAddress.parse("10.0.0.0/16");
        ip3 = IPAddress.parse("10.0.0.0/8");
        {
            let res = [ip1, ip2, ip3].sort((a,b) => a.cmp(b));
            assert.equal(IPAddress.to_string_vec(res), ["10.0.0.0/8", "10.0.0.0/16", "10.0.0.0/24"]);
        }
    }
    );
    it("test_method_minus", () => {
        let ip1 = IPAddress.parse("10.1.1.1/8");
        let ip2 = IPAddress.parse("10.1.1.10/8");
        assert.equal(9, ip2.sub(ip1).toNumber());
        assert.equal(9, ip1.sub(ip2).toNumber());
    }
    );
    it("test_method_plus", () => {
        let ip1 = IPAddress.parse("172.16.10.1/24");
        let ip2 = IPAddress.parse("172.16.11.2/24");
        assert.equal(IPAddress.to_string_vec(ip1.add(ip2)), ["172.16.10.0/23"]);

        ip2 = IPAddress.parse("172.16.12.2/24");
        assert.equal(IPAddress.to_string_vec(ip1.add(ip2)),
            [ip1.network().to_string(), ip2.network().to_string()]);

        ip1 = IPAddress.parse("10.0.0.0/23");
        ip2 = IPAddress.parse("10.0.2.0/24");
        assert.equal(IPAddress.to_string_vec(ip1.add(ip2)),
            ["10.0.0.0/23", "10.0.2.0/24"]);

        ip1 = IPAddress.parse("10.0.0.0/23");
        ip2 = IPAddress.parse("10.0.2.0/24");
        assert.equal(IPAddress.to_string_vec(ip1.add(ip2)),
            ["10.0.0.0/23", "10.0.2.0/24"]);

        ip1 = IPAddress.parse("10.0.0.0/16");
        ip2 = IPAddress.parse("10.0.2.0/24");
        assert.equal(IPAddress.to_string_vec(ip1.add(ip2)), ["10.0.0.0/16"]);

        ip1 = IPAddress.parse("10.0.0.0/23");
        ip2 = IPAddress.parse("10.1.0.0/24");
        assert.equal(IPAddress.to_string_vec(ip1.add(ip2)),
            ["10.0.0.0/23", "10.1.0.0/24"]);
    }
    );
    it("test_method_netmask_equal", () => {
        let ip = IPAddress.parse("10.1.1.1/16");
        assert.equal(16, ip.prefix.num);
        let ip2 = ip.change_netmask("255.255.255.0");
        assert.equal(24, ip2.prefix.num);
    }
    );
    it("test_method_split", () => {
        assert.isFalse(setup().ip.split(0));
        assert.isFalse(setup().ip.split(257));

        assert.equal(setup().ip.split(1), [setup().ip.network()]);

        assert.equal(IPAddress.to_string_vec(setup().network.split(8)),
            ["172.16.10.0/27",
                "172.16.10.32/27",
                "172.16.10.64/27",
                "172.16.10.96/27",
                "172.16.10.128/27",
                "172.16.10.160/27",
                "172.16.10.192/27",
                "172.16.10.224/27"]);

        assert.equal(IPAddress.to_string_vec(setup().network.split(7)),
            ["172.16.10.0/27",
                "172.16.10.32/27",
                "172.16.10.64/27",
                "172.16.10.96/27",
                "172.16.10.128/27",
                "172.16.10.160/27",
                "172.16.10.192/26"]);

        assert.equal(IPAddress.to_string_vec(setup().network.split(6)),
            ["172.16.10.0/27",
                "172.16.10.32/27",
                "172.16.10.64/27",
                "172.16.10.96/27",
                "172.16.10.128/26",
                "172.16.10.192/26"]);
        assert.equal(IPAddress.to_string_vec(setup().network.split(5)),
            ["172.16.10.0/27",
                "172.16.10.32/27",
                "172.16.10.64/27",
                "172.16.10.96/27",
                "172.16.10.128/25"]);
        assert.equal(IPAddress.to_string_vec(setup().network.split(4)),
            ["172.16.10.0/26", "172.16.10.64/26", "172.16.10.128/26", "172.16.10.192/26"]);
        assert.equal(IPAddress.to_string_vec(setup().network.split(3)),
            ["172.16.10.0/26", "172.16.10.64/26", "172.16.10.128/25"]);
        assert.equal(IPAddress.to_string_vec(setup().network.split(2)),
            ["172.16.10.0/25", "172.16.10.128/25"]);
        assert.equal(IPAddress.to_string_vec(setup().network.split(1)),
            ["172.16.10.0/24"]);
    }
    );
    it("test_method_subnet", () => {
        assert.isFalse(setup().network.subnet(23));
        assert.isFalse(setup().network.subnet(33));
        assert.isOk(setup().ip.subnet(30));
        assert.equal(IPAddress.to_string_vec(setup().network.subnet(26)),
            ["172.16.10.0/26",
                "172.16.10.64/26",
                "172.16.10.128/26",
                "172.16.10.192/26"]);
        assert.equal(IPAddress.to_string_vec(setup().network.subnet(25)),
            ["172.16.10.0/25", "172.16.10.128/25"]);
        assert.equal(IPAddress.to_string_vec(setup().network.subnet(24)),
            ["172.16.10.0/24"]);
    }
    );
    it("test_method_supernet", () => {
        assert.isFalse(setup().ip.supernet(24));
        assert.equal("0.0.0.0/0", setup().ip.supernet(0).to_string());
        // assert.equal("0.0.0.0/0", setup().ip.supernet(-2).to_string());
        assert.equal("172.16.10.0/23",
            setup().ip.supernet(23).to_string());
        assert.equal("172.16.8.0/22",
            setup().ip.supernet(22).to_string());
    }
    );
    it("test_classmethod_parse_u32", () => {
        for (let i of setup().decimal_values) {
            let addr = i[0];
            let int = i[1];
            let ip = Ipv4.from_number(int, 32);
            let splitted = addr.split("/");
            let ip2 = ip.change_prefix(parseInt(splitted[1], 10));
            assert.equal(ip2.to_string(), addr);
        }
    });
    it("test_classmethod_summarize", () => {
        // Should return self if only one network given
        assert.equal(IPAddress.summarize([setup().ip]),
            [setup().ip.network()]);

        // Summarize homogeneous networks
        let ip1 = IPAddress.parse("172.16.10.1/24");
        let ip2 = IPAddress.parse("172.16.11.2/24");
        assert.equal(IPAddress.to_string_vec(IPAddress.summarize([ip1, ip2])),
            ["172.16.10.0/23"]);

        {
            let ip1 = IPAddress.parse("10.0.0.1/24");
            let ip2 = IPAddress.parse("10.0.1.1/24");
            let ip3 = IPAddress.parse("10.0.2.1/24");
            let ip4 = IPAddress.parse("10.0.3.1/24");
            assert.equal(IPAddress.to_string_vec(IPAddress.summarize([ip1, ip2, ip3, ip4])),
                ["10.0.0.0/22"]);
        }
        {
            let ip1 = IPAddress.parse("10.0.0.1/24");
            let ip2 = IPAddress.parse("10.0.1.1/24");
            let ip3 = IPAddress.parse("10.0.2.1/24");
            let ip4 = IPAddress.parse("10.0.3.1/24");
            assert.equal(IPAddress.to_string_vec(IPAddress.summarize([ip4, ip3, ip2, ip1])),
                ["10.0.0.0/22"]);
        }

        // Summarize non homogeneous networks
        ip1 = IPAddress.parse("10.0.0.0/23");
        ip2 = IPAddress.parse("10.0.2.0/24");
        assert.equal(IPAddress.to_string_vec(IPAddress.summarize([ip1, ip2])),
            ["10.0.0.0/23", "10.0.2.0/24"]);

        ip1 = IPAddress.parse("10.0.0.0/16");
        ip2 = IPAddress.parse("10.0.2.0/24");
        assert.equal(IPAddress.to_string_vec(IPAddress.summarize([ip1, ip2])),
            ["10.0.0.0/16"]);

        ip1 = IPAddress.parse("10.0.0.0/23");
        ip2 = IPAddress.parse("10.1.0.0/24");
        assert.equal(IPAddress.to_string_vec(IPAddress.summarize([ip1, ip2])),
            ["10.0.0.0/23", "10.1.0.0/24"]);

        ip1 = IPAddress.parse("10.0.0.0/23");
        ip2 = IPAddress.parse("10.0.2.0/23");
        let ip3 = IPAddress.parse("10.0.4.0/24");
        let ip4 = IPAddress.parse("10.0.6.0/24");
        assert.equal(IPAddress.to_string_vec(IPAddress.summarize([ip1, ip2, ip3, ip4])),
            ["10.0.0.0/22", "10.0.4.0/24", "10.0.6.0/24"]);
        {
            let ip1 = IPAddress.parse("10.0.1.1/24");
            let ip2 = IPAddress.parse("10.0.2.1/24");
            let ip3 = IPAddress.parse("10.0.3.1/24");
            let ip4 = IPAddress.parse("10.0.4.1/24");
            assert.equal(IPAddress.to_string_vec(IPAddress.summarize([ip1, ip2, ip3, ip4])),
                ["10.0.1.0/24", "10.0.2.0/23", "10.0.4.0/24"]);
        }
        {
            let ip1 = IPAddress.parse("10.0.1.1/24");
            let ip2 = IPAddress.parse("10.0.2.1/24");
            let ip3 = IPAddress.parse("10.0.3.1/24");
            let ip4 = IPAddress.parse("10.0.4.1/24");
            assert.equal(IPAddress.to_string_vec(IPAddress.summarize([ip4, ip3, ip2, ip1])),
                ["10.0.1.0/24", "10.0.2.0/23", "10.0.4.0/24"]);
        }

        ip1 = IPAddress.parse("10.0.1.1/24");
        ip2 = IPAddress.parse("10.10.2.1/24");
        ip3 = IPAddress.parse("172.16.0.1/24");
        ip4 = IPAddress.parse("172.16.1.1/24");
        assert.equal(IPAddress.to_string_vec(IPAddress.summarize([ip1, ip2, ip3, ip4])),
            ["10.0.1.0/24", "10.10.2.0/24", "172.16.0.0/23"]);

        let ips = [IPAddress.parse("10.0.0.12/30"),
            IPAddress.parse("10.0.100.0/24")];
        assert.equal(IPAddress.to_string_vec(IPAddress.summarize(ips)),
            ["10.0.0.12/30", "10.0.100.0/24"]);

        ips = [IPAddress.parse("172.16.0.0/31"),
            IPAddress.parse("10.10.2.1/32")];
        assert.equal(IPAddress.to_string_vec(IPAddress.summarize(ips)),
            ["10.10.2.1/32", "172.16.0.0/31"]);

        ips = [IPAddress.parse("172.16.0.0/32"),
            IPAddress.parse("10.10.2.1/32")];
        assert.equal(IPAddress.to_string_vec(IPAddress.summarize(ips)),
            ["10.10.2.1/32", "172.16.0.0/32"]);
    }

    );
    it("test_classmethod_parse_classful", () => {
        for (let i in setup().classful) {
            let ip = i[0];
            let prefix = i[1];
            let res = Ipv4.parse_classful(ip);
            assert.equal(prefix, res.prefix.num);
            assert.equal(`{ip}/{prefix}`, res.to_string());
        }
        assert.isFalse(Ipv4.parse_classful("192.168.256.257"));
    });
});
