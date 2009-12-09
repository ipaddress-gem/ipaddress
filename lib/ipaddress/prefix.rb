module IPAddress
  class Prefix

    include Comparable

    attr_reader :prefix

    MASK = 0xffffffff
    SIZE = 32

    def initialize(num)
      @prefix = num.to_i
    end

    def bits
      # "1" * @prefix + "0" * (SIZE - @prefix)
      sprintf "%0#{SIZE}b", (MASK & ~(MASK >> @prefix))
    end
    
    def to_u32
      [bits].pack("B*").unpack("N").first
    end

    def to_ip
      [bits].pack("B*").unpack("CCCC").join(".")
    end
  
    def to_s
      "#@prefix"
    end
    alias_method :inspect, :to_s

    def to_i
      @prefix
    end

    def octets
      to_ip.split(".").map{|i| i.to_i}
    end
    
    def [](index)
      octets[index]
    end

    def hostmask
      [~to_u32].pack("N").unpack("CCCC").join(".")
    end

    def <=>(oth)
      @prefix <=> oth.to_i
    end

    def self.parse_netmask(netmask)
      octets = netmask.split(".").map{|i| i.to_i}
      num = octets.pack("C"*octets.size).unpack("B*").first.count "1"
      return IPAddress::Prefix.new(num)
    end

  end # class Prefix
end # module IPAddress
