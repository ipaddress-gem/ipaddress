
 class Prefix128 < Prefix

   #
   # Creates a new prefix object for 128 bits IPv6 addresses
   #
   #   prefix = IPAddress::Prefix128.new 64
   #     #=> 64
   #
   def initialize(num=128)
     unless (0..128).include? num.to_i
       raise ArgumentError, "Prefix must be in range 0..128, got: #{num}"
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

   #
   # Returns the length of the host portion
   # of a netmask.
   #
   #   prefix = Prefix128.new 96
   #
   #   prefix.host_prefix
   #     #=> 32
   #
   def host_prefix
     128 - @prefix
   end

 end # class Prefix123 < Prefix
