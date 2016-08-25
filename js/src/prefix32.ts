class Prefix32 {
public from(my: &::prefix::Prefix, num: usize) -> Result<::prefix::Prefix, String> {
    return new(num);
}
public new(num: usize) -> Result<::prefix::Prefix, String> {
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

}
