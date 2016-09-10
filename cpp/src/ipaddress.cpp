
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


std::vector<IPAddress> IPAddress::aggregate(const std::vector<IPAddress> &networks) {
  if (networks.size() == 0) {
    return {};
  }
  if (networks.size() == 1) {
    // console.log("aggregate:", networks[0], networks[0].network());
    return { networks[0].network() };
  }
  auto tmp = IPAddress::to_network_vec(networks);
  std::sort (tmp.begin(), tmp.end(), [](const IPAddress &a, const IPAddress &b) {
      return a.lt(b);
  });
  boost::container::stable_vector<IPAddress> stack(tmp.begin(), tmp.end());
  // console.log(IPAddress::to_string_vec(stack));
  // for i in 0..networks.size() {
  //     println!("{}==={}", &networks[i].to_string_uncompressed(),
  //         &stack[i].to_string_uncompressed());
  // }
  for (ssize_t pos = 0; true;) {
    if (pos < 0) {
      pos = 0;
    }
    auto stack_len = static_cast<ssize_t>(stack.size()); // borrow checker
    // println!("loop:{}:{}", pos, stack_len);
    // if stack_len == 1 {
    //     println!("exit 1");
    //     break;
    // }
    if (pos >= stack_len) {
      // println!("exit first:{}:{}", stack_len, pos);
      break;
    }
    auto first = IPAddress::pos_to_idx(pos, stack_len);
    pos = pos + 1;
    if (pos >= stack_len) {
      // println!("exit second:{}:{}", stack_len, pos);
      break;
    }
    auto second = IPAddress::pos_to_idx(pos, stack_len);
    pos = pos + 1;
    //auto firstUnwrap = first;
    if (stack[first].includes(stack[second])) {
      pos = pos - 2;
      // println!("remove:1:{}:{}:{}=>{}", first, second, stack_len, pos + 1);
      auto pidx = IPAddress::pos_to_idx(pos + 1, stack_len);
      // std::cout << "-1:" << pidx << ":" << stack.size() << std::endl;
      stack.erase(stack.begin() + pidx);
    } else {
      stack[first].prefix = stack[first].prefix.sub(1).unwrap();
      // println!("complex:{}:{}:{}:{}:P1:{}:P2:{}", pos, stack_len,
      // first, second,
      // stack[first].to_string(), stack[second].to_string());
      if ((stack[first].prefix.num + 1) == stack[second].prefix.num &&
          stack[first].includes(stack[second])) {
        pos = pos - 2;
        auto idx = IPAddress::pos_to_idx(pos, stack_len);
        stack[idx] = stack[first].clone(); // kaputt
        auto pidx = IPAddress::pos_to_idx(pos + 1, stack_len);
        // std::cout << "-1:" << pidx << ":" << stack.size() << std::endl;
        stack.erase(stack.begin() + pidx);
        // println!("remove-2:{}:{}", pos + 1, stack_len);
        pos = pos - 1; // backtrack
      } else {
        stack[first].prefix = stack[first].prefix.add(1).unwrap(); //reset prefix
        // println!("easy:{}:{}=>{}", pos, stack_len, stack[first].to_string());
        pos = pos - 1; // do it with second as first
      }
    }
  }
  // println!("agg={}:{}", pos, stack.size());
  //return stack.erase(0, stack.size());
  return std::vector<IPAddress>(stack.begin(), stack.end());
}
