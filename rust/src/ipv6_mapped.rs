class IPAddress::IPv6::Mapped < IPAddress::IPv6

  //  Access the internal IPv4 address
  attr_reader :ipv4

  //
  //  Creates a new IPv6 IPv4-mapped address
  //
  //    ip6 = IPAddress::IPv6::Mapped.new "::ffff:172.16.10.1/128"
  //
  //    ipv6.ipv4.class
  //      // => IPAddress::IPv4
  //
  //  An IPv6 IPv4-mapped address can also be created using the
  //  IPv6 only format of the address:
  //
  //    ip6 = IPAddress::IPv6::Mapped.new "::0d01:4403"
  //
  //    ip6.to_string
  //      // => "::ffff:13.1.68.3"
  //
  pub fn initialize(str)
    string, netmask = str.split("/")
    if string =~ /\./ //  IPv4 in dotted decimal form
      @ipv4 = IPAddress::IPv4.extract(string)
    else //  IPv4 in hex form
      groups = IPAddress::IPv6.groups(string)
      @ipv4 = IPAddress::IPv4.parse_u32((groups[-2]<< 16)+groups[-1])
    end
    super("::ffff:// {@ipv4.to_ipv6}/// {netmask}")
  end

  //
  //  Similar to IPv6// to_s, but prints out the IPv4 address
  //  in dotted decimal format
  //
  //    ip6 = IPAddress "::ffff:172.16.10.1/128"
  //
  //    ip6.to_s
  //      // => "::ffff:172.16.10.1"
  //
  pub fn to_s
    "::ffff:// {@ipv4.address}"
  end

  //
  //  Similar to IPv6// to_string, but prints out the IPv4 address
  //  in dotted decimal format
  //
  //
  //    ip6 = IPAddress "::ffff:172.16.10.1/128"
  //
  //    ip6.to_string
  //      // => "::ffff:172.16.10.1/128"
  //
  pub fn to_string
    "::ffff:// {@ipv4.address}/// @prefix"
  end

  //
  //  Checks if the IPv6 address is IPv4 mapped
  //
  //    ip6 = IPAddress "::ffff:172.16.10.1/128"
  //
  //    ip6.mapped?
  //      // => true
  //
  pub fn mapped?
    true
  end
end //  class IPv6::Mapped
