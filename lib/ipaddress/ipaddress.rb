

class IPAddress

  # 
  # Checks if the given string is a valid IP address,
  # either IPv4 or IPv6
  #
  # Example:
  #
  #   IPAddress::valid? "2002::1"
  #     #=> true
  #
  #   IPAddress::valid? "10.0.0.256"   
  #     #=> false
  #
  def self.valid?(addr)
    valid_ipv4?(addr) || valid_ipv6?(addr)
  end

  def self.valid_ipv4?(addr)
    if /\A(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})\Z/ =~ addr
      return $~.captures.all? {|i| i.to_i < 256}
    end
    false
  end
  
  def self.valid_ipv6?(addr)
    # IPv6 (normal)
    return true if /\A[\dA-Fa-f]{1,4}(:[\dA-Fa-f]{1,4})*\Z/ =~ addr
    return true if /\A[\dA-Fa-f]{1,4}(:[\dA-Fa-f]{1,4})*::([\dA-Fa-f]{1,4}(:[\dA-Fa-f]{1,4})*)?\Z/ =~ addr
    return true if /\A::([\dA-Fa-f]{1,4}(:[\dA-Fa-f]{1,4})*)?\Z/ =~ addr
    # IPv6 (IPv4 compat)
    return true if /\A[\dA-Fa-f]{1,4}(:[\dA-Fa-f]{1,4})*:/ =~ addr && valid_ipv4?($')
    return true if /\A[\dA-Fa-f]{1,4}(:[\dA-Fa-f]{1,4})*::([\dA-Fa-f]{1,4}(:[\dA-Fa-f]{1,4})*:)?/ =~ addr && valid_ipv4?($')
    return true if /\A::([\dA-Fa-f]{1,4}(:[\dA-Fa-f]{1,4})*:)?/ =~ addr && valid_ipv4?($')
    false
  end
  
  #
  # Returns the netmask length in prefix format
  #
  #   ip = IPAddress.new("10.1.1.1/23")
  #   ip.prefix
  #     #=> 23
  #
  def prefix
    @octects.pack("C"*@octects.size).unpack("B*").first.count "1"
  end

  # 
  # Returns the IP address in human readable form
  #
  #   ip.to_s
  #     #=> "172.16.11.40/24"
  #
  #   ip6.to_s
  #     #=> "FF80::1/64"
  #
  def to_s
    @octests.
      
  end

  @ip = str.split(".").pack("CCCC").unpack("N")

  #
  # Check whether an IP represents a network or 
  # a host.
  #
  # For instance, 10.0.0.130/25 is the secondo host
  # in the 10.0.0.128/25 network.
  #
  #   ip = IPAddress("10.0.0.128/25")
  #   ip.network?
  #     #=> true
  # 
  def network?
    @ip & 
  end

  def initialize(str)
    @ip, @prefix = str.split "/" 
  end
  
end


