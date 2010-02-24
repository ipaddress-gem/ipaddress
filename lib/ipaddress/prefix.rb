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
  # IPAddresS::Prefix is the parent class for IPAddress::Prefix32 
  # and IPAddress::Prefix128, 


  class Prefix

    include Comparable

    attr_reader :prefix

    def initialize(num)
      @prefix = num.to_i
    end

    def to_s
      "#@prefix"
    end
    alias_method :inspect, :to_s

    def to_i
      @prefix
    end

    def <=>(oth)
      @prefix <=> oth.to_i
    end

   end # class Prefix


  class Prefix32 < Prefix

    def initialize(num)
      unless (1..32).include? num
        raise ArgumentError, "Prefix must be in range 1..128, got: #{num}"
      end
      super(num)
    end

    def bits
      "1" * @prefix + "0" * (32 - @prefix)
    end

    def to_ip
      [bits].pack("B*").unpack("CCCC").join(".")
    end

    def octets
      to_ip.split(".").map{|i| i.to_i}
    end

    def to_u32
      [bits].pack("B*").unpack("N").first
    end
    
    def [](index)
      octets[index]
    end

    def hostmask
      [~to_u32].pack("N").unpack("CCCC").join(".")
    end
    
    def self.parse_netmask(netmask)
      octets = netmask.split(".").map{|i| i.to_i}
      num = octets.pack("C"*octets.size).unpack("B*").first.count "1"
      return IPAddress::Prefix.new(num)
    end
    
  end # class Prefix32 < Prefix

  class Prefix128 < Prefix

    def initialize(num=128)
      unless (1..128).include? num.to_i
        raise ArgumentError, "Prefix must be in range 1..128, got: #{num}"
      end
      super(num.to_i)
    end

    def bits
      "1" * @prefix + "0" * (128 - @prefix)
    end

    def to_u128
      eval "0b#{bits}.to_i"
    end

  end # class Prefix123 < Prefix

end # module IPAddress
