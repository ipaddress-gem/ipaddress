
import Crunchy from './crunchy';

import Rle from './rle';

import IpVersion from './ip_version';

interface ToString {
    (source: IpBits, num: Crunchy): string;
}

// #[derive(Debug, Clone)]
class IpBits {
    version: IpVersion;
    vt_as_compressed_string: ToString;
    vt_as_uncompressed_string: ToString;
    bits: number;
    part_bits: number;
    dns_bits: number;
    rev_domain: string;
    part_mod: number;
    host_ofs: Crunchy; // ipv4=1, ipv6=0

    public clone(): IpBits {
        let my = new IpBits();
        my.version = this.version;
        my.vt_as_compressed_string = this.vt_as_compressed_string;
        my.vt_as_uncompressed_string = this.vt_as_uncompressed_string;
        my.bits = this.bits;
        my.part_bits = this.part_bits;
        my.dns_bits = this.dns_bits;
        my.rev_domain = this.rev_domain;
        my.part_mod = this.part_mod;
        my.host_ofs = this.host_ofs.clone();
        return my;
    }

    public parts(bu: Crunchy): number[] {
        let vec: number[] = [];
        let my = bu.clone();
        let part_mod = Crunchy.one().shl(this.part_bits);// - Crunchy::one();
        for (let i = 0; i < (this.bits / this.part_bits); ++i) {
            // console.log("parts-1:", my, part_mod, my.mod(part_mod), my.mod(part_mod).toString());
            vec.push(+my.mod(part_mod).toString());
            my = my.shr(this.part_bits);
        }
        // console.log("parts:", vec);
        return vec.reverse();
    }

    public as_compressed_string(bu: Crunchy): string {
        return (this.vt_as_compressed_string)(this, bu);
    }
    public as_uncompressed_string(bu: Crunchy): string {
        return (this.vt_as_uncompressed_string)(this, bu);
    }

    public dns_part_format(i: number): string {
        switch (this.version) {
            case IpVersion.V4: return `${i}`;
            case IpVersion.V6: return `${i.toString(16)}`;
        }
    }

    public static v4(): IpBits {
        let my = new IpBits();
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

    public static v6(): IpBits {
        let my = new IpBits();
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

    public static ipv4_as_compressed(ip_bits: IpBits, host_address: Crunchy): string {
        let ret = "";
        let sep = "";
        for (let part of ip_bits.parts(host_address)) {
            ret += sep;
            ret += `${part}`;
            sep = ".";
        }
        return ret;
    }

    public static ipv6_as_compressed(ip_bits: IpBits, host_address: Crunchy): string {
        //println!("ipv6_as_compressed:{}", host_address);
        let ret = "";
        let colon = "";
        let done = false;
        for (let rle of Rle.code(ip_bits.parts(host_address))) {
//            console.log(rle.toString());
            for (let _ = 0; _ < rle.cnt; _++) {
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
    public static ipv6_as_uncompressed(ip_bits: IpBits, host_address: Crunchy): string {
        let ret = "";
        let sep = "";
        for (let part of ip_bits.parts(host_address)) {
            ret += sep;
            ret += (0x10000 + part).toString(16).slice(1);
            sep = ":";
        }
        return ret;
    }

}

export default IpBits;