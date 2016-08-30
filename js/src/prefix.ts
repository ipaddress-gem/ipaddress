
import IpBits from './ip_bits';
import Crunchy from './crunchy';

interface From {
    (source: Prefix, num: number): Prefix;
}


class Prefix {
    num: number;
    ip_bits: IpBits;
    net_mask: Crunchy;
    vt_from: From;

    constructor(obj: {[id:string] : any}) {
       this.num = obj['num']; 
       this.ip_bits = obj['ip_bits']; 
       this.net_mask = obj['net_mask']; 
       this.vt_from = obj['vt_from']; 
    }

    public clone() : Prefix {
        return new Prefix({
            num: this.num,
            ip_bits: this.ip_bits,
            net_mask: this.net_mask,
            vt_from: this.vt_from
        });
    }

    public eq(other: Prefix) : boolean {
        return this.ip_bits.version == other.ip_bits.version &&
          this.num == other.num;
    }
    public ne(other: Prefix) : boolean {
        return !this.eq(other);
    }
    public cmp(oth: Prefix) : number {
        if (this.ip_bits.version < oth.ip_bits.version) {
            return -1;
        } else if (this.ip_bits.version > oth.ip_bits.version) {
            return 1;
        } else {
            if (this.num < oth.num) {
                return -1;
            } else if (this.num > oth.num) {
                return 1;
            } else {
                return 0;
            }
        }
    }
    //#[allow(dead_code)]
    public from(num: number) : Prefix {
        return (this.vt_from)(this, num);
    }

    public to_ip_str() : string {
        return this.ip_bits.vt_as_compressed_string(this.ip_bits, this.net_mask);
    }

    public size() : Crunchy {
      return Crunchy.one().shl(this.ip_bits.bits-this.num);
    }

    public static new_netmask(prefix: number, bits: number) : Crunchy {
        let mask = Crunchy.zero();
        let host_prefix = bits-prefix;
        for (let i=0; i < prefix; ++i) {
            // console.log(">>>", i, host_prefix, mask);
            mask = mask.add(Crunchy.one().shl(host_prefix+i));
        }
        return mask
    }

    public netmask() : Crunchy {
        return this.net_mask;
    }

    public get_prefix() : number {
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
    public host_mask() : Crunchy {
        let ret = Crunchy.zero();
        for(let _ = 0; _ < (this.ip_bits.bits-this.num); _++) {
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
    public host_prefix() : number {
        return this.ip_bits.bits - this.num;
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
    public bits() : string {
        return this.netmask().toString(2);
    }
    // #[allow(dead_code)]
    // public net_mask(&self) -> BigUint {
    //     return (self.in_mask.clone() >> (self.host_prefix() as usize)) << (self.host_prefix() as usize);
    // }

    public to_s() : string {
        return this.get_prefix().toString();
    }
    //#[allow(dead_code)]
    // public inspect(&self) -> String {
    //     return self.to_s();
    // }
    public to_i() : number {
        return this.get_prefix();
    }

    public add_prefix(other: Prefix) : Prefix {
        return this.from(this.get_prefix() + other.get_prefix());
    }

    public add(other: number) : Prefix {
        return this.from(this.get_prefix() + other)
    }

    public sub_prefix(other: Prefix) : Prefix {
        return this.sub(other.get_prefix());
    }

    public sub(other: number) : Prefix {
        if (other > this.get_prefix()) {
            return this.from(other-this.get_prefix());
        }
        return this.from(this.get_prefix() - other);
    }

}

export default Prefix;