require 'ipaddress/ipbase'
require 'ipaddress/prefix'
require 'pp'

# 
# =Name
# 
# Net::DNS::Resolver - DNS resolver class
#
# =Synopsis
#
#    require 'net/dns/resolver'
#
# =Description
# 
# The Net::DNS::Resolver class implements a complete DNS resolver written
# in pure Ruby, without a single C line of code. It has all of the 
# tipical properties of an evoluted resolver, and a bit of OO which 
# comes from having used Ruby. 
# 
# This project started as a porting of the Net::DNS Perl module, 
# written by Martin Fuhr, but turned out (in the last months) to be
# an almost complete rewriting. Well, maybe some of the features of
# the Perl version are still missing, but guys, at least this is
# readable code! 
#
# FIXME
#
# =Environment
#
# The Following Environment variables can also be used to configure 
# the resolver:
#
# * +RES_NAMESERVERS+: A space-separated list of nameservers to query.
#
#      # Bourne Shell
#      $ RES_NAMESERVERS="192.168.1.1 192.168.2.2 192.168.3.3"
#      $ export RES_NAMESERVERS
#
#      # C Shell
#      % setenv RES_NAMESERVERS "192.168.1.1 192.168.2.2 192.168.3.3"
#
# * +RES_SEARCHLIST+: A space-separated list of domains to put in the 
#   search list.
#
#      # Bourne Shell
#      $ RES_SEARCHLIST="example.com sub1.example.com sub2.example.com"
#      $ export RES_SEARCHLIST
#
#      # C Shell
#      % setenv RES_SEARCHLIST "example.com sub1.example.com sub2.example.com"
#  
# * +LOCALDOMAIN+: The default domain.
#
#      # Bourne Shell
#      $ LOCALDOMAIN=example.com
#      $ export LOCALDOMAIN
#
#      # C Shell
#      % setenv LOCALDOMAIN example.com
#
# * +RES_OPTIONS+: A space-separated list of resolver options to set.  
#   Options that take values are specified as option:value.
#
#      # Bourne Shell
#      $ RES_OPTIONS="retrans:3 retry:2 debug"
#      $ export RES_OPTIONS
#
#      # C Shell
#      % setenv RES_OPTIONS "retrans:3 retry:2 debug"
#        


module IPAddress; class IPv4 < IPBase
                    
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
  # Creates a new IPv4 address.
  #
  # An IPv4 address can be expressed in any of the following forms:
  # 
  #   * 10.1.1.1
  #   *
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
        @prefix = Prefix.new(netmask.to_i)
      elsif IPAddress.valid_ipv4_netmask?(netmask)  # netmask in IP format
        @prefix = Prefix.parse_netmask(netmask)
      else  # invalid netmask
        raise ArgumentError, "Invalid netmask #{netmask}"
      end
    else  # netmask is nil, reverting to defaul classful mask
      @prefix = prefix_from_ip(@address)
    end

    # Array formed with the IP octets
    @octets = @address.split(".").map{|i| i.to_i}

  end # def initialize

  def address
    @address
  end
  
  def prefix
    @prefix
  end

  def prefix=(num)
    @prefix = Prefix.new(num)
  end

  def octets
    @octets
  end
  
  def to_s
    "#@address/#@prefix"
  end

  def netmask
    @prefix.to_ip
  end

  def netmask=(addr)
    @prefix = Prefix.parse_netmask(addr)
  end
  
  def to_u32
    @octets.pack("CCCC").unpack("N").first
  end

  #
  # Returns the octet specified by index
  #
  #   ip = IPAddress("172.16.100.50/24")
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
  #   ip.bits
  #     #=> "01111111000000000000000000000001"
  #
  def bits
    @octets.pack("CCCC").unpack("B*").first
  end

  #
  # Returns the broadcast address for the given IP.
  #
  #   ip = IPAddress("172.16.10.64/24")
  #   ip.broadcast.to_s
  #     #=> "172.16.10.255/24"
  #
  def broadcast
    self.class.parse_u32(broadcast_u32, @prefix)
  end
  
  #
  # Check if the IP address is actually a network
  #
  #   ip = IPAddress("172.16.10.64/24")
  #   ip.network?
  #     #=> false
  # 
  #   ip = IPAddress("172.16.10.64/26")
  #   ip.network?
  #     #=> true
  #
  def network?
    to_u32 | @prefix.to_u32 == @prefix.to_u32
  end

  #
  # Returns a new IPv4 object with the network number 
  # for the given IP.
  #
  #   ip = IPAddress("172.16.10.64/24")
  #   ip.network.to_s
  #     #=> "172.16.10.0/24"
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
  #   ip.first.to_s
  #     #=> "192.168.100.1/24"
  #
  # The object IP doesn't need to be a network: the method
  # automatically gets the network number from it
  #
  #   ip = IPAddress("192.168.100.50/24")
  #   ip.first.to_s
  #     #=> "192.168.100.1/24"
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
  # host IP address is 192.168.100.1.
  #
  #   ip = IPAddress("192.168.100.0/24")
  #   ip.last.to_s
  #     #=> "192.168.100.254/24"
  #
  # The object IP doesn't need to be a network: the method
  # automatically gets the network number from it
  #
  #   ip = IPAddress("192.168.100.50/24")
  #   ip.last.to_s
  #     #=> "192.168.100.254/24"
  #
  def last
    self.class.parse_u32(broadcast_u32-1, @prefix)
  end

  #
  # Iterates over all the hosts IP addresses for the given
  # network (or IP address).
  #
  #   ip = IPaddress("10.0.0.1/29")
  #   ip.each do |i|
  #     p i
  #   end
  #     #=> "10.0.0.1"
  #     #=> "10.0.0.2"
  #     #=> "10.0.0.3"
  #     #=> "10.0.0.4"
  #     #=> "10.0.0.5"
  #     #=> "10.0.0.6"
  #
  def each_host
    hosts.each do |i|
      yield i
    end
  end

  #
  # Iterates over all the IP addresses for the given
  # network (or IP address).
  #
  # The object yielded is a new IPv4 object created
  # from the iteration.
  #
  #   ip = IPaddress("10.0.0.1/29")
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
  #   ip2 = IPAddress ""172.16.0.1/16"
  #   ip3 = IPAddress ""10.100.100.1/16"
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
  # number and the broadcast address.
  #
  #   ip = IPaddress("10.0.0.1/29")
  #   ip.size
  #     #=> 8
  #
  def size
    to_a.size
  end

  #
  # Returns an array with the IP addresses of
  # all the hosts in the network.
  # 
  #   ip = IPaddress("10.0.0.1/29")
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
  #   ip = IPaddress("10.0.0.1/29")
  #   ip.network_u32
  #     #=> 167772160
  #
  def network_u32
    to_u32 & @prefix.to_u32
  end

  #
  # Returns the broadcast address in Unsigned 32bits format
  #
  #   ip = IPaddress("10.0.0.1/29")
  #   ip.broadcast_u32
  #     #=> 167772167
  #
  def broadcast_u32
    [to_u32 | ~@prefix.to_u32].pack("N").unpack("N").first
  end

  #
  # Checks whether a subnet includes the given IP address.
  #
  # Accepts either string with the IP or and IPAddress::IPv4
  # object.
  #
  #   ip = IPAddress("192.168.10.100/24")
  #
  #   addr = IPAddress("192.168.10.102/24")
  #   ip.include? addr
  #     #=> true
  #
  #   ip.include? IPAddress("172.16.0.48/16")
  #     #=> false
  #
  def include?(oth)
    to_a.map{|i| i.address}.include?(oth.address) and @prefix <= oth.prefix
  end

  #
  # Returns the IP address in in-addr.arpa format
  # for DNS lookups
  #
  #   ip = IPAddress("172.16.100.50/24")
  #   ip.reverse
  #     #=> "50.100.16.172.in-addr.arpa"
  #
  def reverse
    @octets.reverse.join(".") + ".in-addr.arpa"
  end
  
  #
  # Subnetting a network
  #
  # If the IP Address is a network, it can be divided into
  # multiple networks. If +self+ is not a network, the
  # method will calculate the network from the IP and then
  # subnet it.
  #
  # If +num+ is an even number, the resulting networks will be
  # divided evenly from the supernet.
  #
  #   network = IPAddress("172.16.10.0/24")
  #   network / 4
  #     #=> ["172.16.10.0/26",
  #          "172.16.10.64/26",
  #          "172.16.10.128/26",
  #          "172.16.10.192/26"]
  #
  # If +num+ is a odd number, the supernet will be 
  # divided into num-1 networks with a even number of hosts and
  # a last network with the remaining addresses.
  #
  #   network = IPAddress("172.16.10.0/24")
  #   network / 3
  #     #=> ["172.16.10.0/26",
  #          "172.16.10.64/26",
  #          "172.16.10.128/25"]
  #
  # Returns an array of IPAddress objects,
  #
  def subnet(subnets=2)
    unless (1..(2**(32-prefix.to_i))).include? subnets
      raise ArgumentError, "Value #{subnets} out of range" 
    end
    
    if subnets.even?
      return subnet_even(subnets)
    else
      return subnet_odd(subnets)
    end
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
  #   ip.supernet(23).to_s
  #     #=> "172.16.10.0/23"
  #
  # However if you supernet it with a /22 prefix, the
  # network address will change:
  #
  #   ip.supernet(22).to_s
  #     #=> "172.16.8.0/22"
  # 
  def supernet(new_prefix)
    raise ArgumentError, "Can't supernet a /1 network" if new_prefix < 1
    raise ArgumentError, "New prefix must be smaller than existing prefix" if new_prefix >= @prefix.to_i
    self.class.new(@address+"/#{new_prefix}").network
  end
  
  
  def -(oth)
    return (to_u32 - oth.to_u32).abs
  end

  #
  # Creates a new IPv4 object from an
  # unsigned 32bits integer.
  #
  #   ip = IPAddress::IPv4::parse_u32(167772160)
  #   ip.prefix = 8
  #   ip.to_s
  #     #=> "10.0.0.0/8"
  #
  # The +prefix+ parameter is optional:
  #
  #   ip = IPAddress::IPv4::parse_u32(167772160, 8)
  #   ip.to_s
  #     #=> "10.0.0.0/8"
  #
  def self.parse_u32(u32, prefix=nil)
    ip = [u32].pack("N").unpack("CCCC").join(".")
    if prefix
      IPAddress::IPv4.new(ip+"/#{prefix}")
    else
      IPAddress::IPv4.new(ip)
    end
  end
  
  def self.summarize(*args)
    puts "CHIAMATA A SUMMARIZE!!:"
    pp args
    
    # return argument if only one network given
    return args.flatten if args.size == 1
    
    enum=args.sort.each_cons(2)
    unless enum.all? {|x,y| x.broadcast_u32 == y.network_u32-1}
      raise ArgumentError, "Networks must be contiguous to be summarized"
    end
    
    result = []
    enum.each do |x,y|
      snet = x.supernet(x.prefix.to_i-1)
      if snet.include? y
        result << snet
      else
        result << x.network unless result.any?{|i| i.include? x}
      end
    end

    lst = enum.to_a.flatten.last.network
    result << lst unless result.any?{|i| i.include? lst}
    
    if result.size == args.size
      return result
    else
      return self.summarize(*result)
    end
  end
  
  

  #
  # private methods
  #

  private
  
  def prefix_from_netmask(netmask)
    octets = netmask.split(".").map{|i| i.to_i}
    octets.pack("C"*octets.size).unpack("B*").first.count "1"
  end

  def netmask_from_prefix(prefix)
    bits = "1" * prefix + "0" * (32 - prefix)
    bits.pack("B*").unpack("CCCC").join(".")
  end

  def bits_from_address(ip)
    ip.split(".").map{|i| i.to_i}.pack("CCCC").unpack("B*").first
  end
  
  def prefix_from_ip(ip)
    bits = bits_from_address(ip)
    CLASSFUL.each {|reg,prefix| return prefix if bits =~ reg}
  end

  
  def subnet_even(subnets)
    new_prefix = @prefix.to_i + Math::log2(subnets).ceil
    networks = Array.new
    (0..subnets-1).each do |i|
      mul = i * (2**(32-new_prefix))
      networks << IPAddress::IPv4.parse_u32(network_u32+mul, new_prefix)
    end
    return networks
  end
  
  def subnet_odd(subnets)
    networks = subnet_even(subnets+1)
    networks[-2..-1] = IPAddress::IPv4.summarize(networks[-2],networks[-1])
    return networks
  end
  
  
end; end # module IPAddress; class IPv4

