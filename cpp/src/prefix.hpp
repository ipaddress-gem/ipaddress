#ifndef __PREFIX128__
#define __PREFIX128__

#include <ip_bits.hpp>
#include <crunchy.hpp>




class Prefix {
public:
    typedef std::function<Prefix(const Prefix &source, const size_t num)> From;
    size_t num;
    IpBits ip_bits;
    Crunchy net_mask;
    From vt_from;

    Prefix(size_t num, const IpBits &ip_bits, const Crunchy &net_mask, From &from) {
        this.num = num;
        this.ip_bits = ip_bits;
        this.net_mask = net_mask;
        this.vt_from = vt_from;
    }

    Prefix clone() const {
        return Prefix(
            this.num,
            this.ip_bits,
            this.net_mask,
            this.vt_from
        );
    }

    bool eq(Prefix &other) {
        return this->ip_bits.version == other.ip_bits.version &&
            this->num == other.num;
    }
    bool ne(Prefix &other) {
        return !this.eq(other);
    }
    ssize_t cmp(Prefix &oth) {
        if (this->ip_bits.version < oth.ip_bits.version) {
            return -1;
        } else if (this->ip_bits.version > oth.ip_bits.version) {
            return 1;
        } else {
            if (this->num < oth.num) {
                return -1;
            } else if (this->num > oth.num) {
                return 1;
            } else {
                return 0;
            }
        }
    }
    //#[allow(dead_code)]
    Prefix from(size_t num) const {
        return (this->vt_from)(this, num);
    }

    std::string to_ip_str() const {
        return this->ip_bits.vt_as_compressed_string(this->ip_bits, this->net_mask);
    }

    Crunchy size() const {
        return Crunchy.one().shl(this->ip_bits.bits - this->num);
    }

    static Crunchy new_netmask(size_t prefix, size_t bits) {
        auto mask = Crunchy.zero();
        auto host_prefix = bits - prefix;
        for (size_t i = 0; i < prefix; ++i) {
            // console.log(">>>", i, host_prefix, mask);
            mask = mask.add(Crunchy.one().shl(host_prefix + i));
        }
        return mask
    }

    Crunchy netmask() const {
        return this.net_mask;
    }

    size_t get_prefix() const {
        return this.num;
    }

    //  The hostmask is the contrary of the subnet mask,
    //  as it shows the bits that can change within the
    //  hosts
    //
    //    prefix = IPAddress::Prefix32.new 24
    //
    //    prefix.hostmask
    //      // => "0.0.0.255"
    //
    Crunchy host_mask() const {
        auto ret = Crunchy.zero();
        for (size_t _ = 0; _ < (this->ip_bits.bits - this->num); _++) {
            ret = ret.shl(1).add(Crunchy.one());
        }
        return ret;
    }


    //
    //  Returns the length of the host portion
    //  of a netmask.
    //
    //    prefix = Prefix128.new 96
    //
    //    prefix.host_prefix
    //      // => 128
    //
    size_t host_prefix() const {
        return this->ip_bits.bits - this->num;
    }

    //
    //  Transforms the prefix into a string of bits
    //  representing the netmask
    //
    //    prefix = IPAddress::Prefix128.new 64
    //
    //    prefix.bits
    //      // => "1111111111111111111111111111111111111111111111111111111111111111"
    //          "0000000000000000000000000000000000000000000000000000000000000000"
    //
    std::string bits() const {
        return this->netmask().toString(2);
    }
    // #[allow(dead_code)]
    // public net_mask(&self) -> BigUint {
    //     return (self.in_mask.clone() >> (self.host_prefix() as usize)) << (self.host_prefix() as usize);
    // }

    std::string to_s() const {
        return this->get_prefix().toString();
    }
    //#[allow(dead_code)]
    // public inspect(&self) -> String {
    //     return self.to_s();
    // }
    number to_i() const {
        return this.get_prefix();
    }

    Prefix add_prefix(Prefix &other) const {
        return this->from(this->get_prefix() + other.get_prefix());
    }

    Prefix add(size_t other) const {
        return this->from(this->get_prefix() + other)
    }

    Prefix sub_prefix(Prefix &other) const {
        return this->sub(other.get_prefix());
    }

    Prefix sub(size_t other) const {
        if (other > this->get_prefix()) {
            return this->from(other - this->get_prefix());
        }
        return this->from(this->get_prefix() - other);
    }

};

#endif
