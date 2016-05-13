


use num::bigint::BigInt;

//#[derive(Ord,PartialOrd,Eq,PartialEq,Debug,Copy,Clone)]
pub struct Prefix128 {
}

impl Prefix128 {
  //
  //  Creates a new prefix object for 128 bits IPv6 addresses
  //
  //    prefix = IPAddress::Prefix128.new 64
  //      // => 64
  //
  pub fn new(num: u8) -> Result<::Prefix, String> {
      if 0 <= num && num <= 128 {
          static _from: &'static (Fn(&::Prefix, u8) -> Result<::Prefix, String>) = &Prefix128::from;
          static _host_prefix: &'static (Fn(&::Prefix) -> u8) = &Prefix128::host_prefix;
          static _bits: &'static (Fn(&::Prefix) -> String) = &Prefix128::bits;
          static _to_ip_str: &'static (Fn(&::Prefix) -> String) = &Prefix128::to_ip_str;
          static _to_number: &'static (Fn(&::Prefix) -> BigInt) = &Prefix128::to_number;
          return Ok(::Prefix{
              num: num,
              vt_from: _from,
              vt_host_prefix: _host_prefix,
              vt_bits: _bits,
              vt_to_ip_str: _to_ip_str,
              vt_to_number: _to_number,
          })
      }
      return Err(format!("Prefix must be in range 0..128, got: {}", num))
  }

  pub fn from(my: &::Prefix, num: u8) -> Result<::Prefix, String> {
      return Prefix128::new(num)
  }

  pub fn to_ip_str(my: &::Prefix) -> String {
      return String::from("fuckoff")
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
  pub fn bits(my: &::Prefix) -> String {
      return my.to_number().to_str_radix(2)
  }

  //
  //  Unsigned 128 bits decimal number representing
  //  the prefix
  //
  //    prefix = IPAddress::Prefix128.new 64
  //
  //    prefix.to_u128
  //      // => 340282366920938463444927863358058659840
  //
  pub fn to_number(my: &::Prefix) -> BigInt {
    return (BigInt::parse_bytes(::IN6MASK, 16).unwrap()
        >> (my.host_prefix() as usize)) << (my.host_prefix() as usize)
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
  pub fn host_prefix(my: &::Prefix) -> u8 {
    return 128 - my.num
  }
}
