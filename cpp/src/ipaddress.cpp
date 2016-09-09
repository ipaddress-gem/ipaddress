
#include "ipaddress.hpp"

#include "ipv4.hpp"
#include "ipv6.hpp"
#include "ipv6_mapped.hpp"

std::ostream& operator<<(std::ostream &o, const IPAddress &ipaddress) {
    o << "<ipaddress@" 
      << "ipbits:" << ipaddress.ip_bits << ","
      << "crunchy:" << ipaddress.host_address << ","
      << "prefix:" << ipaddress.prefix << ","
      << "mapped:"  << ipaddress.mapped << ","
      << ">";
    return o;
}

std::ostream& operator<<(std::ostream &o, const Prefix &prefix) {
    o << "<Prefix@"
      << "num:" << prefix.num << ","
      << "ip_bits:" << prefix.ip_bits << ","
      << "net_mask:" << prefix.net_mask << ","
      << ">";
      return o;
}

std::ostream& operator<<(std::ostream &o, const IpBits &ip_bits) {
    o << "<IpBits@"
    << "version:" << ip_bits.version << ","
    << "bits:" << ip_bits.bits << ","
    << "dns_bits:" << ip_bits.dns_bits << ","
    << "rev_domain:" << ip_bits.rev_domain << ","
    << "part_mod:" << ip_bits.part_mod << ","
    << "host_ofs:" << ip_bits.host_ofs << ","
    << ">";
  return o;
}
static boost::regex re_mapped(":.+\\.");
static boost::regex re_ipv4("\\.");
static boost::regex re_ipv6(":");

Result<IPAddress> IPAddress::parse(const std::string &str) {
  if (boost::regex_search(str, re_mapped)) {
    // console.log("mapped:", str);
    return Ipv6Mapped::create(str);
  } else {
    if (boost::regex_search(str, re_ipv4)) {
      // console.log("ipv4:", str);
      return Ipv4::create(str);
    } else if (boost::regex_search(str, re_ipv6)) {
      // console.log("ipv6:", str);
      return Ipv6::create(str);
    }
  }
  return Err<IPAddress>("illegal formated address");
}
