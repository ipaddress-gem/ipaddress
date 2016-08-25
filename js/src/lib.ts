extern crate num;
extern crate num_traits;
extern crate num_integer;
extern crate regex;
extern crate core;

// use num::bigint::BigUint;


// const IN4MASK:  &'static [u8; 8] = b"FFFFFFFF";
// const IN6MASK : &'static [u8; 32] = b"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";


pub mod rle;
pub mod ip_bits;
//pub use ip_bits::IpBits;
pub mod prefix;
//pub use prefix::Prefix;

mod ipaddress;
pub use ipaddress::IPAddress;


pub mod prefix128;
pub mod ipv6;
pub mod ipv6_loopback;
pub mod ipv6_mapped;
pub mod ipv6_unspec;

pub mod prefix32;
pub mod ipv4;

//pub use prefix32::Prefix32;
//pub use prefix128::Prefix128;
