impl IPAddress {

  //
  // Parse the argument string to create a new
  // IPv4, IPv6 or Mapped IP object
  //
  //   ip  = IPAddress.parse "172.16.10.1/24"
  //   ip6 = IPAddress.parse "2001:db8::8:800:200c:417a/64"
  //   ip_mapped = IPAddress.parse "::ffff:172.16.10.1/128"
  //
  // All the object created will be instances of the
  // correct class:
  //
  //  ip.class
  //    //=> IPAddress::IPv4
  //  ip6.class
  //    //=> IPAddress::IPv6
  //  ip_mapped.class
  //    //=> IPAddress::IPv6::Mapped
  //
  pub fn parse(str:: String) {
    case str
    when /:.+\./
      IPAddress::IPv6::Mapped.new(str)
    when /\./
      IPAddress::IPv4.new(str)
    when /:/
      IPAddress::IPv6.new(str)
    else
      raise ArgumentError, "Unknown IP Address //{str}"
    end
  }

  //
  // True if the object is an IPv4 address
  //
  //   ip = IPAddress("192.168.10.100/24")
  //
  //   ip.ipv4?
  //     //-> true
  //
  pub fn is_ipv4(&self) {
    self.kind_of? IPAddress::IPv4
  }

  //
  // True if the object is an IPv6 address
  //
  //   ip = IPAddress("192.168.10.100/24")
  //
  //   ip.ipv6?
  //     //-> false
  //
  pub fn is_ipv6(&self) {
    self.kind_of? IPAddress::IPv6
  }

  //
  // Checks if the given string is a valid IP address,
  // either IPv4 or IPv6
  //
  // Example:
  //
  //   IPAddress::valid? "2002::1"
  //     //=> true
  //
  //   IPAddress::valid? "10.0.0.256"
  //     //=> false
  //
  pub fn is_valid(addr: String) {
    valid_ipv4?(addr) || valid_ipv6?(addr)
  }

  //
  // Checks if the given string is a valid IPv4 address
  //
  // Example:
  //
  //   IPAddress::valid_ipv4? "2002::1"
  //     //=> false
  //
  //   IPAddress::valid_ipv4? "172.16.10.1"
  //     //=> true
  //
  pub fn valid_ipv4(addr: String) {
    if /\A(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})\Z/ =~ addr
      return $~.captures.all? {|i| i.to_i < 256}
    end
    false
  }

  //
  // Checks if the argument is a valid IPv4 netmask
  // expressed in dotted decimal format.
  //
  //   IPAddress.valid_ipv4_netmask? "255.255.0.0"
  //     //=> true
  //
  pub fn valid_ipv4_netmask(addr: String) {
    arr = addr.split(".").map{|i| i.to_i}.pack("CCCC").unpack("B*").first.scan(/01/)
    arr.empty? && valid_ipv4?(addr)
  rescue
    return false
  }

  //
  // Checks if the given string is a valid IPv6 address
  //
  // Example:
  //
  //   IPAddress::valid_ipv6? "2002::1"
  //     //=> true
  //
  //   IPAddress::valid_ipv6? "2002::DEAD::BEEF"
  //     //=> false
  //
  pub fn valid_ipv6(addr: String) {
    // https://gist.github.com/cpetschnig/294476
    // http://forums.intermapper.com/viewtopic.php?t=452
    return true if /^\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*$/ =~ addr
    false
  }


  //
  // private helper for summarize
  // assumes that networks is output from reduce_networks
  // means it should be sorted lowers first and uniq
  //
  pub fn aggregate(networks: &[]) {
    stack = networks.map{|i| i.network }.sort! // make input imutable
    pos = 0
    while true
      pos = pos < 0 ? 0 : pos // start @beginning
      first = stack[pos]
      unless first
        break
      end
      pos += 1
      second = stack[pos]
      unless second
        break
      end
      pos += 1
      if first.include?(second)
        pos -= 2
        stack.delete_at(pos+1)
      else
        first.prefix -= 1
        if first.prefix+1 == second.prefix && first.include?(second)
          pos -= 2
          stack[pos] = first
          stack.delete_at(pos+1)
          pos -= 1 // backtrack
        else
          first.prefix += 1 //reset prefix
          pos -= 1 // do it with second as first
        end
      end
    end
    stack[0..pos-1]
}

  //
  // Summarization (or aggregation) is the process when two or more
  // networks are taken together to check if a supernet, including all
  // and only these networks, exists. If it exists then this supernet
  // is called the summarized (or aggregated) network.
  //
  // It is very important to understand that summarization can only
  // occur if there are no holes in the aggregated network, or, in other
  // words, if the given networks fill completely the address space
  // of the supernet. So the two rules are:
  //
  // 1) The aggregate network must contain +all+ the IP addresses of the
  //    original networks;
  // 2) The aggregate network must contain +only+ the IP addresses of the
  //    original networks;
  //
  // A few examples will help clarify the above. Let's consider for
  // instance the following two networks:
  //
  //   ip1 = IPAddress("172.16.10.0/24")
  //   ip2 = IPAddress("172.16.11.0/24")
  //
  // These two networks can be expressed using only one IP address
  // network if we change the prefix. Let Ruby do the work:
  //
  //   IPAddress::IPv4::summarize(ip1,ip2).to_s
  //     //=> "172.16.10.0/23"
  //
  // We note how the network "172.16.10.0/23" includes all the addresses
  // specified in the above networks, and (more important) includes
  // ONLY those addresses.
  //
  // If we summarized +ip1+ and +ip2+ with the following network:
  //
  //   "172.16.0.0/16"
  //
  // we would have satisfied rule //1 above, but not rule //2. So "172.16.0.0/16"
  // is not an aggregate network for +ip1+ and +ip2+.
  //
  // If it's not possible to compute a single aggregated network for all the
  // original networks, the method returns an array with all the aggregate
  // networks found. For example, the following four networks can be
  // aggregated in a single /22:
  //
  //   ip1 = IPAddress("10.0.0.1/24")
  //   ip2 = IPAddress("10.0.1.1/24")
  //   ip3 = IPAddress("10.0.2.1/24")
  //   ip4 = IPAddress("10.0.3.1/24")
  //
  //   IPAddress::IPv4::summarize(ip1,ip2,ip3,ip4).to_string
  //     //=> "10.0.0.0/22",
  //
  // But the following networks can't be summarized in a single network:
  //
  //   ip1 = IPAddress("10.0.1.1/24")
  //   ip2 = IPAddress("10.0.2.1/24")
  //   ip3 = IPAddress("10.0.3.1/24")
  //   ip4 = IPAddress("10.0.4.1/24")
  //
  //   IPAddress::IPv4::summarize(ip1,ip2,ip3,ip4).map{|i| i.to_string}
  //     //=> ["10.0.1.0/24","10.0.2.0/23","10.0.4.0/24"]
  //
  pub fn summarize(networks: []) {
    aggregate(networks.map{|i| ((i.kind_of?(String)&&IPAddress.parse(i))||i) })
  }

} // module IPAddress

//
// IPAddress is a wrapper method built around
// IPAddress's library classes. Its purpouse is to
// make you indipendent from the type of IP address
// you're going to use.
//
// For example, instead of creating the three types
// of IP addresses using their own contructors
//
//   ip  = IPAddress::IPv4.new "172.16.10.1/24"
//   ip6 = IPAddress::IPv6.new "2001:db8::8:800:200c:417a/64"
//   ip_mapped = IPAddress::IPv6::Mapped "::ffff:172.16.10.1/128"
//
// you can just use the IPAddress wrapper:
//
//   ip  = IPAddress "172.16.10.1/24"
//   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
//   ip_mapped = IPAddress "::ffff:172.16.10.1/128"
//
// All the object created will be instances of the
// correct class:
//
//  ip.class
//    //=> IPAddress::IPv4
//  ip6.class
//    //=> IPAddress::IPv6
//  ip_mapped.class
//    //=> IPAddress::IPv6::Mapped
//
// def IPAddress(str)
//   IPAddress::parse str
// end

//
// Compatibility with Ruby 1.8
//
// if RUBY_VERSION =~ /1\.8/
//   class Hash // :nodoc:
//     alias :key :index
//   end
//   module Math // :nodoc:
//     def Math.log2(n)
//       log(n) / log(2)
//     end
//   end
// end
