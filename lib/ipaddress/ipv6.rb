require 'ipaddress/ipbase'
require 'ipaddress/prefix'

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
  #
  class IPv6 < IPBase
    
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
    #   ip = IPAddress("2001:db8::8:800:200c:417a")
    #   puts ip
    #     #=> 2001:db8::8:800:200c:417a/128
    #
    #   ip.prefix = 64
    #   puts ip
    #     #=> 2001:db8::8:800:200c:417a/64
    #
    def prefix=(num)
      @prefix = Prefix128.new(num)
    end

    # 
    # Unlike its counterpart IPv6#to_s method, IPv6#to_string 
    # returns the whole IPv6 address and prefix in an uncompressed form
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #   ip6.to_string
    #     #=> "2001:0db8:0000:0000:0008:0800:200c:417a/64"
    #
    def to_string
      "#@address/#@prefix"
    end

    #
    # Returns the IPv6 address in a human readable form,
    # using the compressed address.
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
    #   ip6.to_string
    #     #=> "2001:db8::8:800:200c:417a/64"
    #
    def to_s
      "#{compressed}/#@prefix"
    end

    #
    # Returns a decimal format (unsigned 128 bit) of the
    # IPv6 address
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
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
    #   ip6.network?
    #     #=> false
    #
    #   ip6 = IPAddress "2001:db8:8:800::/64"
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
    #   ip6.hexs
    #     #=> ["2001", "0db8", "0000", "0000", "0008", "0800", "200c", "417a"]
    #
    # Not to be confused with the similar IPv6#to_hex method.
    #
    def hexs
      @address.split(":")
    end

    #
    # Compressed form of the IPv6 address
    #
    #   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
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
      false
    end
    
    #
    # Returns the address portion of an IP in binary format,
    # as a string containing a sequence of 0 and 1
    #
    #   ip = IPAddress("2001:db8::8:800:200c:417a")
    #   ip.bits
    #     #=> "01111111000000000000000000000001"
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
    #   ip6 = IPAddress::IPv4::parse_data " \001\r\270\000\000\000\000\000\b\b\000 \fAz"
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

  class IPAddress::IPv6::Unspecified < IPAddress::IPv6
    def initialize
      @address = ("0000:"*8).chop
      @groups = Array.new(8,0)
      @prefix = Prefix128.new(128)
      @compressed = compress_address
    end # class IPv6::Unspecified
  end

  class IPAddress::IPv6::Loopback < IPAddress::IPv6
    def initialize
      @address = ("0000:"*7)+"0001"
      @groups = Array.new(7,0).push(1) 
      @prefix = Prefix128.new(128)
      @compressed = compress_address
    end
  end # class IPv6::Loopback

  class IPAddress::IPv6::Mapped < IPAddress::IPv6

    attr_reader :ipv4

    def initialize(str)
      string, netmask = str.split("/")
      @ipv4 = IPAddress::IPv4.extract(string)
      super("::FFFF:#{@ipv4.to_ipv6}/#{netmask}")
    end

    def to_s
      "::FFFF:#{@ipv4.address}/#@prefix"
    end

    def mapped?
      true
    end
  
  end # class IPv6::Mapped
  
      

end # module IPAddress

