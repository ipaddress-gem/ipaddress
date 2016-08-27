
use core::result::Result;

#[allow(unused_variables)]
fn from(my: &::prefix::Prefix, num: usize) -> Result<::prefix::Prefix, String> {
    return new(num);
}
///  Gives the prefix in IPv4 dotted decimal format,
///  i.e. the canonical netmask we're all used to
///
///    prefix = IPAddress::prefix::Prefix32.new 24
///
///    prefix.to_ip
///      ///  "255.255.255.0"
///
#[allow(unused_comparisons)]
pub fn new(num: usize) -> Result<::prefix::Prefix, String> {
    if 0 <= num && num <= 32 {
        //static _FROM: &'static (Fn(&::prefix::Prefix, usize) -> Result<::prefix::Prefix, String>) =
            &from;
        //static _TO_IP_STR: &'static (Fn(&Vec<u16>) -> String) = &to_ip_str;
        let ip_bits = ::ip_bits::v4();
        let bits = ip_bits.bits;
        return Ok(::prefix::Prefix {
            num: num,
            ip_bits: ip_bits,
            net_mask: ::prefix::Prefix::new_netmask(num, bits),
            vt_from: from,
            //vt_to_ip_str: _TO_IP_STR,
        });
    }
    return Err(format!("Prefix must be in range 0..32, got: {}", num));
}
