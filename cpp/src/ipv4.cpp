
#include "ipaddress.hpp"
#include "ipv6.hpp"
#include "prefix128.hpp"

IPAddress Ipv4::to_ipv6(const IPAddress &ia) {
    return IPAddress(
        IpBits::v6(),
        ia.host_address.clone(),
        Prefix128::create(ia.prefix.num).unwrap(),
        None<IPAddress>(),
        Ipv6::ipv6_is_private,
        Ipv6::ipv6_is_loopback,
        Ipv6::to_ipv6
    );
}
