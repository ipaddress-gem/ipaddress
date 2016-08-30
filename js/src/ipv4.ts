
import Crunchy from './crunchy';
import Prefix32 from './prefix32';
import IPAddress from './ipaddress';
import IpBits from './ip_bits';
import Prefix128 from './prefix128';
import Ipv6 from './ipv6';

class Ipv4 {
    public static from_number(addr: Crunchy, prefix_num: number): IPAddress {
        let prefix = Prefix32.create(prefix_num);
        if (!prefix) {
            return null;
        }
        return new IPAddress({
            ip_bits: IpBits.v4(),
            host_address: addr.clone(),
            prefix: prefix,
            mapped: null,
            vt_is_private: Ipv4.ipv4_is_private,
            vt_is_loopback: Ipv4.ipv4_is_loopback,
            vt_to_ipv6: Ipv4.to_ipv6,
        });
    }

    public static create(str: string): IPAddress {
        // console.log("create:v4:", str);
        let enable = str == "0.0.0.0/0";
        let tmp = IPAddress.split_at_slash(str);
        let ip = tmp[0];
        let netmask = tmp[1];
        if (!IPAddress.is_valid_ipv4(ip)) {
            enable && console.log("xx1");
            return null;
        }
        let ip_prefix_num = 32;
        if (netmask) {
            //  netmask is defined
            ip_prefix_num = IPAddress.parse_netmask_to_prefix(netmask);
            if (ip_prefix_num === null) {
                enable && console.log("xx2");
                return null;
            }
            //if ip_prefix.ip_bits.version
        }
        let ip_prefix = Prefix32.create(ip_prefix_num);
        if (ip_prefix === null) {
            enable && console.log("xx3");
            return null;
        }
        let split_number = IPAddress.split_to_u32(ip);
        if (split_number === null) {
            enable && console.log("xx4");
            return null;
        }
        // console.log(">>>>>>>", ip, ip_prefix);
        return new IPAddress({
            ip_bits: IpBits.v4(),
            host_address: split_number,
            prefix: ip_prefix,
            mapped: null,
            vt_is_private: Ipv4.ipv4_is_private,
            vt_is_loopback: Ipv4.ipv4_is_loopback,
            vt_to_ipv6: Ipv4.to_ipv6,
        });
    }

    public static ipv4_is_private(my: IPAddress): boolean {
        return [IPAddress.parse("10.0.0.0/8"),
            IPAddress.parse("172.16.0.0/12"),
            IPAddress.parse("192.168.0.0/16")]
            .find(i => i.includes(my)) != null;
    }

    public static ipv4_is_loopback(my: IPAddress): boolean {
        return IPAddress.parse("127.0.0.0/8").includes(my);
    }

    public static to_ipv6(ia: IPAddress): IPAddress {
        return new IPAddress({
            ip_bits: IpBits.v6(),
            host_address: ia.host_address.clone(),
            prefix: Prefix128.create(ia.prefix.num),
            mapped: null,
            vt_is_private: Ipv6.ipv6_is_private,
            vt_is_loopback: Ipv6.ipv6_is_loopback,
            vt_to_ipv6: Ipv6.to_ipv6
        });
    }

    //  Checks whether the ip address belongs to a
    //  RFC 791 CLASS A network, no matter
    //  what the subnet mask is.
    //
    //  Example:
    //
    //    ip = IPAddress("10.0.0.1/24")
    //
    //    ip.a?
    //      // => true
    //
    public static is_class_a(my: IPAddress): boolean {
        // console.log("is_class_a:", my.to_string(), Crunchy.from_string("80000000", 16), my.is_ipv4()); 
        return my.is_ipv4() && my.host_address.lt(Crunchy.from_string("80000000", 16));
    }

    //  Checks whether the ip address belongs to a
    //  RFC 791 CLASS B network, no matter
    //  what the subnet mask is.
    //
    //  Example:
    //
    //    ip = IPAddress("172.16.10.1/24")
    //
    //    ip.b?
    //      // => true
    //
    public static is_class_b(my: IPAddress): boolean {
        return my.is_ipv4() &&
            Crunchy.from_string("80000000", 16).lte(my.host_address) &&
                my.host_address.lt(Crunchy.from_string("c0000000", 16));
    }

    //  Checks whether the ip address belongs to a
    //  RFC 791 CLASS C network, no matter
    //  what the subnet mask is.
    //
    //  Example:
    //
    //    ip = IPAddress("192.168.1.1/30")
    //
    //    ip.c?
    //      // => true
    //
    public static is_class_c(my: IPAddress): boolean {
        return my.is_ipv4() &&
            Crunchy.from_string("c0000000", 16).lte(my.host_address) &&
                my.host_address.lt(Crunchy.from_string("e0000000", 16));
    }


    //  Creates a new IPv4 address object by parsing the
    //  address in a classful way.
    //
    //  Classful addresses have a fixed netmask based on the
    //  class they belong to:
    //
    //  * Class A, from 0.0.0.0 to 127.255.255.255
    //  * Class B, from 128.0.0.0 to 191.255.255.255
    //  * Class C, D and E, from 192.0.0.0 to 255.255.255.254
    //
    //  Example:
    //
    //    ip = IPAddress::IPv4.parse_classful "10.0.0.1"
    //
    //    ip.netmask
    //      // => "255.0.0.0"
    //    ip.a?
    //      // => true
    //
    //  Note that classes C, D and E will all have a default
    //  prefix of /24 or 255.255.255.0
    //
    public static parse_classful(ip_si: string): IPAddress {
        if (!IPAddress.is_valid_ipv4(ip_si)) {
            return null;
        }
        let o_ip = IPAddress.parse(ip_si);
        if (o_ip === null) {
            return o_ip;
        }
        let ip = o_ip;
        if (Ipv4.is_class_a(ip)) {
            ip.prefix = Prefix32.create(8);
        } else if (Ipv4.is_class_b(ip)) {
            ip.prefix = Prefix32.create(16);
        } else if (Ipv4.is_class_c(ip)) {
            ip.prefix = Prefix32.create(24);
        }
        return ip;
    }
}


export default Ipv4;