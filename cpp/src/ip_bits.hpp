#ifndef __IP_BITS__
#define __IP_BITS__

#include <crunchy.hpp>
#include <rle.hpp>
#include <ip_version.hpp>

#include <string>
#include <function>
#include <vector>

typedef std::function<std::string(const IpBits &source, const Crunchy &num)> ToString;

class IpBits {
public:
    IpVersion version;
    ToString vt_as_compressed_string;
    ToString vt_as_uncompressed_string;
    size_t bits;
    size_t part_bits: number;
    size_t dns_bits: number;
    std::string rev_domain: string;
    size_t part_mod: number;
    Crunchy host_ofs; // ipv4=1, ipv6=0

    IpBits clone() const {
        IpBits my;
        my.version = this->version;
        my.vt_as_compressed_string = this->vt_as_compressed_string;
        my.vt_as_uncompressed_string = this->vt_as_uncompressed_string;
        my.bits = this->bits;
        my.part_bits = this->part_bits;
        my.dns_bits = this->dns_bits;
        my.rev_domain = this->rev_domain;
        my.part_mod = this->part_mod;
        my.host_ofs = this->host_ofs.clone();
        return my;
    }
    std::vector<size_t> parts(const Crunchy &bu) const {
        std::vector<size_t> vec;
        auto my = bu.clone();
        auto part_mod = Crunchy.one().shl(this.part_bits);// - Crunchy::one();
        for (size_t i = 0; i < (this->bits / this->part_bits); ++i) {
            // console.log("parts-1:", my, part_mod, my.mod(part_mod), my.mod(part_mod).toString());
            vec.push_back(my.mod(part_mod).toString());
            my = my.shr(this->part_bits);
        }
        vec.reverse();
        // console.log("parts:", vec);
        return vec;
    }

    std::string as_compressed_string(const Crunchy &bu) const {
        return (this->vt_as_compressed_string)(*this, bu);
    }
    std::string as_uncompressed_string(const Crunchy &bu) const {
        return (this.vt_as_uncompressed_string)(*this, bu);
    }

    std::string dns_part_format(size_t i) {
        switch (this->version) {
            case IpVersion.V4: return `${i}`;
            case IpVersion.V6: return `${i.toString(16)}`;
        }
    }

    static IpBits v4() {
        IpBits my;
        my.version = IpVersion.V4;
        my.vt_as_compressed_string = IpBits.ipv4_as_compressed;
        my.vt_as_uncompressed_string = IpBits.ipv4_as_compressed;
        my.bits = 32;
        my.part_bits = 8;
        my.dns_bits = 8;
        my.rev_domain = "in-addr.arpa";
        my.part_mod = 1 << 8;
        my.host_ofs = Crunchy.one();
        return my;
    }

    static IpBits v6() {
        IpBits my;
        my.version = IpVersion.V6;
        my.vt_as_compressed_string = IpBits.ipv6_as_compressed;
        my.vt_as_uncompressed_string = IpBits.ipv6_as_uncompressed;
        my.bits = 128;
        my.part_bits = 16;
        my.dns_bits = 4;
        my.rev_domain = "ip6.arpa";
        my.part_mod = 1 << 16;
        my.host_ofs = Crunchy.zero();
        return my;
    }

    static std::string ipv4_as_compressed(IpBits &ip_bits, Crunchy &host_address) {
        std::string ret;
        std::string sep;
        for (auto part : ip_bits.parts(host_address)) {
            ret += sep;
            ret += `${part}`;
            sep = ".";
        }
        return ret;
    }

    static std::string ipv6_as_compressed(IpBits &ip_bits, Crunchy &host_address) {
        //println!("ipv6_as_compressed:{}", host_address);
        std::string ret;
        std::string colon;
        bool done = false;
        for (auto rle : Rle.code(ip_bits.parts(host_address))) {
//            console.log(rle.toString());
            for (size_t _ = 0; _ < rle.cnt; _++) {
                if (done || !(rle.part == 0 && rle.max)) {
                    ret += `${colon}${rle.part.toString(16)}`;
                    colon = ":";
                } else if (rle.part == 0 && rle.max) {
                    ret += "::";
                    colon = "";
                    done = true;
                    break;
                }
            }
        }
        return ret;
    }
    static ipv6_as_uncompressed(ip_bits: IpBits, host_address: Crunchy): string {
        std::string ret;
        std::string sep;
        for (auto part : ip_bits.parts(host_address)) {
            ret += sep;
            ret += (0x10000 + part).toString(16).slice(1);
            sep = ":";
        }
        return ret;
    }

};

#endif
