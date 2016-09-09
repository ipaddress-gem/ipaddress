#ifndef __PREFIX32__
#define __PREFIX32__

#include "result.hpp"
#include "prefix.hpp"
#include "ip_bits.hpp"

class Prefix32 {
    public:
    static Result<Prefix> from(size_t num) {
        return Prefix32::create(num);
    }

    static Result<Prefix> create(size_t num) {
        if (num <= 32) {
            //static _FROM: &'static (Fn(&::prefix::Prefix, usize) -> Result<::prefix::Prefix, String>) =
            // &from;
            //static _TO_IP_STR: &'static (Fn(&Vec<u16>) -> String) = &to_ip_str;
            auto ip_bits = IpBits::v4();
            auto bits = ip_bits.bits;
            Prefix prefix(
                num,
                ip_bits,
                Prefix::new_netmask(num, bits),
                Prefix32::from
            );
            return Ok(prefix);
        }
        return Err<Prefix>("num to big");
    }

};

#endif
