import * as Crunch from 'node-crunch';

import Rle from 'rle';

type Crunchy = number[];
enum IpVersion {
    V4 = 4,
    V6 = 6
}

// #[derive(Debug, Clone)]
class IpBits {
    version: IpVersion;
    //pub vt_as_compressed_string: fn(&IpBits,this.Crunchy) -> String,
    //pub vt_as_uncompressed_string: fn(&IpBits,this.Crunchy) -> String,
    bits: number;
    part_bits: number;
    dns_bits: number;
    rev_domain: string;
    part_mod: number;
    host_ofs: number; // ipv4=1, ipv6=0

    public clone() : IpBits {
        let my = new IpBits();
        my.version = this.version;
        my.vt_as_compressed_string = this.vt_as_compressed_string;
        my.vt_as_uncompressed_string = this.vt_as_uncompressed_string;
        my.bits = this.bits;
        my.part_bits = this.part_bits;
        my.dns_bits = this.dns_bits;
        my.rev_domain = this.rev_domain;
        my.part_mod = this.part_mod;
        my.host_ofs = this.host_ofs;
        return my;
    }

    public parts(bu: Crunchy) : Crunchy {
        let vec: number[] = [];
        let my = bu.slice();
        let part_mod = Crunch.leftShift([1], this.part_bits);// - Crunchy::one();
        for(let i = 0; i < (this.bits / this.part_bits); ++i) {
            vec.push(0+Crunch.stringify(Crunch.mod(my, part_mod)));
            my = Crunch.rightShift(my, this.part_bits);
        }
        return vec.reverse();
    }

    public as_compressed_string(bu: Crunchy) : String {
        return (this.vt_as_compressed_string)(this, bu);
    }
    public as_uncompressed_string(bu:this.Crunchy) : String {
        return (this.vt_as_uncompressed_string)(this, bu);
    }

    public dns_part_format(i: number) : String {
        switch(this.version) {
            case IpVersion.V4: return `${i}`;
            case IpVersion.V6: return `${i.toString(16)}`;
        }
    }

    public static v4() : IpBits {
        let my = new IpBits();
        my.version = IpVersion.V4;
        my.vt_as_compressed_string = ipv4_as_compressed;
        my.vt_as_uncompressed_string = ipv4_as_compressed;
        my.bits = 32;
        my.part_bits = 8;
        my.dns_bits = 8;
        my.rev_domain = "in-addr.arpa";
        my.part_mod = 1 << 8;
        my.host_ofs = 1;
        return my;
    }

    public static v6() : IpBits {
        let my = new IpBits();
        my.version: IpVersion.V6;
        my.vt_as_compressed_string = ipv6_as_compressed;
        my.vt_as_uncompressed_string = ipv6_as_uncompressed;
        my.bits = 128;
        my.part_bits = 16;
        my.dns_bits = 4;
        my.rev_domain = "ip6.arpa";
        my.part_mod = 1 << 16;
        my.host_ofs = 0;
        return my;
    }


    public static ipv4_as_compressed(ip_bits: IpBits, host_address: Crunchy) : string {
        let ret = "";
        let sep = "";
        for (let part in ip_bits.parts(host_address)) {
            ret += sep;
            ret += `${part}`;
            sep = ".";
        }
        return ret;
    }
    public static ipv6_as_compressed(ip_bits:IpBits, host_address:Crunchy) : string {
        //println!("ipv6_as_compressed:{}", host_address);
        let ret = "";
        let colon = "";
        for (let rle in Rle.code(ip_bits.parts(host_address))) {
            // println!(">>{:?}", rle);
            for (let _ = 0; _ < rle.cnt; _++) {
               if (!(rle.part == 0 && rle.max)) {
                ret += &format!("{}{:x}", colon, rle.part));
                colon = &the_colon;
            } else if rle.part == 0 && rle.max {
                ret.push_str("::");
                colon = &the_empty;
                break;
            }
            }
        }
        return ret;
    }
    public static ipv6_as_uncompressed(ip_bits:this.IpBits, host_address:this.Crunchy) -> String {
        let ret = String::new();
        let sep = "";
        for part in ip_bits.parts(host_address) {
            ret.push_str(sep);
            ret.push_str(&format!("{:04x}", part));
            sep = ":";
        }
        return ret;
    }


}
