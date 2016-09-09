#ifndef __PREFIX128__
#define __PREFIX128__
#include "prefix.hpp"
#include "ip_bits.hpp"

#include "result.hpp"

class Prefix128 {
public:
    // #[derive(Ord,PartialOrd,Eq,PartialEq,Debug,Copy,Clone)]
    // pub struct Prefix128 {
    // }
    //
    // impl Prefix128 {
    //
    //  Creates a new prefix object for 128 bits IPv6 addresses
    //
    //    prefix = IPAddressPrefix128.new 64
    //      // => 64
    //
    //#[allow(unused_comparisons)]
    static Result<Prefix> create(size_t num) {
        if (num <= 128) {
            //static _FROM: &'static (Fn(&Prefix, usize) -> Result<Prefix, String>) = &from;
            //static _TO_IP_STR: &'static (Fn(&Vec<u16>) -> String) = &Prefix128::to_ip_str;
            auto ip_bits = IpBits::v6();
            auto bits = ip_bits.bits;
            Prefix ret(
                num,
                ip_bits,
                Prefix::new_netmask(num, bits),
                Prefix128::from // vt_to_ip_str: _TO_IP_STR
            );
            return Ok(ret);
        }
        return Err<Prefix>("num is to big 128");
    }

    static Result<Prefix> from(size_t num) {
        return Prefix128::create(num);
    }
};

#endif
