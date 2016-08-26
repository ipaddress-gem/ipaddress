
import Prefix from './prefix';
import IpBits from './ip_bits';

class Prefix32 {
    public static from(my: Prefix, num: number): Prefix {
        return Prefix32.create(num);
    }

    public static create(num: number): Prefix {
        if (0 <= num && num <= 32) {
            //static _FROM: &'static (Fn(&::prefix::Prefix, usize) -> Result<::prefix::Prefix, String>) =
            // &from;
            //static _TO_IP_STR: &'static (Fn(&Vec<u16>) -> String) = &to_ip_str;
            let ip_bits = IpBits.v4();
            let bits = ip_bits.bits;
            return new Prefix({
                num: num,
                ip_bits: ip_bits,
                net_mask: Prefix.new_netmask(num, bits),
                vt_from: from,
                //vt_to_ip_str: _TO_IP_STR,
            });
        }
        return null;
    }

}


export default Prefix32;