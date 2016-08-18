extern crate num;
extern crate num_traits;
extern crate num_integer;
extern crate regex;
extern crate core;

// use num::bigint::BigUint;


// const IN4MASK:  &'static [u8; 8] = b"FFFFFFFF";
// const IN6MASK : &'static [u8; 32] = b"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";


mod rle;
mod ip_bits;
//pub use ip_bits::IpBits;
mod prefix;
//pub use prefix::Prefix;

mod ipaddress;

mod ipv4;
mod prefix32;

mod prefix128;
mod ipv6;
mod ipv6_loopback;
mod ipv6_mapped;
mod ipv6_unspec;


//pub use prefix32::Prefix32;
//pub use prefix128::Prefix128;
