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
      
      # Check the ip and remove white space
      if IPAddress.valid_ipv6?(ip)
        @groups = self.class.groups(ip)
        @address = IN6FORMAT % @groups
        @compressed = compress_address
      else
        raise ArgumentError, "Invalid IP #{ip.inspect}"
      end
      
      # Check the prefix
      if netmask =~ /^\d{1,3}$/  
        @prefix = Prefix128.new(netmask.to_i)
      else
        @prefix = Prefix128.new(128)
      end

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

    def to_string
      "#@address/#@prefix"
    end

    def to_s
      "#{compressed}/#@prefix"
    end

    def to_i
      to_hex.hex
    end

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
  end
end

class IPAddress::IPv6::Loopback < IPAddress::IPv6
  def initialize
    @address = ("0000:"*7)+"0001"
    @groups = Array.new(7,0).push(1) 
    @prefix = Prefix128.new(128)
  end
end

 

end # module IPAddress

