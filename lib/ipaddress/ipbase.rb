require 'ipaddress/extensions/extensions'

module IPAddress

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

  #
  # Checks if the given string is a valid IPv4 address
  #
  # Example:
  #
  #   IPAddress::valid_ipv4? "2002::1"
  #     #=> false
  #
  #   IPAddress::valid_ipv4? "172.16.10.1"
  #     #=> true
  #
  def self.valid_ipv4?(addr)
    if /\A(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})\Z/ =~ addr
      return $~.captures.all? {|i| i.to_i < 256}
    end
    false
  end
  
  #
  # Checks if the argument is a valid IPv4 netmark
  # expressed in dotted decimal format.
  #
  #   IPAddress.valid_ipv4_netmask? "255.255.0.0"
  #     #=> true
  #
  def self.valid_ipv4_netmask?(addr)
    arr = addr.split(".").map{|i| i.to_i}.pack("CCCC").unpack("B*").first.scan(/01/)
    arr.empty? && valid_ipv4?(addr)
  rescue
    return false
  end
  
  #
  # Checks if the given string is a valid IPv6 address
  #
  # Example:
  #
  #   IPAddress::valid_ipv6? "2002::1"
  #     #=> true
  #
  #   IPAddress::valid_ipv6? "2002::DEAD::BEEF"
  #     #=> false
  #
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
  
end # module IPAddress





