extern crate num;
extern crate num_traits;
extern crate num_integer;

// use num::bigint::BigUint;


// const IN4MASK:  &'static [u8; 8] = b"FFFFFFFF";
// const IN6MASK : &'static [u8; 32] = b"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";


mod prefix;
pub use prefix::Prefix;
mod prefix32;
pub use prefix32::Prefix32;
mod prefix128;
pub use prefix128::Prefix128;
