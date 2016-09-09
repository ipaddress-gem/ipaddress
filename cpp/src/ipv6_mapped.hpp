#ifndef __IPV6_MAPPED__
#define __IPV6_MAPPED__

#include "crunchy.hpp"
#include "ip_bits.hpp"
#include "ipaddress.hpp"

class Ipv6Mapped {
public:
    //  Ac
    //  It is usually identified as a IPv4 mapped IPv6 address, a particular
    //  IPv6 address which aids the transition from IPv4 to IPv6. The
    //  structure of the address is
    //
    //    ::ffff:w.y.x.z
    //
    //  where w.x.y.z is a normal IPv4 address. For example, the following is
    //  a mapped IPv6 address:
    //
    //    ::ffff:192.168.100.1
    //
    //  IPAddress is very powerful in handling mapped IPv6 addresses, as the
    //  IPv4 portion is stored internally as a normal IPv4 object. Let's have
    //  a look at some examples. To create a new mapped address, just use the
    //  class builder itself
    //
    //    ip6 = IPAddress::IPv6::Mapped.new "::ffff:172.16.10.1/128"
    //
    //  or just use the wrapper method
    //
    //    ip6 = IPAddress "::ffff:172.16.10.1/128"
    //
    //  Let's check it's really a mapped address:
    //
    //    ip6.mapped?
    //      // => true
    //
    //    ip6.to_string
    //      // => "::FFFF:172.16.10.1/128"
    //
    //  Now with the +ipv4+ attribute, we can easily access the IPv4 portion
    //  of the mapped IPv6 address:
    //
    //    ip6.ipv4.address
    //      // => "172.16.10.1"
    //
    //  Internally, the IPv4 address is stored as two 16 bits
    //  groups. Therefore all the usual methods for an IPv6 address are
    //  working perfectly fine:
    //
    //    ip6.to_hex
    //      // => "00000000000000000000ffffac100a01"
    //
    //    ip6.address
    //      // => "0000:0000:0000:0000:0000:ffff:ac10:0a01"
    //
    //  A mapped IPv6 can also be created just by specify the address in the
    //  following format:
    //
    //    ip6 = IPAddress "::172.16.10.1"
    //
    //  That is, two colons and the IPv4 address. However, as by RFC, the ffff
    //  group will be automatically added at the beginning
    //
    //    ip6.to_string
    //      => "::ffff:172.16.10.1/128"
    //
    //  making it a mapped IPv6 compatible address.
    //
    //
    //  Creates a new IPv6 IPv4-mapped address
    //
    //    ip6 = IPAddress::IPv6::Mapped.new "::ffff:172.16.10.1/128"
    //
    //    ipv6.ipv4.class
    //      // => IPAddress::IPv4
    //
    //  An IPv6 IPv4-mapped address can also be created using the
    //  IPv6 only format of the address:
    //
    //    ip6 = IPAddress::IPv6::Mapped.new "::0d01:4403"
    //
    //    ip6.to_string
    //      // => "::ffff:13.1.68.3"
    //
    static Result<IPAddress> create(const std::string &str) {
        // console.log("mapped-1");
        auto i = IPAddress::split_at_slash(str);
        if (i.isErr()) {
          std::cout << "mapped:slash" << std::endl;
          return Err<IPAddress>(i.text());
        }
        std::vector<std::string> split_colon;
        boost::split(split_colon, i.unwrap().addr, boost::is_any_of(":"));
        if (split_colon.size() <= 1) {
            // console.log("mapped-2");
            std::cout << "mapped:colon" << std::endl;
            return Err<IPAddress>("no colon found");
        }
        // if split_colon.get(0).unwrap().len() > 0 {
        //     // println!("---1a");
        //     return Err(format!("not mapped format-2: {}", &str));
        // }
        // auto mapped: Option<IPAddress> = None;
        std::string netmask;
        if (i.unwrap().netmask.isSome()) {
            netmask += "/";
            netmask += i.unwrap().netmask.unwrap();
        }
        auto ipv4_str = split_colon[split_colon.size() - 1];
        if (IPAddress::is_valid_ipv4(ipv4_str)) {
          std::string ipv4_net = ipv4_str;
          ipv4_net += netmask;
            auto ipv4 = IPAddress::parse(ipv4_net);
            if (ipv4.isErr()) {
                std::cout << "mapped:ipv4parse" << std::endl;
                // console.log("mapped-3");
                return ipv4;
            }
            //mapped = Some(ipv4.unwrap());
            auto addr = ipv4.unwrap();
            auto ipv6_bits = IpBits::v6();
            auto part_mod = Crunchy::from_number(ipv6_bits.part_mod);
            auto up_addr = addr.host_address;
            auto down_addr = addr.host_address;

            std::stringstream rebuild_ipv6;
            const char *colon = "";
            for (size_t i = 0; i < split_colon.size() - 1; ++i) {
                rebuild_ipv6 << colon << split_colon[i];
                colon = ":";
            }
            rebuild_ipv6 << colon;
            auto high_part = up_addr.shr(IpBits::v6().part_bits).mod(part_mod);
            auto low_part = down_addr.mod(part_mod);
            auto bits = ipv6_bits.bits - addr.prefix.host_prefix();
            std::stringstream rebuild_ipv4;
            rebuild_ipv4 << high_part.toString(16) << ":" << low_part.toString(16) << "/" << bits;
            rebuild_ipv6 << rebuild_ipv4.str();
            // std::cout << "mapped:rebuild_ipv6:" << ipv4_net << ":" << addr.host_address <<
            //   "|" << rebuild_ipv6.str() << "|" << high_part << ":" << low_part
            //   << ":" << part_mod << std::endl;

            // console.log("-----A", rebuild_ipv6, part_mod);
            auto r_ipv6 = IPAddress::parse(rebuild_ipv6.str());
            if (r_ipv6.isErr()) {
                std::cout << "mapped:ipv6parse|" << rebuild_ipv6.str() << "|" << r_ipv6.text() << std::endl;
                // println!("---3|{}", &rebuild_ipv6);
                // console.log("mapped-4");
                return r_ipv6;
            }
            auto ipv6 = r_ipv6.unwrap();
            if (ipv6.is_mapped()) {
                // console.log("mapped-5");
                // std::cout << "mapped:mapped" << std::endl;
                return r_ipv6;
            }
            auto p96bit = ipv6.host_address.shr(32);
            if (!p96bit.eq(Crunchy::zero())) {
                std::cout << "mapped:no ffff" << std::endl;
                // println!("---4|{}", &rebuild_ipv6);
                //console.log("mapped-6",ipv6.host_address, p96bit, Crunchy::zero());
                return Err<IPAddress>("ipv6 part is not ::ffff:");
            }
            std::stringstream s2;
            s2 << "::ffff:" << rebuild_ipv4.str();
            return IPAddress::parse(s2.str());
        }
        std::cout << "ipv4 not valid" << std::endl;
        // console.log("mapped-9");
        return Err<IPAddress>("not a valid mapped string");
    }
};
#endif
