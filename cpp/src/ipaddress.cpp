
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

Result<size_t> IPAddress::parse_str(const std::string &str, size_t radix) {
  std::stringstream s2;
  if (radix == 10) {
    s2 << std::dec;
  } else if (radix == 16) {
    s2 << std::hex;
  } else {
    return Err<size_t>("unknown radix");
  }
  s2 << str;
  size_t ret;
  s2 >> ret;
  if (s2.fail()) {
    return Err<size_t>("not a decimal number");
  }
  //std::cout << "parse_str:" << str << ":" << ret << std::endl;
  return Ok(ret);
}

static boost::regex re_dec("^\\d+$");
Result<size_t> IPAddress::parse_dec_str(const std::string &str) {
  if (boost::regex_match(str, re_dec)) {
    return IPAddress::parse_str(str, 10);
  }
  return Err<size_t>("not a decimal string");
}
static boost::regex re_hex("^[0-9a-fA-F]+$");
Result<size_t> IPAddress::parse_hex_str(const std::string &str) {
  if (boost::regex_match(str, re_hex)) {
    return IPAddress::parse_str(str, 16);
  }
  return Err<size_t>("not a hex string");
}
