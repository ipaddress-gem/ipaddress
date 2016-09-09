#ifndef __IP_BITS__
#define __IP_BITS__

#include "crunchy.hpp"
#include "rle.hpp"
#include "ip_version.hpp"

#include <string>
#include <functional>
#include <vector>


class IpBits {
public:
    typedef std::function<std::string(const IpBits &source, const Crunchy &num)> ToString;
    IpVersion version;
    ToString vt_as_compressed_string;
    ToString vt_as_uncompressed_string;
    size_t bits;
    size_t part_bits;
    size_t dns_bits;
    std::string rev_domain;
    size_t part_mod;
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
        // auto part_mod = Crunchy::one().shl(this->part_bits);// - Crunchy::one();
        for (size_t i = 0; i < (this->bits / this->part_bits); ++i) {
            // console.log("parts-1:", my, part_mod, my.mod(part_mod), my.mod(part_mod).toString());
            vec.push_back(my.mds(this->part_mod));
            my = my.shr(this->part_bits);
        }
        std::reverse(vec.begin(),vec.end());
        // console.log("parts:", vec);
        return vec;
    }

    std::string as_compressed_string(const Crunchy &bu) const {
        return (this->vt_as_compressed_string)(*this, bu);
    }
    std::string as_uncompressed_string(const Crunchy &bu) const {
        return (this->vt_as_uncompressed_string)(*this, bu);
    }

    std::string dns_part_format(size_t i) const {
      std::stringstream s2;
      if (this->version ==  IpVersion::V6) {
        s2 << std::hex;
      }
      s2 << i;
      return s2.str();
    }

    static IpBits v4() {
        IpBits my;
        my.version = IpVersion::V4;
        my.vt_as_compressed_string = IpBits::ipv4_as_compressed;
        my.vt_as_uncompressed_string = IpBits::ipv4_as_compressed;
        my.bits = 32;
        my.part_bits = 8;
        my.dns_bits = 8;
        my.rev_domain = "in-addr.arpa";
        my.part_mod = 1 << 8;
        my.host_ofs = Crunchy::one();
        return my;
    }

    static IpBits v6() {
        IpBits my;
        my.version = IpVersion::V6;
        my.vt_as_compressed_string = IpBits::ipv6_as_compressed;
        my.vt_as_uncompressed_string = IpBits::ipv6_as_uncompressed;
        my.bits = 128;
        my.part_bits = 16;
        my.dns_bits = 4;
        my.rev_domain = "ip6.arpa";
        my.part_mod = 1 << 16;
        my.host_ofs = Crunchy::zero();
        return my;
    }

    static std::string ipv4_as_compressed(const IpBits &ip_bits, const Crunchy &host_address) {
        std::stringstream ret;
        std::string sep;
        for (auto part : ip_bits.parts(host_address)) {
            ret << sep;
            ret << part;
            sep = ".";
        }
        return ret.str();
    }

    static std::string ipv6_as_compressed(const IpBits &ip_bits, const Crunchy &host_address) {
        //println!("ipv6_as_compressed:{}", host_address);
        std::stringstream ret;
        std::string colon;
        bool done = false;
        ret << std::hex;
        for (auto rle : Rle::code(ip_bits.parts(host_address))) {
//            console.log(rle.toString());
            for (size_t _ = 0; _ < rle.cnt; _++) {
                if (done || !(rle.part == 0 && rle.max)) {
                    ret << colon << rle.part;
                    colon = ":";
                } else if (rle.part == 0 && rle.max) {
                    ret << "::";
                    colon = "";
                    done = true;
                    break;
                }
            }
        }
        return ret.str();
    }
    static std::string ipv6_as_uncompressed(const IpBits &ip_bits, const Crunchy &host_address) {
        std::string ret;
        const char *sep = "";
        for (auto part : ip_bits.parts(host_address)) {
            ret += sep;
            std::stringstream s2;
            s2 << std::hex << (0x10000 + part);
            ret += s2.str().substr(1);
            sep = ":";
        }
        return ret;
    }

};

std::ostream& operator<<(std::ostream &o, const IpBits &ip_bits);

#endif
