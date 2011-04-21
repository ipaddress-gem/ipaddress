#
# = IPAddress
#
# A ruby library to manipulate IPv4 and IPv6 addresses
#
#
# Package::     IPAddress
# Author::      Marco Ceresa <ceresa@ieee.org>
# License::     Ruby License
#
#--
#
#++

module IPAddress

  NAME            = "IPAddress"
  GEM             = "ipaddress"
  AUTHORS         = ["Marco Ceresa <ceresa@ieee.org>"]

  #
  # Parse the argument string to create a new
  # IPv4, IPv6 or Mapped IP object
  #
  #   ip  = IPAddress.parse "172.16.10.1/24"
  #   ip6 = IPAddress.parse "2001:db8::8:800:200c:417a/64"
  #   ip_mapped = IPAddress.parse "::ffff:172.16.10.1/128"
  #
  # All the object created will be instances of the 
  # correct class:
  #
  #  ip.class
  #    #=> IPAddress::IPv4
  #  ip6.class
  #    #=> IPAddress::IPv6
  #  ip_mapped.class
  #    #=> IPAddress::IPv6::Mapped
  #
  def IPAddress::parse(str)
    case str
    when /:.+\./
      IPAddress::IPv6::Mapped.new(str)
    else
      IPAddress::IPv4.new(str) rescue IPAddress::IPv6.new(str)
    end
  end

  #
  # True if the object is an IPv4 address
  #
  #   ip = IPAddress("192.168.10.100/24")
  #
  #   ip.ipv4?
  #     #-> true
  #
  def ipv4?
    self.kind_of? IPAddress::IPv4
  end
  
  #
  # True if the object is an IPv6 address
  #
  #   ip = IPAddress("192.168.10.100/24")
  #
  #   ip.ipv6?
  #     #-> false
  #
  def ipv6?
    self.kind_of? IPAddress::IPv6
  end

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
  # Checks if the argument is a valid IPv4 netmask
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

  def self.deprecate(message = nil) # :nodoc:
    message ||= "You are using deprecated behavior which will be removed from the next major or minor release."
    warn("DEPRECATION WARNING: #{message}")
  end
  
end # module IPAddress

#
# IPAddress is a wrapper method built around 
# IPAddress's library classes. Its purpouse is to 
# make you indipendent from the type of IP address 
# you're going to use.
#
# For example, instead of creating the three types 
# of IP addresses using their own contructors
#
#   ip  = IPAddress::IPv4.new "172.16.10.1/24"
#   ip6 = IPAddress::IPv6.new "2001:db8::8:800:200c:417a/64"
#   ip_mapped = IPAddress::IPv6::Mapped "::ffff:172.16.10.1/128" 
#
# you can just use the IPAddress wrapper:
#
#   ip  = IPAddress "172.16.10.1/24"
#   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
#   ip_mapped = IPAddress "::ffff:172.16.10.1/128"
#
# All the object created will be instances of the 
# correct class:
#
#  ip.class
#    #=> IPAddress::IPv4
#  ip6.class
#    #=> IPAddress::IPv6
#  ip_mapped.class
#    #=> IPAddress::IPv6::Mapped
#
def IPAddress(str)
  IPAddress::parse str
end

module IPAddress
  
  #
  # =NAME
  #   
  # IPAddress::Prefix
  #
  # =SYNOPSIS
  #  
  # Parent class for Prefix32 and Prefix128
  #
  # =DESCRIPTION
  #
  # IPAddress::Prefix is the parent class for IPAddress::Prefix32 
  # and IPAddress::Prefix128, defining some modules in common for
  # both the subclasses.
  #
  # IPAddress::Prefix shouldn't be accesses directly, unless
  # for particular needs.
  #
  class Prefix

    include Comparable

    attr_reader :prefix

    #
    # Creates a new general prefix
    #
    def initialize(num)
      @prefix = num.to_i
    end

    #
    # Returns a string with the prefix 
    #
    def to_s
      "#@prefix"
    end
    alias_method :inspect, :to_s

    # 
    # Returns the prefix
    #
    def to_i
      @prefix
    end

    # 
    # Compare the prefix
    #
    def <=>(oth)
      @prefix <=> oth.to_i
    end

    #
    # Sums two prefixes or a prefix to a 
    # number, returns a Fixnum
    #
    def +(oth)
      if oth.is_a? Fixnum
        self.prefix + oth
      else
        self.prefix + oth.prefix
      end
    end

    #
    # Returns the difference between two
    # prefixes, or a prefix and a number,
    # as a Fixnum
    #
    def -(oth)
      if oth.is_a? Fixnum
        self.prefix - oth
      else
        (self.prefix - oth.prefix).abs
      end
    end
    
   end # class Prefix


  class Prefix32 < Prefix

    IN4MASK = 0xffffffff
    
    #
    # Creates a new prefix object for 32 bits IPv4 addresses
    #
    #   prefix = IPAddress::Prefix32.new 24
    #     #=> 24
    #
    def initialize(num)
      unless (0..32).include? num
        raise ArgumentError, "Prefix must be in range 0..32, got: #{num}"
      end
      super(num)
    end

    #
    # Returns the length of the host portion
    # of a netmask. 
    #
    #   prefix = Prefix32.new 24
    #
    #   prefix.host_prefix
    #     #=> 8
    #
    def host_prefix
      32 - @prefix
    end
    
    #
    # Transforms the prefix into a string of bits
    # representing the netmask
    #
    #   prefix = IPAddress::Prefix32.new 24
    # 
    #   prefix.bits 
    #     #=> "11111111111111111111111100000000"
    #
    def bits
      "%.32b" % to_u32
    end

    #
    # Gives the prefix in IPv4 dotted decimal format, 
    # i.e. the canonical netmask we're all used to
    #
    #   prefix = IPAddress::Prefix32.new 24
    #
    #   prefix.to_ip
    #     #=> "255.255.255.0"
    #
    def to_ip
      [bits].pack("B*").unpack("CCCC").join(".")
    end

    #
    # An array of octets of the IPv4 dotted decimal 
    # format 
    #
    #   prefix = IPAddress::Prefix32.new 24
    #
    #   prefix.octets
    #     #=> [255, 255, 255, 0]
    #
    def octets
      to_ip.split(".").map{|i| i.to_i}
    end

    #
    # Unsigned 32 bits decimal number representing
    # the prefix
    #
    #   prefix = IPAddress::Prefix32.new 24
    #
    #   prefix.to_u32
    #     #=> 4294967040
    #
    def to_u32
      (IN4MASK >> host_prefix) << host_prefix
    end
    
    #
    # Shortcut for the octecs in the dotted decimal 
    # representation
    #
    #   prefix = IPAddress::Prefix32.new 24
    #
    #   prefix[2]
    #     #=> 255
    #
    def [](index)
      octets[index]
    end

    #
    # The hostmask is the contrary of the subnet mask,
    # as it shows the bits that can change within the
    # hosts
    #
    #   prefix = IPAddress::Prefix32.new 24
    #
    #   prefix.hostmask
    #     #=> "0.0.0.255"
    #
    def hostmask
      [~to_u32].pack("N").unpack("CCCC").join(".")
    end
    
    #
    # Creates a new prefix by parsing a netmask in 
    # dotted decimal form
    #
    #   prefix = IPAddress::Prefix32::parse_netmask "255.255.255.0"
    #     #=> 24
    #
    def self.parse_netmask(netmask)
      octets = netmask.split(".").map{|i| i.to_i}
      num = octets.pack("C"*octets.size).unpack("B*").first.count "1"
      return self.new(num)
    end
    
  end # class Prefix32 < Prefix

  class Prefix128 < Prefix

    #
    # Creates a new prefix object for 128 bits IPv6 addresses
    #
    #   prefix = IPAddress::Prefix128.new 64
    #     #=> 64
    #
    def initialize(num=128)
      unless (1..128).include? num.to_i
        raise ArgumentError, "Prefix must be in range 1..128, got: #{num}"
      end
      super(num.to_i)
    end

    #
    # Transforms the prefix into a string of bits
    # representing the netmask
    #
    #   prefix = IPAddress::Prefix128.new 64
    #
    #   prefix.bits
    #     #=> "1111111111111111111111111111111111111111111111111111111111111111"
    #         "0000000000000000000000000000000000000000000000000000000000000000"
    #
    def bits
      "1" * @prefix + "0" * (128 - @prefix)
    end

    #
    # Unsigned 128 bits decimal number representing
    # the prefix
    #
    #   prefix = IPAddress::Prefix128.new 64
    #
    #   prefix.to_u128
    #     #=> 340282366920938463444927863358058659840
    #
    def to_u128
      bits.to_i(2)
    end

  end # class Prefix123 < Prefix

end # module IPAddress

module IPAddress; 
  # 
  # =Name
  # 
  # IPAddress::IPv4 - IP version 4 address manipulation library
  #
  # =Synopsis
  #
  #    require 'ipaddress'
  #
  # =Description
  # 
  # Class IPAddress::IPv4 is used to handle IPv4 type addresses. 
  #
  class IPv4
    
    include IPAddress
    include Enumerable  
    include Comparable                  
    
    #
    # This Hash contains the prefix values for Classful networks
    #
    # Note that classes C, D and E will all have a default 
    # prefix of /24 or 255.255.255.0
    #
    CLASSFUL = {
      /^0../ => 8,  # Class A, from 0.0.0.0 to 127.255.255.255
      /^10./ => 16, # Class B, from 128.0.0.0 to 191.255.255.255
      /^110/ => 24  # Class C, D and E, from 192.0.0.0 to 255.255.255.254
    }

    #
    # Regular expression to match an IPv4 address
    #
    REGEXP = Regexp.new(/((25[0-5]|2[0-4]\d|1\d\d|[1-9]\d|\d)\.){3}(25[0-5]|2[0-4]\d|1\d\d|[1-9]\d|\d)/)
    
    #
    # Creates a new IPv4 address object.
    #
    # An IPv4 address can be expressed in any of the following forms:
    # 
    # * "10.1.1.1/24": ip +address+ and +prefix+. This is the common and
    # suggested way to create an object                  .
    # * "10.1.1.1/255.255.255.0": ip +address+ and +netmask+. Although
    # convenient sometimes, this format is less clear than the previous
    # one.             
    # * "10.1.1.1": if the address alone is specified, the prefix will be
    # set as default 32, also known as the host prefix    
    #
    # Examples:
    #
    #   # These two are the same
    #   ip = IPAddress::IPv4.new("10.0.0.1/24")
    #   ip = IPAddress("10.0.0.1/24")
    #   
    #   # These two are the same
    #   IPAddress::IPv4.new "10.0.0.1/8"
    #   IPAddress::IPv4.new "10.0.0.1/255.0.0.0"
    #
    def initialize(str)
      ip, netmask = str.split("/")
      
      # Check the ip and remove white space
      if IPAddress.valid_ipv4?(ip)
        @address = ip.strip
      else
        raise ArgumentError, "Invalid IP #{ip.inspect}"
      end
      
      # Check the netmask
      if netmask  # netmask is defined
        netmask.strip!
        if netmask =~ /^\d{1,2}$/  # netmask in cidr format 
          @prefix = Prefix32.new(netmask.to_i)
        elsif IPAddress.valid_ipv4_netmask?(netmask)  # netmask in IP format
          @prefix = Prefix32.parse_netmask(netmask)
        else  # invalid netmask
          raise ArgumentError, "Invalid netmask #{netmask}"
        end
      else  # netmask is nil, reverting to defaul classful mask
        @prefix = Prefix32.new(32)
      end

      # Array formed with the IP octets
      @octets = @address.split(".").map{|i| i.to_i}
      # 32 bits interger containing the address
      @u32 = (@octets[0]<< 24) + (@octets[1]<< 16) + (@octets[2]<< 8) + (@octets[3])
      
    end # def initialize

    #
    # Returns the address portion of the IPv4 object
    # as a string.
    #
    #   ip = IPAddress("172.16.100.4/22")
    #
    #   ip.address
    #     #=> "172.16.100.4"
    #
    def address
      @address
    end

    #
    # Returns the prefix portion of the IPv4 object
    # as a IPAddress::Prefix32 object
    #
    #   ip = IPAddress("172.16.100.4/22")
    #
    #   ip.prefix
    #     #=> 22
    #
    #   ip.prefix.class
    #     #=> IPAddress::Prefix32
    #
    def prefix
      @prefix
    end

    #
    # Set a new prefix number for the object
    #
    # This is useful if you want to change the prefix
    # to an object created with IPv4::parse_u32 or
    # if the object was created using the classful
    # mask.
    #
    #   ip = IPAddress("172.16.100.4")
    #
    #   puts ip
    #     #=> 172.16.100.4/16
    #   
    #   ip.prefix = 22
    #
    #   puts ip
    #     #=> 172.16.100.4/22
    #
    def prefix=(num)
      @prefix = Prefix32.new(num)
    end

    # 
    # Returns the address as an array of decimal values
    #
    #   ip = IPAddress("172.16.100.4")
    #
    #   ip.octets
    #     #=> [172, 16, 100, 4]
    #
    def octets
      @octets
    end
    
    #
    # Returns a string with the address portion of 
    # the IPv4 object
    #
    #   ip = IPAddress("172.16.100.4/22")
    #
    #   ip.to_s
    #     #=> "172.16.100.4"
    #
    def to_s
      @address
    end

    #
    # Returns a string with the IP address in canonical
    # form.
    #
    #   ip = IPAddress("172.16.100.4/22")
    #
    #   ip.to_string
    #     #=> "172.16.100.4/22"
    #
    def to_string
      "#@address/#@prefix"
    end

    # 
    # Returns the prefix as a string in IP format
    #
    #   ip = IPAddress("172.16.100.4/22")
    #
    #   ip.netmask
    #     #=> "255.255.252.0"
    #
    def netmask
      @prefix.to_ip
    end

    #
    # Like IPv4#prefix=, this method allow you to 
    # change the prefix / netmask of an IP address
    # object.
    #
    #   ip = IPAddress("172.16.100.4")
    #
    #   puts ip
    #     #=> 172.16.100.4/16
    #
    #   ip.netmask = "255.255.252.0"
    #
    #   puts ip
    #     #=> 172.16.100.4/22
    #
    def netmask=(addr)
      @prefix = Prefix32.parse_netmask(addr)
    end

    #
    # Returns the address portion in unsigned
    # 32 bits integer format.
    #
    # This method is identical to the C function
    # inet_pton to create a 32 bits address family 
    # structure. 
    #
    #   ip = IPAddress("10.0.0.0/8")
    #
    #   ip.to_i
    #     #=> 167772160
    #
    def u32
      @u32
    end
    alias_method :to_i, :u32
    alias_method :to_u32, :u32
    
    #
    # Returns the address portion of an IPv4 object
    # in a network byte order format.
    #
    #   ip = IPAddress("172.16.10.1/24")
    #
    #   ip.data
    #     #=> "\254\020\n\001"
    #
    # It is usually used to include an IP address
    # in a data packet to be sent over a socket
    #
    #   a = Socket.open(params) # socket details here
    #   ip = IPAddress("10.1.1.0/24")
    #   binary_data = ["Address: "].pack("a*") + ip.data 
    #   
    #   # Send binary data
    #   a.puts binary_data
    #
    def data
      [@u32].pack("N")
    end

    #
    # Returns the octet specified by index
    #
    #   ip = IPAddress("172.16.100.50/24")
    #
    #   ip[0]
    #     #=> 172
    #   ip[1]
    #     #=> 16
    #   ip[2]
    #     #=> 100
    #   ip[3]
    #     #=> 50
    #
    def [](index)
      @octets[index]
    end
    alias_method :octet, :[]
    
    #
    # Returns the address portion of an IP in binary format,
    # as a string containing a sequence of 0 and 1
    #
    #   ip = IPAddress("127.0.0.1")
    #
    #   ip.bits
    #     #=> "01111111000000000000000000000001"
    #
    def bits
      data.unpack("B*").first
    end

    #
    # Returns the broadcast address for the given IP.
    #
    #   ip = IPAddress("172.16.10.64/24")
    #
    #   ip.broadcast.to_s
    #     #=> "172.16.10.255"
    #
    def broadcast
      self.class.parse_u32(broadcast_u32, @prefix)
    end
    
    #
    # Checks if the IP address is actually a network
    #
    #   ip = IPAddress("172.16.10.64/24")
    #
    #   ip.network?
    #     #=> false
    # 
    #   ip = IPAddress("172.16.10.64/26")
    #
    #   ip.network?
    #     #=> true
    #
    def network?
      @u32 | @prefix.to_u32 == @prefix.to_u32
    end

    #
    # Returns a new IPv4 object with the network number 
    # for the given IP.
    #
    #   ip = IPAddress("172.16.10.64/24")
    #
    #   ip.network.to_s
    #     #=> "172.16.10.0"
    #
    def network
      self.class.parse_u32(network_u32, @prefix)
    end

    #
    # Returns a new IPv4 object with the
    # first host IP address in the range.
    # 
    # Example: given the 192.168.100.0/24 network, the first
    # host IP address is 192.168.100.1.
    #
    #   ip = IPAddress("192.168.100.0/24")
    #
    #   ip.first.to_s
    #     #=> "192.168.100.1"
    #
    # The object IP doesn't need to be a network: the method
    # automatically gets the network number from it
    #
    #   ip = IPAddress("192.168.100.50/24")
    #
    #   ip.first.to_s
    #     #=> "192.168.100.1"
    #
    def first
      self.class.parse_u32(network_u32+1, @prefix)
    end

    #
    # Like its sibling method IPv4#first, this method 
    # returns a new IPv4 object with the 
    # last host IP address in the range.
    # 
    # Example: given the 192.168.100.0/24 network, the last
    # host IP address is 192.168.100.254
    #
    #   ip = IPAddress("192.168.100.0/24")
    #
    #   ip.last.to_s
    #     #=> "192.168.100.254"
    #
    # The object IP doesn't need to be a network: the method
    # automatically gets the network number from it
    #
    #   ip = IPAddress("192.168.100.50/24")
    #
    #   ip.last.to_s
    #     #=> "192.168.100.254"
    #
    def last
      self.class.parse_u32(broadcast_u32-1, @prefix)
    end

    #
    # Iterates over all the hosts IP addresses for the given
    # network (or IP address).
    #
    #   ip = IPAddress("10.0.0.1/29")
    #
    #   ip.each_host do |i|
    #     p i.to_s
    #   end
    #     #=> "10.0.0.1"
    #     #=> "10.0.0.2"
    #     #=> "10.0.0.3"
    #     #=> "10.0.0.4"
    #     #=> "10.0.0.5"
    #     #=> "10.0.0.6"
    #
    def each_host
      (network_u32+1..broadcast_u32-1).each do |i|
        yield self.class.parse_u32(i, @prefix)
      end
    end

    #
    # Iterates over all the IP addresses for the given
    # network (or IP address).
    #
    # The object yielded is a new IPv4 object created
    # from the iteration.
    #
    #   ip = IPAddress("10.0.0.1/29")
    #
    #   ip.each do |i|
    #     p i.address
    #   end
    #     #=> "10.0.0.0"
    #     #=> "10.0.0.1"
    #     #=> "10.0.0.2"
    #     #=> "10.0.0.3"
    #     #=> "10.0.0.4"
    #     #=> "10.0.0.5"
    #     #=> "10.0.0.6"
    #     #=> "10.0.0.7"
    #
    def each
      (network_u32..broadcast_u32).each do |i|
        yield self.class.parse_u32(i, @prefix)
      end
    end

    #
    # Spaceship operator to compare IP addresses
    #
    # An IP address is considered to be minor if it 
    # has a greater prefix (thus smaller hosts
    # portion) and a smaller u32 value.
    #
    # For example, "10.100.100.1/8" is smaller than
    # "172.16.0.1/16", but it's bigger than "10.100.100.1/16".
    #
    # Example:
    #
    #   ip1 = IPAddress "10.100.100.1/8"
    #   ip2 = IPAddress "172.16.0.1/16"
    #   ip3 = IPAddress "10.100.100.1/16"
    #
    #   ip1 < ip2
    #     #=> true
    #   ip1 < ip3
    #     #=> false
    #
    def <=>(oth)
      if to_u32 > oth.to_u32
        return 1
      elsif to_u32 < oth.to_u32
        return -1
      else
        if prefix < oth.prefix
          return 1
        elsif prefix > oth.prefix
          return -1
        end
      end
      return 0
    end
    
    #
    # Returns the number of IP addresses included
    # in the network. It also counts the network
    # address and the broadcast address.
    #
    #   ip = IPAddress("10.0.0.1/29")
    #
    #   ip.size
    #     #=> 8
    #
    def size
      2 ** @prefix.host_prefix
    end

    #
    # Returns an array with the IP addresses of
    # all the hosts in the network.
    # 
    #   ip = IPAddress("10.0.0.1/29")
    #
    #   ip.hosts.map {|i| i.address}
    #     #=> ["10.0.0.1",
    #     #=>  "10.0.0.2",
    #     #=>  "10.0.0.3",
    #     #=>  "10.0.0.4",
    #     #=>  "10.0.0.5",
    #     #=>  "10.0.0.6"]
    #
    def hosts
      to_a[1..-2]
    end
    
    #
    # Returns the network number in Unsigned 32bits format
    #
    #   ip = IPAddress("10.0.0.1/29")
    #
    #   ip.network_u32
    #     #=> 167772160
    #
    def network_u32
      @u32 & @prefix.to_u32
    end

    #
    # Returns the broadcast address in Unsigned 32bits format
    #
    #   ip = IPaddress("10.0.0.1/29")
    #
    #   ip.broadcast_u32
    #     #=> 167772167
    #
    def broadcast_u32
      network_u32 + size - 1
    end

    #
    # Checks whether a subnet includes the given IP address.
    #
    # Accepts an IPAddress::IPv4 object.
    #
    #   ip = IPAddress("192.168.10.100/24")
    #
    #   addr = IPAddress("192.168.10.102/24")
    #
    #   ip.include? addr
    #     #=> true
    #
    #   ip.include? IPAddress("172.16.0.48/16")
    #     #=> false
    #
    def include?(oth)
      @prefix <= oth.prefix and network_u32 == (oth.to_u32 & @prefix.to_u32)
    end

    #
    # Checks whether a subnet includes all the 
    # given IPv4 objects.
    #
    #   ip = IPAddress("192.168.10.100/24")
    #
    #   addr1 = IPAddress("192.168.10.102/24")
    #   addr2 = IPAddress("192.168.10.103/24")
    #
    #   ip.include_all?(addr1,addr2)
    #     #=> true
    #
    def include_all?(*others)
      others.all? {|oth| include?(oth)}
    end
    
    #
    # True if the object is an IPv4 address
    #
    #   ip = IPAddress("192.168.10.100/24")
    #
    #   ip.ipv4?
    #     #-> true
    #
#    def ipv4?
#      true
#    end

    #
    # True if the object is an IPv6 address
    #
    #   ip = IPAddress("192.168.10.100/24")
    #
    #   ip.ipv6?
    #     #-> false
    #
#    def ipv6?
#      false
#    end

    #
    # Checks if an IPv4 address objects belongs
    # to a private network RFC1918
    #
    # Example:
    #
    #   ip = IPAddress "10.1.1.1/24"
    #   ip.private?
    #     #=> true
    #
    def private?
      [self.class.new("10.0.0.0/8"),
       self.class.new("172.16.0.0/12"),
       self.class.new("192.168.0.0/16")].any? {|i| i.include? self}
    end

    #
    # Returns the IP address in in-addr.arpa format
    # for DNS lookups
    #
    #   ip = IPAddress("172.16.100.50/24")
    #
    #   ip.reverse
    #     #=> "50.100.16.172.in-addr.arpa"
    #
    def reverse
      @octets.reverse.join(".") + ".in-addr.arpa"
    end
    alias_method :arpa, :reverse
    
    #
    # Subnetting a network
    #
    # If the IP Address is a network, it can be divided into
    # multiple networks. If +self+ is not a network, the
    # method will calculate the network from the IP and then
    # subnet it.
    #
    # If +subnets+ is an power of two number, the resulting 
    # networks will be divided evenly from the supernet.
    #
    #   network = IPAddress("172.16.10.0/24")
    #
    #   network / 4   # implies map{|i| i.to_string}
    #     #=> ["172.16.10.0/26",
    #          "172.16.10.64/26",
    #          "172.16.10.128/26",
    #          "172.16.10.192/26"]
    #
    # If +num+ is any other number, the supernet will be 
    # divided into some networks with a even number of hosts and
    # other networks with the remaining addresses.
    #
    #   network = IPAddress("172.16.10.0/24")
    #
    #   network / 3   # implies map{|i| i.to_string}
    #     #=> ["172.16.10.0/26",
    #          "172.16.10.64/26",
    #          "172.16.10.128/25"]
    #
    # Returns an array of IPAddress objects
    #
    def subnet(subnets=2)
      unless (1..(2**@prefix.host_prefix)).include? subnets
        raise ArgumentError, "Value #{subnets} out of range" 
      end
      calculate_subnets(subnets)
    end
    alias_method :/, :subnet

    #
    # Returns a new IPv4 object from the supernetting
    # of the instance network.
    #
    # Supernetting is similar to subnetting, except
    # that you getting as a result a network with a
    # smaller prefix (bigger host space). For example,
    # given the network
    #
    #   ip = IPAddress("172.16.10.0/24")
    #
    # you can supernet it with a new /23 prefix
    #
    #   ip.supernet(23).to_string
    #     #=> "172.16.10.0/23"
    #
    # However if you supernet it with a /22 prefix, the
    # network address will change:
    #
    #   ip.supernet(22).to_string
    #     #=> "172.16.8.0/22"
    # 
    def supernet(new_prefix)
      raise ArgumentError, "Can't supernet a /1 network" if new_prefix < 1
      raise ArgumentError, "New prefix must be smaller than existing prefix" if new_prefix >= @prefix.to_i
      self.class.new(@address+"/#{new_prefix}").network
    end

    #
    # Returns the difference between two IP addresses
    # in unsigned int 32 bits format
    #  
    # Example:
    #
    #   ip1 = IPAddress("172.16.10.0/24")
    #   ip2 = IPAddress("172.16.11.0/24")
    #
    #   puts ip1 - ip2
    #     #=> 256
    #
    def -(oth)
      return (to_u32 - oth.to_u32).abs
    end

    #
    # Returns a new IPv4 object which is the result 
    # of the summarization, if possible, of the two 
    # objects
    #
    # Example:
    #
    #   ip1 = IPAddress("172.16.10.1/24")
    #   ip2 = IPAddress("172.16.11.2/24")
    #
    #   p (ip1 + ip2).map {|i| i.to_string}
    #     #=> ["172.16.10.0/23"]
    #
    # If the networks are not contiguous, returns
    # the two network numbers from the objects
    #
    #   ip1 = IPAddress("10.0.0.1/24")
    #   ip2 = IPAddress("10.0.2.1/24")
    #
    #   p (ip1 + ip2).map {|i| i.to_string}
    #     #=> ["10.0.0.0/24","10.0.2.0/24"]
    #
    def +(oth)
      aggregate(*[self,oth].sort.map{|i| i.network})
    end

    #
    # Checks whether the ip address belongs to a 
    # RFC 791 CLASS A network, no matter
    # what the subnet mask is.
    #
    # Example:
    # 
    #   ip = IPAddress("10.0.0.1/24")
    #
    #   ip.a?
    #     #=> true
    #
    def a?
      CLASSFUL.key(8) === bits
    end
    
    #
    # Checks whether the ip address belongs to a
    # RFC 791 CLASS B network, no matter
    # what the subnet mask is.
    #
    # Example:
    #
    #   ip = IPAddress("172.16.10.1/24")
    #
    #   ip.b?
    #     #=> true
    #
    def b?
      CLASSFUL.key(16) === bits
    end

    #
    # Checks whether the ip address belongs to a
    # RFC 791 CLASS C network, no matter
    # what the subnet mask is.
    #
    # Example:
    #
    #   ip = IPAddress("192.168.1.1/30")
    #
    #   ip.c?
    #     #=> true
    #
    def c?
      CLASSFUL.key(24) === bits
    end

    #
    # Return the ip address in a format compatible
    # with the IPv6 Mapped IPv4 addresses
    # 
    # Example:
    #
    #   ip = IPAddress("172.16.10.1/24")
    #
    #   ip.to_ipv6
    #     #=> "ac10:0a01"
    #
    def to_ipv6
      "%.4x:%.4x" % [to_u32].pack("N").unpack("nn")
    end

    #
    # Creates a new IPv4 object from an
    # unsigned 32bits integer.
    #
    #   ip = IPAddress::IPv4::parse_u32(167772160)
    #
    #   ip.prefix = 8
    #   ip.to_string
    #     #=> "10.0.0.0/8"
    #
    # The +prefix+ parameter is optional:
    #
    #   ip = IPAddress::IPv4::parse_u32(167772160, 8)
    #
    #   ip.to_string
    #     #=> "10.0.0.0/8"
    #
    def self.parse_u32(u32, prefix=32)
      self.new([u32].pack("N").unpack("C4").join(".")+"/#{prefix}")
    end

    #
    # Creates a new IPv4 object from binary data,
    # like the one you get from a network stream.
    # 
    # For example, on a network stream the IP 172.16.0.1
    # is represented with the binary "\254\020\n\001".
    # 
    #   ip = IPAddress::IPv4::parse_data "\254\020\n\001"
    #   ip.prefix = 24
    #
    #   ip.to_string
    #     #=> "172.16.10.1/24"
    #
    def self.parse_data(str, prefix=32)
      self.new(str.unpack("C4").join(".")+"/#{prefix}")
    end

    #
    # Extract an IPv4 address from a string and 
    # returns a new object
    #
    # Example:
    #
    #   str = "foobar172.16.10.1barbaz"
    #   ip = IPAddress::IPv4::extract str
    #
    #   ip.to_s
    #     #=> "172.16.10.1"
    #
    def self.extract(str)
      self.new REGEXP.match(str).to_s
    end
    
    #
    # Summarization (or aggregation) is the process when two or more
    # networks are taken together to check if a supernet, including all
    # and only these networks, exists. If it exists then this supernet
    # is called the summarized (or aggregated) network.
    #
    # It is very important to understand that summarization can only
    # occur if there are no holes in the aggregated network, or, in other
    # words, if the given networks fill completely the address space
    # of the supernet. So the two rules are:
    #
    # 1) The aggregate network must contain +all+ the IP addresses of the
    #    original networks;
    # 2) The aggregate network must contain +only+ the IP addresses of the
    #    original networks;
    #
    # A few examples will help clarify the above. Let's consider for
    # instance the following two networks:
    #
    #   ip1 = IPAddress("172.16.10.0/24")
    #   ip2 = IPAddress("172.16.11.0/24")
    #
    # These two networks can be expressed using only one IP address
    # network if we change the prefix. Let Ruby do the work:
    #
    #   IPAddress::IPv4::summarize(ip1,ip2).to_s
    #     #=> "172.16.10.0/23"
    #
    # We note how the network "172.16.10.0/23" includes all the addresses
    # specified in the above networks, and (more important) includes
    # ONLY those addresses. 
    #
    # If we summarized +ip1+ and +ip2+ with the following network:
    #
    #   "172.16.0.0/16"
    #
    # we would have satisfied rule #1 above, but not rule #2. So "172.16.0.0/16"
    # is not an aggregate network for +ip1+ and +ip2+.
    #
    # If it's not possible to compute a single aggregated network for all the
    # original networks, the method returns an array with all the aggregate
    # networks found. For example, the following four networks can be
    # aggregated in a single /22:
    #
    #   ip1 = IPAddress("10.0.0.1/24")
    #   ip2 = IPAddress("10.0.1.1/24")
    #   ip3 = IPAddress("10.0.2.1/24")
    #   ip4 = IPAddress("10.0.3.1/24")
    #
    #   IPAddress::IPv4::summarize(ip1,ip2,ip3,ip4).to_string
    #     #=> "10.0.0.0/22", 
    #
    # But the following networks can't be summarized in a single network:
    #
    #   ip1 = IPAddress("10.0.1.1/24")
    #   ip2 = IPAddress("10.0.2.1/24")
    #   ip3 = IPAddress("10.0.3.1/24")
    #   ip4 = IPAddress("10.0.4.1/24")
    #
    #   IPAddress::IPv4::summarize(ip1,ip2,ip3,ip4).map{|i| i.to_string}
    #     #=> ["10.0.1.0/24","10.0.2.0/23","10.0.4.0/24"]
    #
    def self.summarize(*args)
      # one network? no need to summarize
      return [args.first.network] if args.size == 1
      
      i = 0
      result = args.dup.sort.map{|ip| ip.network}
      while i < result.size-1
        sum = result[i] + result[i+1]
        result[i..i+1] = sum.first if sum.size == 1
        i += 1
      end
      
      result.flatten!
      if result.size == args.size
        # nothing more to summarize
        return result
      else
        # keep on summarizing
        return self.summarize(*result)
      end
    end

    #
    # Creates a new IPv4 address object by parsing the 
    # address in a classful way.
    #
    # Classful addresses have a fixed netmask based on the 
    # class they belong to:
    #
    # * Class A, from 0.0.0.0 to 127.255.255.255
    # * Class B, from 128.0.0.0 to 191.255.255.255
    # * Class C, D and E, from 192.0.0.0 to 255.255.255.254
    #
    # Note that classes C, D and E will all have a default
    # prefix of /24 or 255.255.255.0
    #
    # Example:
    #
    #   ip = IPAddress::IPv4.parse_classful "10.0.0.1"
    #
    #   ip.netmask 
    #     #=> "255.0.0.0"
    #   ip.a?
    #     #=> true
    #
    def self.parse_classful(ip)
      if IPAddress.valid_ipv4?(ip)
        address = ip.strip
      else
        raise ArgumentError, "Invalid IP #{ip.inspect}"
      end
      prefix = CLASSFUL.find{|h,k| h === ("%.8b" % address.to_i)}.last
      self.new "#{address}/#{prefix}"
    end

    #
    # private methods
    #
    private
    
    def calculate_subnets(subnets)
      po2 = closest_power_of_2(subnets)
      new_prefix = @prefix + log2(po2).to_i
      networks = Array.new
      (0..po2-1).each do |i|
        mul = i * (2**(32-new_prefix))
        networks << IPAddress::IPv4.parse_u32(network_u32+mul, new_prefix)
      end
      until networks.size == subnets
        networks = sum_first_found(networks)
      end
      return networks
    end
    
    def sum_first_found(arr)
      dup = arr.dup.reverse
      dup.each_with_index do |obj,i|
        a = [IPAddress::IPv4.summarize(obj,dup[i+1])].flatten
        if a.size == 1
          dup[i..i+1] = a
          return dup.reverse
        end
      end
      return dup.reverse
    end

    def aggregate(ip1,ip2)
      return [ip1] if ip1.include? ip2

      snet = ip1.supernet(ip1.prefix-1)
      if snet.include_all?(ip1, ip2) && ((ip1.size + ip2.size) == snet.size)
        return [snet]
      else
        return [ip1, ip2]
      end
    end

    def log2(n); Math::log(n) / Math::log(2); end
    
    def power_of_2?(int)
      log2(int).to_i == log2(int)
    end

    def closest_power_of_2(int, limit=32)
      int.upto(limit) do |i|
        return i if power_of_2?(i)
      end
    end
    
  end # class IPv4
end # module IPAddress

module IPAddress; 
  # 
  # =Name
  # 
  # IPAddress::IPv6 - IP version 6 address manipulation library
  #
  # =Synopsis
  #
  #    require 'ipaddress'
  #
  # =Description
  # 
  # Class IPAddress::IPv6 is used to handle IPv6 type addresses. 
  #
  # == IPv6 addresses
  #
  # IPv6 addresses are 128 bits long, in contrast with IPv4 addresses
  # which are only 32 bits long. An IPv6 address is generally written as
  # eight groups of four hexadecimal digits, each group representing 16
  # bits or two octect. For example, the following is a valid IPv6
  # address:
  #
  #   1080:0000:0000:0000:0008:0800:200c:417a
  #
  # Letters in an IPv6 address are usually written downcase, as per
  # RFC. You can create a new IPv6 object using uppercase letters, but
  # they will be converted.
  #
  # === Compression
  #
  # Since IPv6 addresses are very long to write, there are some
  # semplifications and compressions that you can use to shorten them.
  #
  # * Leading zeroes: all the leading zeroes within a group can be
  #   omitted: "0008" would become "8"
  #
  # * A string of consecutive zeroes can be replaced by the string
  #   "::". This can be only applied once.
  #
  # Using compression, the IPv6 address written above can be shorten into
  # the following, equivalent, address
  #
  #   1080::8:800:200c:417a
  #
  # This short version is often used in human representation.
  #
  # === Network Mask
  #
  # As we used to do with IPv4 addresses, an IPv6 address can be written
  # using the prefix notation to specify the subnet mask:
  #
  #   1080::8:800:200c:417a/64
  #
  # The /64 part means that the first 64 bits of the address are
  # representing the network portion, and the last 64 bits are the host
  # portion.
  #
  #
  class IPv6 
    
    include IPAddress
    include Enumerable  
    include Comparable                  

    
    #
    # Format string to pretty print IPv6 addresses
    #
    IN6FORMAT = ("%.4x:"*8).chop
    
    #
    # Creates a new IPv6 address object.
    #
    # An IPv6 address can be expressed in any of the following forms:
    # 
    # * "1080:0000:0000:0000:0008:0800:200C:417A": IPv6 address with no compression
    # * "1080:0:0:0:8:800:200C:417A": IPv6 address with leading zeros compression 
    # * "1080::8:800:200C:417A": IPv6 address with full compression
    #
    # In all these 3 cases, a new IPv6 address object will be created, using the default
    # subnet mask /128
    #
    # You can also specify the subnet mask as with IPv4 addresses:
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    def initialize(str)
      ip, netmask = str.split("/")

      if str =~ /:.+\./
        raise ArgumentError, "Please use #{self.class}::Mapped for IPv4 mapped addresses"
      end
      
      if IPAddress.valid_ipv6?(ip)
        @groups = self.class.groups(ip)
        @address = IN6FORMAT % @groups
        @compressed = compress_address
      else
        raise ArgumentError, "Invalid IP #{ip.inspect}"
      end

      @prefix = Prefix128.new(netmask ? netmask : 128)

    end # def initialize

    #
    # Returns the IPv6 address in uncompressed form:
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    #   ip6.address
    #     #=> "2001:0db8:0000:0000:0008:0800:200c:417a"
    #
    def address
      @address
    end

    #
    # Returns an array with the 16 bits groups in decimal 
    # format:
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    #   ip6.groups
    #     #=> [8193, 3512, 0, 0, 8, 2048, 8204, 16762]
    #
    def groups
      @groups
    end

    # 
    # Returns an instance of the prefix object
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    #   ip6.prefix
    #     #=> 64
    #
    def prefix
      @prefix
    end

    #
    # Set a new prefix number for the object
    #
    # This is useful if you want to change the prefix
    # to an object created with IPv6::parse_u128 or
    # if the object was created using the default prefix
    # of 128 bits.
    #
    #   ip6 = IPAddress("2001:db8::8:800:200c:417a")
    #
    #   puts ip6.to_string
    #     #=> "2001:db8::8:800:200c:417a/128"
    #
    #   ip6.prefix = 64
    #   puts ip6.to_string
    #     #=> "2001:db8::8:800:200c:417a/64"
    #
    def prefix=(num)
      @prefix = Prefix128.new(num)
    end

    # 
    # Unlike its counterpart IPv6#to_string method, IPv6#to_string_uncompressed 
    # returns the whole IPv6 address and prefix in an uncompressed form
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    #   ip6.to_string_uncompressed
    #     #=> "2001:0db8:0000:0000:0008:0800:200c:417a/64"
    #
    def to_string_uncompressed
      "#@address/#@prefix"
    end

    #
    # Returns the IPv6 address in a human readable form,
    # using the compressed address.
    #
    #   ip6 = IPAddress "2001:0db8:0000:0000:0008:0800:200c:417a/64"
    #
    #   ip6.to_string
    #     #=> "2001:db8::8:800:200c:417a/64"
    #
    def to_string
      "#@compressed/#@prefix"
    end

    #
    # Returns the IPv6 address in a human readable form,
    # using the compressed address.
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    #   ip6.to_s
    #     #=> "2001:db8::8:800:200c:417a"
    #
    def to_s
      @compressed
    end

    #
    # Returns a decimal format (unsigned 128 bit) of the
    # IPv6 address
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    #   ip6.to_i
    #     #=> 42540766411282592856906245548098208122
    #
    def to_i
      to_hex.hex
    end
    alias_method :to_u128, :to_i

    #
    # True if the IPv6 address is a network
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    #   ip6.network?
    #     #=> false
    #
    #   ip6 = IPAddress "2001:db8:8:800::/64"
    #
    #   ip6.network?
    #     #=> true
    #
    def network?
      to_u128 | @prefix.to_u128 == @prefix.to_u128
    end

    #
    # Returns the 16-bits value specified by index
    #
    #   ip = IPAddress("2001:db8::8:800:200c:417a/64")
    #
    #   ip[0]
    #     #=> 8193
    #   ip[1]
    #     #=> 3512
    #   ip[2]
    #     #=> 0
    #   ip[3]
    #     #=> 0
    #
    def [](index)
      @groups[index]
    end
    alias_method :group, :[]

    # 
    # Returns a Base16 number representing the IPv6 
    # address
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    #   ip6.to_hex
    #     #=> "20010db80000000000080800200c417a"
    #
    def to_hex
      hexs.join("")
    end

    # Returns the address portion of an IPv6 object
    # in a network byte order format.
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    #   ip6.data
    #     #=> " \001\r\270\000\000\000\000\000\b\b\000 \fAz"
    #
    # It is usually used to include an IP address
    # in a data packet to be sent over a socket
    #
    #   a = Socket.open(params) # socket details here
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #   binary_data = ["Address: "].pack("a*") + ip.data
    #
    #   # Send binary data
    #   a.puts binary_data
    #
    def data
      @groups.pack("n8")
    end

    #
    # Returns an array of the 16 bits groups in hexdecimal 
    # format:
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    #   ip6.hexs
    #     #=> ["2001", "0db8", "0000", "0000", "0008", "0800", "200c", "417a"]
    #
    # Not to be confused with the similar IPv6#to_hex method.
    #
    def hexs
      @address.split(":")
    end

    #
    # Returns the IPv6 address in a DNS reverse lookup
    # string, as per RFC3172 and RFC2874.
    #   
    #   ip6 = IPAddress "3ffe:505:2::f"
    #   
    #   ip6.reverse
    #     #=> "f.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.2.0.0.0.5.0.5.0.e.f.f.3.ip6.arpa"
    #
    def reverse
      to_hex.reverse.gsub(/./){|c| c+"."} + "ip6.arpa"
    end
    alias_method :arpa, :reverse

    #
    # Returns the network number in Unsigned 128bits format
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    #   ip6.network_u128
    #     #=> 42540766411282592856903984951653826560
    #
    def network_u128
      to_u128 & @prefix.to_u128
    end

    #
    # Checks whether a subnet includes the given IP address.
    #
    # Example:
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #   addr = IPAddress "2001:db8::8:800:200c:1/128"
    #
    #   ip6.include? addr
    #     #=> true
    #
    #   ip6.include? IPAddress("2001:db8:1::8:800:200c:417a/76")
    #     #=> false
    #
    def include?(oth)
      @prefix <= oth.prefix and network_u128 == self.class.new(oth.address+"/#@prefix").network_u128
    end

    #
    # Compressed form of the IPv6 address
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    #   ip6.compressed
    #     #=> "2001:db8::8:800:200c:417a"
    #
    def compressed
      @compressed
    end

    # 
    # Returns true if the address is an unspecified address
    # 
    # See IPAddress::IPv6::Unspecified for more information
    #
    def unspecified?
      @prefix == 128 and @compressed == "::"
    end

    # 
    # Returns true if the address is a loopback address
    # 
    # See IPAddress::IPv6::Loopback for more information
    #
    def loopback?
      @prefix == 128 and @compressed == "::1"
    end

    # 
    # Returns true if the address is a mapped address
    # 
    # See IPAddress::IPv6::Mapped for more information
    #
    def mapped?
      to_u128 >> 32 == 0xffff
    end
    
    #
    # Returns the address portion of an IP in binary format,
    # as a string containing a sequence of 0 and 1
    #
    #   ip6 = IPAddress("2001:db8::8:800:200c:417a")
    #
    #   ip6.bits 
    #     #=> "0010000000000001000011011011100000 [...] "
    #
    def bits
      data.unpack("B*").first
    end
    
    #
    # Expands an IPv6 address in the canocical form
    #
    #   IPAddress::IPv6.expand "2001:0DB8:0:CD30::"
    #     #=> "2001:0DB8:0000:CD30:0000:0000:0000:0000"
    #
    def self.expand(str)
      self.new(str).address
    end

    #
    # Compress an IPv6 address in its compressed form
    #
    #   IPAddress::IPv6.compress "2001:0DB8:0000:CD30:0000:0000:0000:0000"
    #     #=> "2001:db8:0:cd30::"
    #
    def self.compress(str)
      self.new(str).compressed
    end

    # 
    # Literal version of the IPv6 address
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #
    #   ip6.literal
    #     #=> "2001-0db8-0000-0000-0008-0800-200c-417a.ipv6-literal.net"
    # 
    def literal
      @address.gsub(":","-") + ".ipv6-literal.net"
    end

    #
    # Extract 16 bits groups from a string
    #
    def self.groups(str)
      l, r = if str =~ /^(.*)::(.*)$/
               [$1,$2].map {|i| i.split ":"}
             else
               [str.split(":"),[]]
             end
      (l + Array.new(8-l.size-r.size, '0') + r).map {|i| i.hex}
    end

    #
    # Creates a new IPv6 object from binary data,
    # like the one you get from a network stream.
    # 
    # For example, on a network stream the IP 
    # 
    #  "2001:db8::8:800:200c:417a"
    #
    # is represented with the binary data
    #
    #   " \001\r\270\000\000\000\000\000\b\b\000 \fAz"
    #
    # With that data you can create a new IPv6 object:
    #
    #   ip6 = IPAddress::IPv6::parse_data " \001\r\270\000\000\000\000\000\b\b\000 \fAz"
    #   ip6.prefix = 64
    #
    #   ip6.to_s
    #     #=> "2001:db8::8:800:200c:417a/64"
    #
    def self.parse_data(str)
      self.new(IN6FORMAT % str.unpack("n8"))
    end

    #
    # Creates a new IPv6 object from an
    # unsigned 128 bits integer.
    #
    #   ip6 = IPAddress::IPv6::parse_u128(21932261930451111902915077091070067066)
    #   ip6.prefix = 64
    #
    #   ip6.to_s
    #     #=> "1080::8:800:200c:417a/64"
    #
    # The +prefix+ parameter is optional:
    #
    #   ip6 = IPAddress::IPv6::parse_u128(21932261930451111902915077091070067066, 64)
    #
    #   ip6.to_s
    #     #=> "1080::8:800:200c:417a/64"
    #
    def self.parse_u128(u128, prefix=128)
      str = IN6FORMAT % (0..7).map{|i| (u128>>(112-16*i))&0xffff}
      self.new(str + "/#{prefix}")
    end

    #
    # Creates a new IPv6 object from a number expressed in
    # hexdecimal format:
    #
    #   ip6 = IPAddress::IPv6::parse_hex("20010db80000000000080800200c417a")
    #   ip6.prefix = 64
    #
    #   ip6.to_s
    #     #=> "2001:db8::8:800:200c:417a/64"
    #
    # The +prefix+ parameter is optional:
    #
    #   ip6 = IPAddress::IPv6::parse_hex("20010db80000000000080800200c417a", 64)
    #
    #   ip6.to_s
    #     #=> "1080::8:800:200c:417a/64"
    #
    def self.parse_hex(hex, prefix=128)
      self.parse_u128(hex.hex, prefix)
    end
    
    private

    def compress_address
      str = @groups.map{|i| i.to_s 16}.join ":"
      loop do
        break if str.sub!(/\A0:0:0:0:0:0:0:0\Z/, '::')
        break if str.sub!(/\b0:0:0:0:0:0:0\b/, ':')
        break if str.sub!(/\b0:0:0:0:0:0\b/, ':')
        break if str.sub!(/\b0:0:0:0:0\b/, ':')
        break if str.sub!(/\b0:0:0:0\b/, ':')
        break if str.sub!(/\b0:0:0\b/, ':')
        break if str.sub!(/\b0:0\b/, ':')
        break
      end
      str.sub(/:{3,}/, '::')
    end
    
  end # class IPv6

  #
  # The address with all zero bits is called the +unspecified+ address
  # (corresponding to 0.0.0.0 in IPv4). It should be something like this:
  #
  #   0000:0000:0000:0000:0000:0000:0000:0000
  #
  # but, with the use of compression, it is usually written as just two
  # colons:
  #
  #   ::
  #
  # or, specifying the netmask:
  #
  #   ::/128
  #
  # With IPAddress, create a new unspecified IPv6 address using its own
  # subclass:
  #
  #   ip = IPAddress::IPv6::Unspecified.new
  #
  #   ip.to_s
  #     #=> => "::/128"
  #
  # You can easily check if an IPv6 object is an unspecified address by
  # using the IPv6#unspecified? method
  #
  #   ip.unspecified?
  #     #=> true
  #
  # An unspecified IPv6 address can also be created with the wrapper
  # method, like we've seen before
  #
  #   ip = IPAddress "::"
  #
  #   ip.unspecified?
  #     #=> true
  #
  # This address must never be assigned to an interface and is to be used
  # only in software before the application has learned its host's source
  # address appropriate for a pending connection. Routers must not forward
  # packets with the unspecified address.
  #
  class IPAddress::IPv6::Unspecified < IPAddress::IPv6
    #
    # Creates a new IPv6 unspecified address
    #
    #   ip = IPAddress::IPv6::Unspecified.new
    #
    #   ip.to_s
    #      #=> => "::/128"
    #
    def initialize
      @address = ("0000:"*8).chop
      @groups = Array.new(8,0)
      @prefix = Prefix128.new(128)
      @compressed = compress_address
    end 
  end # class IPv6::Unspecified

  #
  #   The loopback  address is a unicast localhost address. If an
  # application in a host sends packets to this address, the IPv6 stack
  # will loop these packets back on the same virtual interface.
  #
  # Loopback addresses are expressed in the following form:
  #
  #   ::1
  #
  # or, with their appropriate prefix,
  #
  #   ::1/128
  #
  # As for the unspecified addresses, IPv6 loopbacks can be created with
  # IPAddress calling their own class:
  #
  #   ip = IPAddress::IPv6::Loopback.new
  #
  #   ip.to_s
  #     #=> "::1/128"
  #
  # or by using the wrapper:
  #
  #   ip = IPAddress "::1"
  #
  #   ip.to_s
  #     #=> "::1/128"
  #
  # Checking if an address is loopback is easy with the IPv6#loopback?
  # method:
  #
  #   ip.loopback?
  #     #=> true
  #
  # The IPv6 loopback address corresponds to 127.0.0.1 in IPv4.
  #
  class IPAddress::IPv6::Loopback < IPAddress::IPv6
    #
    # Creates a new IPv6 unspecified address
    #
    #   ip = IPAddress::IPv6::Loopback.new
    #
    #   ip.to_s
    #     #=> "::1/128"
    #
    def initialize
      @address = ("0000:"*7)+"0001"
      @groups = Array.new(7,0).push(1) 
      @prefix = Prefix128.new(128)
      @compressed = compress_address
    end
  end # class IPv6::Loopback

  #
  # It is usually identified as a IPv4 mapped IPv6 address, a particular
  # IPv6 address which aids the transition from IPv4 to IPv6. The
  # structure of the address is
  #
  #   ::ffff:w.y.x.z
  #
  # where w.x.y.z is a normal IPv4 address. For example, the following is
  # a mapped IPv6 address:
  #
  #   ::ffff:192.168.100.1
  #
  # IPAddress is very powerful in handling mapped IPv6 addresses, as the
  # IPv4 portion is stored internally as a normal IPv4 object. Let's have
  # a look at some examples. To create a new mapped address, just use the
  # class builder itself
  #
  #   ip6 = IPAddress::IPv6::Mapped.new "::ffff:172.16.10.1/128"
  #
  # or just use the wrapper method
  #
  #   ip6 = IPAddress "::ffff:172.16.10.1/128"
  #
  # Let's check it's really a mapped address:
  #
  #   ip6.mapped?
  #     #=> true
  #
  #   ip6.to_s
  #     #=> "::FFFF:172.16.10.1/128"
  #
  # Now with the +ipv4+ attribute, we can easily access the IPv4 portion
  # of the mapped IPv6 address:
  #
  #   ip6.ipv4.address
  #     #=> "172.16.10.1"
  #
  # Internally, the IPv4 address is stored as two 16 bits
  # groups. Therefore all the usual methods for an IPv6 address are
  # working perfectly fine:
  #
  #   ip6.to_hex
  #     #=> "00000000000000000000ffffac100a01"
  #
  #   ip6.address
  #     #=> "0000:0000:0000:0000:0000:ffff:ac10:0a01"
  #
  # A mapped IPv6 can also be created just by specify the address in the
  # following format:
  #
  #   ip6 = IPAddress "::172.16.10.1"
  #
  # That is, two colons and the IPv4 address. However, as by RFC, the ffff
  # group will be automatically added at the beginning
  #
  #   ip6.to_s
  #     => "::ffff:172.16.10.1/128"
  #
  # making it a mapped IPv6 compatible address.
  #
  class IPAddress::IPv6::Mapped < IPAddress::IPv6

    # Access the internal IPv4 address
    attr_reader :ipv4

    #
    # Creates a new IPv6 IPv4-mapped address
    #
    #   ip6 = IPAddress::IPv6::Mapped.new "::ffff:172.16.10.1/128"
    #
    #   ipv6.ipv4.class
    #     #=> IPAddress::IPv4
    #
    # An IPv6 IPv4-mapped address can also be created using the 
    # IPv6 only format of the address:
    #
    #   ip6 = IPAddress::IPv6::Mapped.new "::0d01:4403"
    #
    #   ip6.to_s
    #     #=> "::ffff:13.1.68.3"
    #
    def initialize(str)
      string, netmask = str.split("/")
      if string =~ /\./ # IPv4 in dotted decimal form
        @ipv4 = IPAddress::IPv4.extract(string)
      else # IPv4 in hex form
        groups = IPAddress::IPv6.groups(string)
        @ipv4 = IPAddress::IPv4.parse_u32((groups[-2]<< 16)+groups[-1])
      end
      super("::ffff:#{@ipv4.to_ipv6}/#{netmask}")
    end

    # 
    # Similar to IPv6#to_s, but prints out the IPv4 address 
    # in dotted decimal format
    #
    #   ip6 = IPAddress "::ffff:172.16.10.1/128"
    #
    #   ip6.to_s
    #     #=> "::ffff:172.16.10.1"
    #
    def to_s
      "::ffff:#{@ipv4.address}"
    end

    # 
    # Similar to IPv6#to_string, but prints out the IPv4 address 
    # in dotted decimal format
    #
    #
    #   ip6 = IPAddress "::ffff:172.16.10.1/128"
    #
    #   ip6.to_string
    #     #=> "::ffff:172.16.10.1/128"
    #
    def to_string
      "::ffff:#{@ipv4.address}/#@prefix"
    end

    #
    # Checks if the IPv6 address is IPv4 mapped
    #
    #   ip6 = IPAddress "::ffff:172.16.10.1/128"
    #
    #   ip6.mapped?
    #     #=> true
    #
    def mapped?
      true
    end
  end # class IPv6::Mapped

end # module IPAddress


