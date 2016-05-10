
extern crate num;

use num::bigint::BigInt;


impl Prefix128 for Prefix {

  IN6MASK = b"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
  //
  //  Creates a new prefix object for 128 bits IPv6 addresses
  //
  //    prefix = IPAddress::Prefix128.new 64
  //      // => 64
  //
  pub fn new(num: u8)
      if (0 <= num && num <= 128) {
          return Ok(Prefix128 { prefix:  num })
      }
      return Err(format!("Prefix must be in range 0..128, got: {}", num))
  end

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
  pub fn bits(&self) {
      self.to_u128().to_str_radix(2)
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
  pub fn to_u128() {
    return (BigInt::parse_bytes(IN6MASK, 16) >> self.host_prefix()) << self.host_prefix()
  }

  //
  //  Returns the length of the host portion
  //  of a netmask.
  //
  //    prefix = Prefix128.new 96
  //
  //    prefix.host_prefix
  //      // => 32
  //
  pub fn host_prefix(&self) {
    return 128 - self.prefix
  }
}
