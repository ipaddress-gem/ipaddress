#ifndef __IPV6__
#define __IPV6__

#include "prefix.hpp"
#include "ip_bits.hpp"
#include "ipaddress.hpp"
#include "ipv4.hpp"
#include "prefix128.hpp"
#include "crunchy.hpp"

#include <string>

class Ipv6 {
public:
    //  =Name
    //
    //  IPAddress::IPv6 - IP version 6 address manipulation library
    //
    //  =Synopsis
    //
    //     require 'ipaddress'
    //
    //  =Description
    //
    //  Class IPAddress::IPv6 is used to handle IPv6 type addresses.
    //
    //  == IPv6 addresses
    //
    //  IPv6 addresses are 128 bits long, in contrast with IPv4 addresses
    //  which are only 32 bits long. An IPv6 address is generally written as
    //  eight groups of four hexadecimal digits, each group representing 16
    //  bits or two octect. For example, the following is a valid IPv6
    //  address:
    //
    //    2001:0db8:0000:0000:0008:0800:200c:417a
    //
    //  Letters in an IPv6 address are usually written downcase, as per
    //  RFC. You can create a new IPv6 object using uppercase letters, but
    //  they will be converted.
    //
    //  === Compression
    //
    //  Since IPv6 addresses are very long to write, there are some
    //  semplifications and compressions that you can use to shorten them.
    //
    //  * Leading zeroes: all the leading zeroes within a group can be
    //    omitted: "0008" would become "8"
    //
    //  * A string of consecutive zeroes can be replaced by the string
    //    "::". This can be only applied once.
    //
    //  Using compression, the IPv6 address written above can be shorten into
    //  the following, equivalent, address
    //
    //    2001:db8::8:800:200c:417a
    //
    //  This short version is often used in human representation.
    //
    //  === Network Mask
    //
    //  As we used to do with IPv4 addresses, an IPv6 address can be written
    //  using the prefix notation to specify the subnet mask:
    //
    //    2001:db8::8:800:200c:417a/64
    //
    //  The /64 part means that the first 64 bits of the address are
    //  representing the network portion, and the last 64 bits are the host
    //  portion.
    //
    //

    static Result<IPAddress> from_str(const std::string &str, size_t radix, size_t prefix) {
        auto num = Crunchy::from_string(str, radix);
        if (num.isErr()) {
            return Err<IPAddress>(num.text());
        }
        return Ipv6::from_int(num.unwrap(), prefix);
    }

    static Result<IPAddress> enhance_if_mapped(IPAddress &ip) {
        // console.log("------A");
        // println!("real mapped {:x} {:x}", &ip.host_address, ip.host_address.clone().shr(32));
        if (ip.is_mapped()) {
            // console.log("------B");
            return Ok(ip);
        }
        // console.log("------C", ip);
        auto ipv6_top_96bit = ip.host_address.shr(32);
        // console.log("------D", ip);
        if (ipv6_top_96bit.eq(Crunchy::from_number(0xffff))) {
            // console.log("------E");
            auto num = ip.host_address.mod(Crunchy::one().shl(32));
            // console.log("------F");
            if (num.eq(Crunchy::zero())) {
                return Ok(ip);
            }
            //println!("ip:{},{:x}", ip.to_string(), num);
            auto ipv4_bits = IpBits::v4();
            if (ipv4_bits.bits < ip.prefix.host_prefix()) {
                std::cout << "mapped:" << ip << std::endl;
                //println!("enhance_if_mapped-2:{}:{}", ip.to_string(), ip.prefix.host_prefix());
                return Err<IPAddress>("prefix not in range");
            }
            // console.log("------G");
            auto mapped = Ipv4::from_number(num, ipv4_bits.bits - ip.prefix.host_prefix());
            // console.log("------H");
            if (mapped.isErr()) {
                // println!("enhance_if_mapped-3");
                return mapped;
            }
            // println!("real mapped!!!!!={}", mapped.clone().to_string());
            ip.mapped = Some(mapped.unwrap());
        }
        return Ok(ip);
    }

    static Result<IPAddress> from_int(const Crunchy &adr, size_t prefix_num) {
        auto prefix = Prefix128::create(prefix_num);
        if (prefix.isErr()) {
            return Err<IPAddress>(prefix.text());
        }
        IPAddress ret(
            IpBits::v6(),
            adr,
            prefix.unwrap(),
            None<IPAddress>(),
            Ipv6::ipv6_is_private,
            Ipv6::ipv6_is_loopback,
            Ipv6::to_ipv6
        );
        return Ipv6::enhance_if_mapped(ret);
    }


    //  Creates a new IPv6 address object.
    //
    //  An IPv6 address can be expressed in any of the following forms:
    //
    //  * "2001:0db8:0000:0000:0008:0800:200C:417A": IPv6 address with no compression
    //  * "2001:db8:0:0:8:800:200C:417A": IPv6 address with leading zeros compression
    //  * "2001:db8::8:800:200C:417A": IPv6 address with full compression
    //
    //  In all these 3 cases, a new IPv6 address object will be created, using the default
    //  subnet mask /128
    //
    //  You can also specify the subnet mask as with IPv4 addresses:
    //
    //    ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    //
    static Result<IPAddress> create(const std::string &str) {
        // console.log("1>>>>>>>>>", str);
        auto r_slash = IPAddress::split_at_slash(str);
        if (r_slash.isErr()) {
          return Err<IPAddress>(r_slash.text());
        }
        // console.log("2>>>>>>>>>", str);
        //std::cout << "ipv6::create|" << str << "|" << r_slash.unwrap().addr << "|" << std::endl;
        if (IPAddress::is_valid_ipv6(r_slash.unwrap().addr)) {
            // console.log("3>>>>>>>>>", str);
            auto o_num = IPAddress::split_to_num(r_slash.unwrap().addr);
            if (o_num.isErr()) {
                // console.log("ipv6_create-1", str);
                return Err<IPAddress>(o_num.text());
            }
            // console.log("4>>>>>>>>>", str);
            size_t netmask = 128;
            if (r_slash.unwrap().netmask.isSome()) {
                auto ret = IPAddress::parse_dec_str(r_slash.unwrap().netmask.unwrap());
                if (ret.isErr()) {
                    // console.log("ipv6_create-2", str);
                    return Err<IPAddress>(ret.text());
                }
                netmask = ret.unwrap();
            }
            // console.log("5>>>>>>>>>", str);
            auto prefix = Prefix128::create(netmask);
            if (prefix.isErr()) {
                // console.log("ipv6_create-3", str);
                return Err<IPAddress>(prefix.text());
            }
            //console.log("6>>>>>>>>>", str, prefix.num, o_netmask, netmask);
            IPAddress ret(
                IpBits::v6(),
                o_num.unwrap().crunchy,
                prefix.unwrap(),
                None<IPAddress>(),
                Ipv6::ipv6_is_private,
                Ipv6::ipv6_is_loopback,
                Ipv6::to_ipv6);
            return Ipv6::enhance_if_mapped(ret);
        } else {
            // console.log("ipv6_create-4", str);
            return Err<IPAddress>("input is not a ipv6 address");
        }
    } //  pub fn initialize

    static IPAddress to_ipv6(const IPAddress &ia) {
        return ia.clone();
    }

    static bool ipv6_is_loopback(const IPAddress &my) {
        // console.log("*************", my.host_address, Crunchy::one());
        return my.host_address.eq(Crunchy::one());
    }


    static bool ipv6_is_private(const IPAddress &my) {
        return IPAddress::parse("fd00::/8").unwrap().includes(my);
    }

};
#endif
