require 'ipaddress/ipbase'
require 'ipaddress/prefix'

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
# This library provides a complete 
#
#
module IPAddress; 
  class IPv6 < IPBase
    
    include IPAddress
    include Enumerable  
    include Comparable                  

    IN6FORMAT = ("%.4x:"*8).chop
    
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


    def address
      @address
    end

    def groups
      @groups
    end

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


    def to_string
      "#@address/#@prefix"
    end

    def to_s
      "#{compressed}/#@prefix"
    end

    def to_i
      to_hex.hex
    end
    alias_method :to_u128, :to_i

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

    def to_hex
      hexs.to_s
    end
    
    def data
      @groups.pack("n8")
    end

    def hexs
      @address.split(":")
    end

    def compressed
      @compressed
    end

    def unspecified?
      @prefix == 128 and @compressed == "::"
    end

    def loopback?
      @prefix == 128 and @compressed == "::1"
    end

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

    def self.compress(str)
      self.new(str).compressed
    end

    def literal
      @address.gsub(":","-") + ".ipv6-literal.net"
    end
    
    def self.groups(str)
      l, r = if str =~ /^(.*)::(.*)$/
               [$1,$2].map {|i| i.split ":"}
             else
               [str.split(":"),[]]
             end
      (l + Array.new(8-l.size-r.size, '0') + r).map {|i| i.hex}
    end
    
    def self.parse_data(str)
      self.new(IN6FORMAT % str.unpack("n8"))
    end

    def self.parse_u128(u128, prefix=128)
      str = IN6FORMAT % (0..7).map{|i| (u128>>(112-16*i))&0xffff}
      self.new(str + "/#{prefix}")
    end

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

