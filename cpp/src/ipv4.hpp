#ifndef __IPV4__
#define  __IPV4__

#include "crunchy.hpp";
#include "prefix32.hpp";
#include "ipaddress.hpp";
#include "ip_bits.hpp";
#include "prefix128.hpp";
#include "ipv6.hpp";

class Ipv4 {
    public:
    static IPAddress from_number(const Crunchy &addr, size_t prefix_num) {
        auto prefix = Prefix32.create(prefix_num);
        if (!prefix) {
            return null;
        }
        return new IPAddress(
            IpBits.v4(),
            addr.clone(),
            prefix,
            null,
            Ipv4.ipv4_is_private,
            Ipv4.ipv4_is_loopback,
            Ipv4.to_ipv6
        );
    }

    static IPAddress create(const std::string &str) {
        // console.log("create:v4:", str);
        // let enable = str == "0.0.0.0/0";
        auto tmp = IPAddress.split_at_slash(str);
        auto ip = tmp[0];
        auto netmask = tmp[1];
        if (!IPAddress.is_valid_ipv4(ip)) {
            // enable && console.log("xx1");
            return null;
        }
        auto ip_prefix_num = 32;
        if (netmask) {
            //  netmask is defined
            ip_prefix_num = IPAddress.parse_netmask_to_prefix(netmask);
            if (ip_prefix_num === null) {
                // enable && console.log("xx2");
                return null;
            }
            //if ip_prefix.ip_bits.version
        }
        auto ip_prefix = Prefix32.create(ip_prefix_num);
        if (ip_prefix === null) {
            // enable && console.log("xx3");
            return null;
        }
        auto split_number = IPAddress.split_to_u32(ip);
        if (split_number === null) {
            // enable && console.log("xx4");
            return null;
        }
        // console.log(">>>>>>>", ip, ip_prefix);
        return IPAddress(
            IpBits.v4(),
            split_number,
            ip_prefix,
            null,
            Ipv4.ipv4_is_private,
            Ipv4.ipv4_is_loopback,
            Ipv4.to_ipv6
        );
    }

    static bool ipv4_is_private(const IPAddress &my) {
        return [IPAddress.parse("10.0.0.0/8"),
            IPAddress.parse("172.16.0.0/12"),
            IPAddress.parse("192.168.0.0/16")]
            .find(i => i.includes(my)) != null;
    }

    static bool ipv4_is_loopback(const IPAddress &my) {
        return IPAddress.parse("127.0.0.0/8").includes(my);
    }

    static IPAddress to_ipv6(const IPAddress &ia) {
        return IPAddress(
            IpBits.v6(),
            ia.host_address.clone(),
            Prefix128.create(ia.prefix.num),
            null,
            Ipv6.ipv6_is_private,
            Ipv6.ipv6_is_loopback,
            Ipv6.to_ipv6
        );
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
    static boolean is_class_a(const IPAddress &my) {
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
    static boolean is_class_b(const IPAddress &my) {
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
    static bool is_class_c(const IPAddress &my) {
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
    static IPAddress parse_classful(const std::string &ip_si) {
        if (!IPAddress.is_valid_ipv4(ip_si)) {
            return null;
        }
        auto o_ip = IPAddress.parse(ip_si);
        if (o_ip === null) {
            return o_ip;
        }
        auto ip = o_ip;
        if (Ipv4.is_class_a(ip)) {
            ip.prefix = Prefix32.create(8);
        } else if (Ipv4.is_class_b(ip)) {
            ip.prefix = Prefix32.create(16);
        } else if (Ipv4.is_class_c(ip)) {
            ip.prefix = Prefix32.create(24);
        }
        return ip;
    }
};
#endif
