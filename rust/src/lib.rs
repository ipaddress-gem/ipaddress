extern crate num;
extern crate num_traits;

pub fn to_ipv4_str(value: u32) -> String {
  return format!("{}.{}.{}.{}",
      (value >> 24) & 0xff,
      (value >> 16) & 0xff,
      (value >> 8) & 0xff,
       value & 0xff)
}

const IN4MASK:  &'static [u8; 8] = b"FFFFFFFF";
const IN6MASK : &'static [u8; 32] = b"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";


mod prefix;
pub use prefix::Prefix;
mod prefix32;
pub use prefix32::Prefix32;
mod prefix128;
pub use prefix128::Prefix128;
