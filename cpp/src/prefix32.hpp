#ifndef __PREFIX32__
#define __PREFIX32__

#include <prefix.hpp>
#include <ip_bits.hpp>

class Prefix32 {
    public:
    static Prefix from(Prefix &my, size_t num) {
        return Prefix32.create(num);
    }

    static Prefix create(size_t num) {
        if (0 <= num && num <= 32) {
            //static _FROM: &'static (Fn(&::prefix::Prefix, usize) -> Result<::prefix::Prefix, String>) =
            // &from;
            //static _TO_IP_STR: &'static (Fn(&Vec<u16>) -> String) = &to_ip_str;
            auto ip_bits = IpBits.v4();
            auto bits = ip_bits.bits;
            return Prefix(
                num,
                ip_bits,
                Prefix.new_netmask(num, bits),
                Prefix32.from
            );
        }
        return null;
    }

};

#endif
