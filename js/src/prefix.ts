
class Prefix {
    num: number;
    ip_bits: IpBits;
    net_mask: Crunchy;
    vt_from: fn(&Prefix, usize) -> Result<Prefix, String>,

    public clone(&self) -> Prefix {
        Prefix {
            num: self.num,
            ip_bits: self.ip_bits.clone(),
            net_mask: self.net_mask.clone(),
            vt_from: self.vt_from
        }
    }
    fn eq(&self, other: &Self) -> bool {
        return self.ip_bits.version == other.ip_bits.version &&
          self.num == self.num;
    }
    fn ne(&self, other: &Self) -> bool {
        !self.eq(other)
    }
    fn cmp(&self, oth: & Prefix) -> Ordering {
        if self.ip_bits.version < oth.ip_bits.version {
            Ordering::Less
        } else if self.ip_bits.version > oth.ip_bits.version {
            Ordering::Greater
        } else {
            if self.num < oth.num {
                Ordering::Less
            } else if self.num > oth.num {
                Ordering::Greater
            } else {
                Ordering::Equal
            }
        }
    }
    //#[allow(dead_code)]
    public from(&self, num: usize) -> Result<Prefix, String>{
        return (self.vt_from)(self, num)
    }

    #[allow(dead_code)]
    public to_ip_str(&self) -> String {
        return (self.ip_bits.vt_as_compressed_string)(&self.ip_bits, &self.netmask())
    }

    #[allow(dead_code)]
    public size(&self) -> BigUint {
      return BigUint::one() << (self.ip_bits.bits-self.num.to_usize().unwrap())
    }

    public new_netmask(prefix: usize, bits: usize) -> BigUint {
        let mut mask = BigUint::zero();
        let host_prefix = bits-prefix;
        for i in 0..prefix {
            mask = mask + (BigUint::one() << (host_prefix+i));
        }
        return mask
    }

    #[allow(dead_code)]
    //#[allow(unused_variables)]
    public netmask(&self) -> BigUint {
        self.net_mask.clone()
    }

    #[allow(dead_code)]
    public get_prefix(&self) -> usize {
        return self.num
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
    #[allow(dead_code)]
    public host_mask(&self) -> BigUint {
        let mut ret = BigUint::zero();
        for _ in 0..(self.ip_bits.bits-self.num) {
            ret = ret.shl(1).add(BigUint::one());
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
    #[allow(dead_code)]
    public host_prefix(&self) -> usize {
        return (self.ip_bits.bits) - self.num;
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
    #[allow(dead_code)]
    public bits(&self) -> String {
        return self.netmask().to_str_radix(2)
    }
    // #[allow(dead_code)]
    // public net_mask(&self) -> BigUint {
    //     return (self.in_mask.clone() >> (self.host_prefix() as usize)) << (self.host_prefix() as usize);
    // }

    #[allow(dead_code)]
    public to_s(&self) -> String {
        return format!("{}", self.get_prefix());
    }
    //#[allow(dead_code)]
    // public inspect(&self) -> String {
    //     return self.to_s();
    // }
    #[allow(dead_code)]
    public to_i(&self) -> usize {
        return self.get_prefix();
    }

    #[allow(dead_code)]
    public add_prefix(&self, other: &Prefix) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other.get_prefix())
    }
    #[allow(dead_code)]
    public add(&self, other: usize) -> Result<Prefix, String> {
        self.from(self.get_prefix() + other)
    }
    #[allow(dead_code)]
    public sub_prefix(&self, other: &Prefix) -> Result<Prefix, String> {
        return self.sub(other.get_prefix());
    }
    #[allow(dead_code)]
    public sub(&self, other: usize) -> Result<Prefix, String> {
        if other > self.get_prefix() {
            return self.from(other-self.get_prefix());
        }
        return self.from(self.get_prefix() - other);
    }

}
