
use ipaddress::IPAddress;
///  The address with all zero bits is called the +unspecified+ address
///  (corresponding to 0.0.0.0 in IPv4). It should be something like this:
///
///    0000:0000:0000:0000:0000:0000:0000:0000
///
///  but, with the use of compression, it is usually written as just two
///  colons:
///
///    ::
///
///  or, specifying the netmask:
///
///    ::/128
///
///  With IPAddress, create a new unspecified IPv6 address using its own
///  subclass:
///
///    ip = IPAddress::IPv6::Unspecified.new
///
///    ip.to_s
///      ///  => "::/128"
///
///  You can easily check if an IPv6 object is an unspecified address by
///  using the IPv6/// unspecified? method
///
///    ip.unspecified?
///      ///  true
///
///  An unspecified IPv6 address can also be created with the wrapper
///  method, like we've seen before
///
///    ip = IPAddress "::"
///
///    ip.unspecified?
///      ///  true
///
///  This address must never be assigned to an interface and is to be used
///  only in software before the application has learned its host's source
///  address appropriate for a pending connection. Routers must not forward
///  packets with the unspecified address.
///
///
///  Creates a new IPv6 unspecified address
///
///    ip = IPAddress::IPv6::Unspecified.new
///
///    ip.to_s
///       ///  => "::/128"
///
#[allow(dead_code)]
pub fn new() -> IPAddress {
    return IPAddress::parse("::").unwrap();
}
