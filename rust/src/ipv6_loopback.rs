
use ipaddress::IPAddress;
use num::bigint::BigUint;
use num_traits::One;

///    The loopback  address is a unicast localhost address. If an
///  application in a host sends packets to this address, the IPv6 stack
///  will loop these packets back on the same virtual interface.
///
///  Loopback addresses are expressed in the following form:
///
///    ::1
///
///  or, with their appropriate prefix,
///
///    ::1/128
///
///  As for the unspecified addresses, IPv6 loopbacks can be created with
///  IPAddress calling their own class:
///
///    ip = IPAddress::IPv6::Loopback.new
///
///    ip.to_string
///      ///  "::1/128"
///
///  or by using the wrapper:
///
///    ip = IPAddress "::1"
///
///    ip.to_string
///      ///  "::1/128"
///
///  Checking if an address is loopback is easy with the IPv6/// loopback?
///  method:
///
///    ip.loopback?
///      ///  true
///
///  The IPv6 loopback address corresponds to 127.0.0.1 in IPv4.
///
///
///  Creates a new IPv6 unspecified address
///
///    ip = IPAddress::IPv6::Loopback.new
///
///    ip.to_string
///      ///  "::1/128"
///
#[allow(dead_code)]
pub fn new() -> IPAddress {
    return ::ipv6::from_int(BigUint::one(), 128).unwrap();
}
