import Prefix from './prefix';
import IpBits from './ip_bits';

class Prefix128 {
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
    public static create(num: number): Prefix {
        if (num <= 128) {
            //static _FROM: &'static (Fn(&Prefix, usize) -> Result<Prefix, String>) = &from;
            //static _TO_IP_STR: &'static (Fn(&Vec<u16>) -> String) = &Prefix128::to_ip_str;
            let ip_bits = IpBits.v6();
            let bits = ip_bits.bits;
            return new Prefix({
                num: num,
                ip_bits: ip_bits,
                net_mask: Prefix.new_netmask(num, bits),
                vt_from: Prefix128.from, // vt_to_ip_str: _TO_IP_STR
            });
        }
        return null;
    }

    public static from(my: Prefix, num: number) : Prefix {
        return Prefix128.create(num);
    }
}


export default Prefix128;
