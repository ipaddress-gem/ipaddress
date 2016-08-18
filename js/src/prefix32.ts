class Prefix32 extends Prefix {

   const IN4MASK: number = 0xffffffff;

   /*
   # Creates a new prefix object for 32 bits IPv4 addresses
   #
   #   prefix = IPAddress::Prefix32.new 24
   #     #=> 24
   */
    public static create(num: number) : Prefix32 {
     if (num < 0 || 32 < num) {
       return null;
       //raise ArgumentError, "Prefix must be in range 0..32, got: #{num}"
     }
     return new Prefix32(num);
   }

   /*
   # Returns the length of the host portion
   # of a netmask.
   #
   #   prefix = Prefix32.new 24
   #
   #   prefix.host_prefix
   #     #=> 8
   */
   public host_prefix() : number {
     return 32 - this.prefix;
   }

   /*
   # Transforms the prefix into a string of bits
   # representing the netmask
   #
   #   prefix = IPAddress::Prefix32.new 24
   #
   #   prefix.bits
   #     #=> "11111111111111111111111100000000"
   */
   public bits() : number {
     "%.32b" % to_u32
   }

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
