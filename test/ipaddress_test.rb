require 'test_helper'

class IPAddressTest < Test::Unit::TestCase

  def setup
    @valid_ipv4   = "172.16.10.1/24"
    @valid_ipv6   = "2001:db8::8:800:200c:417a/64"
    @valid_mapped = "::13.1.68.3"

    @invalid_ipv4   = "10.0.0.256"
    @invalid_ipv6   = ":1:2:3:4:5:6:7"
    @invalid_mapped = "::1:2.3.4"

    @ipv4class   = IPAddress::IPv4
    @ipv6class   = IPAddress::IPv6
    @mappedclass = IPAddress::IPv6::Mapped

    @method = Module.method("IPAddress")
  end

  def test_method_IPAddress
    assert_nothing_raised {@method.call(@valid_ipv4)}
    assert_nothing_raised {@method.call(@valid_ipv6)} 
    assert_nothing_raised {@method.call(@valid_mapped)}

    assert_instance_of @ipv4class, @method.call(@valid_ipv4) 
    assert_instance_of @ipv6class, @method.call(@valid_ipv6) 
    assert_instance_of @mappedclass, @method.call(@valid_mapped)

    assert_raise(ArgumentError) {@method.call(@invalid_ipv4)}
    assert_raise(ArgumentError) {@method.call(@invalid_ipv6)}
    assert_raise(ArgumentError) {@method.call(@invalid_mapped)}

  end

  def test_module_method_valid?
    assert_equal true, IPAddress::valid?("10.0.0.1")
    assert_equal true, IPAddress::valid?("10.0.0.0")
    assert_equal true, IPAddress::valid?("2002::1")
    assert_equal true, IPAddress::valid?("dead:beef:cafe:babe::f0ad")
    assert_equal false, IPAddress::valid?("10.0.0.256")
    assert_equal false, IPAddress::valid?("10.0.0.0.0")
    assert_equal false, IPAddress::valid?("10.0.0")
    assert_equal false, IPAddress::valid?("10.0")
    assert_equal false, IPAddress::valid?("2002:::1")
  end

  def test_module_method_valid_ipv4_netmark?
    assert_equal true, IPAddress::valid_ipv4_netmask?("255.255.255.0")
    assert_equal false, IPAddress::valid_ipv4_netmask?("10.0.0.1")
  end

end
 
class Prefix32Test < Test::Unit::TestCase

  def setup
    @netmask0  = "0.0.0.0"
    @netmask8  = "255.0.0.0"
    @netmask16 = "255.255.0.0"
    @netmask24 = "255.255.255.0"
    @netmask30 = "255.255.255.252"
    @netmasks  = [@netmask0,@netmask8,@netmask16,@netmask24,@netmask30]
    
    @prefix_hash = {
      "0.0.0.0"         => 0,
      "255.0.0.0"       => 8,
      "255.255.0.0"     => 16,
      "255.255.255.0"   => 24,
      "255.255.255.252" => 30}

    @octets_hash = {
      [0,0,0,0]         => 0,
      [255,0,0,0]       => 8,
      [255,255,0,0]     => 16,
      [255,255,255,0]   => 24,
      [255,255,255,252] => 30}

    @u32_hash = {
      0  => 0,
      8  => 4278190080,
      16 => 4294901760,
      24 => 4294967040,
      30 => 4294967292}
    
    @klass = IPAddress::Prefix32
  end

  def test_attributes
    @prefix_hash.values.each do |num|
      prefix = @klass.new(num)
      assert_equal num, prefix.prefix
    end
  end

  def test_parse_netmask
    @prefix_hash.each do |netmask, num|
      prefix = @klass.parse_netmask(netmask)
      assert_equal num, prefix.prefix
      assert_instance_of @klass, prefix
    end
  end

  def test_method_to_ip
    @prefix_hash.each do |netmask, num|
      prefix = @klass.new(num)
      assert_equal netmask, prefix.to_ip
    end
  end
  
  def test_method_to_s
    prefix = @klass.new(8)
    assert_equal "8", prefix.to_s
  end
  
  def test_method_bits
    prefix = @klass.new(16)
    str = "1"*16 + "0"*16
    assert_equal str, prefix.bits
  end

  def test_method_to_u32
    @u32_hash.each do |num,u32|
      assert_equal u32, @klass.new(num).to_u32
    end
  end

  def test_method_plus
    p1 = @klass.new 8
    p2 = @klass.new 10
    assert_equal 18, p1+p2
    assert_equal 12, p1+4
  end

  def test_method_minus
    p1 = @klass.new 8
    p2 = @klass.new 24
    assert_equal 16, p1-p2
    assert_equal 16, p2-p1
    assert_equal 20, p2-4
  end

  def test_initialize
    assert_raise (ArgumentError) do
      @klass.new 33
    end
    assert_nothing_raised do
      @klass.new 8
    end
    assert_instance_of @klass, @klass.new(8)
  end

  def test_method_octets
    @octets_hash.each do |arr,pref|
      prefix = @klass.new(pref)
      assert_equal prefix.octets, arr
    end
  end

  def test_method_brackets
    @octets_hash.each do |arr,pref|
      prefix = @klass.new(pref)
      arr.each_with_index do |oct,index|
        assert_equal prefix[index], oct
      end
    end
  end

  def test_method_hostmask
    prefix = @klass.new(8)
    assert_equal "0.255.255.255", prefix.hostmask
  end
    
end # class Prefix32Test

  
class Prefix128Test < Test::Unit::TestCase
  
  def setup
    @u128_hash = {
      32  => 340282366841710300949110269838224261120,
      64 => 340282366920938463444927863358058659840,
      96 => 340282366920938463463374607427473244160,
      126 => 340282366920938463463374607431768211452}
    
    @klass = IPAddress::Prefix128
  end

  def test_initialize
    assert_raise (ArgumentError) do
      @klass.new 129
    end
    assert_nothing_raised do
      @klass.new 64
    end
    assert_instance_of @klass, @klass.new(64)
  end

  def test_method_bits
    prefix = @klass.new(64)
    str = "1"*64 + "0"*64
    assert_equal str, prefix.bits
  end

  def test_method_to_u32
    @u128_hash.each do |num,u128|
      assert_equal u128, @klass.new(num).to_u128
    end
  end

end # class Prefix128Test

 
class IPv4Test < Test::Unit::TestCase

  def setup
    @klass = IPAddress::IPv4

    @valid_ipv4 = {
      "0.0.0.0/0" => ["0.0.0.0", 0],
      "10.0.0.0" => ["10.0.0.0", 32],
      "10.0.0.1" => ["10.0.0.1", 32],
      "10.0.0.1/24" => ["10.0.0.1", 24],
      "10.0.0.1/255.255.255.0" => ["10.0.0.1", 24]}
    
    @invalid_ipv4 = ["10.0.0.256",
                     "10.0.0.0.0",
                     "10.0.0",
                     "10.0"]

    @valid_ipv4_range = ["10.0.0.1-254",
                         "10.0.1-254.0",
                         "10.1-254.0.0"]

    @netmask_values = {
      "0.0.0.0/0"        => "0.0.0.0",
      "10.0.0.0/8"       => "255.0.0.0",
      "172.16.0.0/16"    => "255.255.0.0",
      "192.168.0.0/24"   => "255.255.255.0",
      "192.168.100.4/30" => "255.255.255.252"}

    @decimal_values ={      
      "0.0.0.0/0"        => 0,
      "10.0.0.0/8"       => 167772160,
      "172.16.0.0/16"    => 2886729728,
      "192.168.0.0/24"   => 3232235520,
      "192.168.100.4/30" => 3232261124}
    
    @ip = @klass.new("172.16.10.1/24")
    @network = @klass.new("172.16.10.0/24")
    
    @broadcast = {
      "10.0.0.0/8"       => "10.255.255.255/8",
      "172.16.0.0/16"    => "172.16.255.255/16",
      "192.168.0.0/24"   => "192.168.0.255/24",
      "192.168.100.4/30" => "192.168.100.7/30"}
    
    @networks = {
      "10.5.4.3/8"       => "10.0.0.0/8",
      "172.16.5.4/16"    => "172.16.0.0/16",
      "192.168.4.3/24"   => "192.168.4.0/24",
      "192.168.100.5/30" => "192.168.100.4/30"}

    @class_a = @klass.new("10.0.0.1/8")
    @class_b = @klass.new("172.16.0.1/16")
    @class_c = @klass.new("192.168.0.1/24")

    @classful = {
      "10.1.1.1"  => 8,
      "150.1.1.1" => 16,
      "200.1.1.1" => 24 }
    
  end

  def test_initialize
    @valid_ipv4.keys.each do |i|
      ip = @klass.new(i)
      assert_instance_of @klass, ip
    end
    assert_instance_of IPAddress::Prefix32, @ip.prefix
    assert_raise (ArgumentError) do
      @klass.new 
    end
    assert_nothing_raised do
      @klass.new "10.0.0.0/8"
    end
  end

  def test_initialize_format_error
    @invalid_ipv4.each do |i|
      assert_raise(ArgumentError) {@klass.new(i)}
    end
    assert_raise (ArgumentError) {@klass.new("10.0.0.0/asd")}
  end

  def test_initialize_without_prefix
    assert_nothing_raised do
      @klass.new("10.10.0.0")
    end
    ip = @klass.new("10.10.0.0")
    assert_instance_of IPAddress::Prefix32, ip.prefix
    assert_equal 32, ip.prefix.to_i
  end

  def test_attributes
    @valid_ipv4.each do |arg,attr|
      ip = @klass.new(arg)
      assert_equal attr.first, ip.address
      assert_equal attr.last, ip.prefix.to_i
    end
  end

  def test_octets
    ip = @klass.new("10.1.2.3/8")
    assert_equal ip.octets, [10,1,2,3]
  end
  
  def test_initialize_should_require_ip
    assert_raise(ArgumentError) { @klass.new }
  end

  def test_method_data
    assert_equal "\254\020\n\001", @ip.data
  end
  
  def test_method_to_string
    @valid_ipv4.each do |arg,attr|
      ip = @klass.new(arg)
      assert_equal attr.join("/"), ip.to_string
    end
  end

  def test_method_to_s
    @valid_ipv4.each do |arg,attr|
      ip = @klass.new(arg)
      assert_equal attr.first, ip.to_s
    end
  end

  def test_netmask
    @netmask_values.each do |addr,mask|
      ip = @klass.new(addr)
      assert_equal mask, ip.netmask
    end
  end

  def test_method_to_u32
    @decimal_values.each do |addr,int|
      ip = @klass.new(addr)
      assert_equal int, ip.to_u32
    end
  end

  def test_method_network?
    assert_equal true, @network.network?
    assert_equal false, @ip.network?
  end

  def test_method_broadcast
    @broadcast.each do |addr,bcast|
      ip = @klass.new(addr)
      assert_instance_of @klass, ip.broadcast
      assert_equal bcast, ip.broadcast.to_string
    end
  end
  
  def test_method_network
    @networks.each do |addr,net|
      ip = @klass.new addr
      assert_instance_of @klass, ip.network
      assert_equal net, ip.network.to_string
    end
  end

  def test_method_bits
    ip = @klass.new("127.0.0.1")
    assert_equal "01111111000000000000000000000001", ip.bits
  end

  def test_method_first
    ip = @klass.new("192.168.100.0/24")
    assert_instance_of @klass, ip.first
    assert_equal "192.168.100.1", ip.first.to_s
    ip = @klass.new("192.168.100.50/24")
    assert_instance_of @klass, ip.first
    assert_equal "192.168.100.1", ip.first.to_s
  end

  def test_method_last
    ip = @klass.new("192.168.100.0/24")
    assert_instance_of @klass, ip.last
    assert_equal  "192.168.100.254", ip.last.to_s
    ip = @klass.new("192.168.100.50/24")
    assert_instance_of @klass, ip.last
    assert_equal  "192.168.100.254", ip.last.to_s
  end
  
  def test_method_each_host
    ip = @klass.new("10.0.0.1/29")
    arr = []
    ip.each_host {|i| arr << i.to_s}
    expected = ["10.0.0.1","10.0.0.2","10.0.0.3",
                "10.0.0.4","10.0.0.5","10.0.0.6"]
    assert_equal expected, arr
  end

  def test_method_each
    ip = @klass.new("10.0.0.1/29")
    arr = []
    ip.each {|i| arr << i.to_s}
    expected = ["10.0.0.0","10.0.0.1","10.0.0.2",
                "10.0.0.3","10.0.0.4","10.0.0.5",
                "10.0.0.6","10.0.0.7"]
    assert_equal expected, arr
  end

  def test_method_size
    ip = @klass.new("10.0.0.1/29")
    assert_equal 8, ip.size
  end

  def test_method_hosts
    ip = @klass.new("10.0.0.1/29")
    expected = ["10.0.0.1","10.0.0.2","10.0.0.3",
                "10.0.0.4","10.0.0.5","10.0.0.6"]
    assert_equal expected, ip.hosts.map {|i| i.to_s}
  end

  def test_method_network_u32
    assert_equal 2886732288, @ip.network_u32
  end
  
  def test_method_broadcast_u32
    assert_equal 2886732543, @ip.broadcast_u32
  end

  def test_method_include?
    ip = @klass.new("192.168.10.100/24")
    addr = @klass.new("192.168.10.102/24")
    assert_equal true, ip.include?(addr)
    assert_equal false, ip.include?(@klass.new("172.16.0.48"))
    ip = @klass.new("10.0.0.0/8")
    assert_equal true, ip.include?(@klass.new("10.0.0.0/9"))
    assert_equal true, ip.include?(@klass.new("10.1.1.1/32"))
    assert_equal true, ip.include?(@klass.new("10.1.1.1/9"))
    assert_equal false, ip.include?(@klass.new("172.16.0.0/16"))
    assert_equal false, ip.include?(@klass.new("10.0.0.0/7"))
    assert_equal false, ip.include?(@klass.new("5.5.5.5/32"))
    assert_equal false, ip.include?(@klass.new("11.0.0.0/8"))
    ip = @klass.new("13.13.0.0/13")
    assert_equal false, ip.include?(@klass.new("13.16.0.0/32"))    
  end

  def test_method_include_all?
    ip = @klass.new("192.168.10.100/24")
    addr1 = @klass.new("192.168.10.102/24")
    addr2 = @klass.new("192.168.10.103/24")    
    assert_equal true, ip.include_all?(addr1,addr2)
    assert_equal false, ip.include_all?(addr1, @klass.new("13.16.0.0/32"))
  end

  def test_method_ipv4?
    assert_equal true, @ip.ipv4?
  end
  
  def test_method_ipv6?
    assert_equal false, @ip.ipv6?
  end
    
  def test_method_private?
    assert_equal true, @klass.new("192.168.10.50/24").private?
    assert_equal true, @klass.new("192.168.10.50/16").private?
    assert_equal true, @klass.new("172.16.77.40/24").private?
    assert_equal true, @klass.new("172.16.10.50/14").private?
    assert_equal true, @klass.new("10.10.10.10/10").private?
    assert_equal true, @klass.new("10.0.0.0/8").private?
    assert_equal false, @klass.new("192.168.10.50/12").private?
    assert_equal false, @klass.new("3.3.3.3").private?
    assert_equal false, @klass.new("10.0.0.0/7").private?
    assert_equal false, @klass.new("172.32.0.0/12").private?
    assert_equal false, @klass.new("172.16.0.0/11").private?
    assert_equal false, @klass.new("192.0.0.2/24").private?
  end

  def test_method_octet
    assert_equal 172, @ip[0]
    assert_equal 16, @ip[1]
    assert_equal 10, @ip[2]
    assert_equal 1, @ip[3]
  end

  def test_method_a?
    assert_equal true, @class_a.a?
    assert_equal false, @class_b.a?
    assert_equal false, @class_c.a?
  end

  def test_method_b?
    assert_equal true, @class_b.b?
    assert_equal false, @class_a.b?
    assert_equal false, @class_c.b?
  end

  def test_method_c?
    assert_equal true, @class_c.c?
    assert_equal false, @class_a.c?
    assert_equal false, @class_b.c?
  end

  def test_method_to_ipv6
    assert_equal "ac10:0a01", @ip.to_ipv6
  end
  
  def test_method_reverse
    assert_equal "1.10.16.172.in-addr.arpa", @ip.reverse
  end
  
  def test_method_comparabble
    ip1 = @klass.new("10.1.1.1/8")
    ip2 = @klass.new("10.1.1.1/16")
    ip3 = @klass.new("172.16.1.1/14")
    ip4 = @klass.new("10.1.1.1/8")

    # ip1 should be major than ip2
    assert_equal true, ip1 > ip2
    assert_equal false, ip1 < ip2
    assert_equal false, ip2 > ip1        
    # ip2 should be minor than ip3
    assert_equal true, ip2 < ip3
    assert_equal false, ip2 > ip3
    # ip1 should be minor than ip3
    assert_equal true, ip1 < ip3
    assert_equal false, ip1 > ip3
    assert_equal false, ip3 < ip1
    # ip1 should be equal to itself
    assert_equal true, ip1 == ip1
    # ip1 should be equal to ip4
    assert_equal true, ip1 == ip4
    # test sorting
    arr = ["10.1.1.1/16","10.1.1.1/8","172.16.1.1/14"]
    assert_equal arr, [ip1,ip2,ip3].sort.map{|s| s.to_string}
  end

  def test_method_minus
    ip1 = @klass.new("10.1.1.1/8")
    ip2 = @klass.new("10.1.1.10/8")    
    assert_equal 9, ip2 - ip1
    assert_equal 9, ip1 - ip2
  end

  def test_method_plus
    ip1 = @klass.new("172.16.10.1/24")
    ip2 = @klass.new("172.16.11.2/24")
    assert_equal ["172.16.10.0/23"], (ip1+ip2).map{|i| i.to_string}

    ip2 = @klass.new("172.16.12.2/24")
    assert_equal [ip1.network.to_string, ip2.network.to_string], 
    (ip1 + ip2).map{|i| i.to_string}

    ip1 = @klass.new("10.0.0.0/23")
    ip2 = @klass.new("10.0.2.0/24")
    assert_equal ["10.0.0.0/23","10.0.2.0/24"], (ip1+ip2).map{|i| i.to_string}

    ip1 = @klass.new("10.0.0.0/23")
    ip2 = @klass.new("10.0.2.0/24")
    assert_equal ["10.0.0.0/23","10.0.2.0/24"], (ip2+ip1).map{|i| i.to_string}

    ip1 = @klass.new("10.0.0.0/16")
    ip2 = @klass.new("10.0.2.0/24")
    assert_equal ["10.0.0.0/16"], (ip1+ip2).map{|i| i.to_string}

    ip1 = @klass.new("10.0.0.0/23")
    ip2 = @klass.new("10.1.0.0/24")
    assert_equal ["10.0.0.0/23","10.1.0.0/24"], (ip1+ip2).map{|i| i.to_string}

  end
  
  def test_method_netmask_equal
    ip = @klass.new("10.1.1.1/16")
    assert_equal 16, ip.prefix.to_i
    ip.netmask = "255.255.255.0"
    assert_equal 24, ip.prefix.to_i
  end

   def test_method_subnet
     assert_raise(ArgumentError) {@ip.subnet(0)}
     assert_raise(ArgumentError) {@ip.subnet(257)}

     arr = ["172.16.10.0/27", "172.16.10.32/27", "172.16.10.64/27", 
            "172.16.10.96/27", "172.16.10.128/27", "172.16.10.160/27", 
            "172.16.10.192/27", "172.16.10.224/27"]
     assert_equal arr, @network.subnet(8).map {|s| s.to_string}
     arr = ["172.16.10.0/27", "172.16.10.32/27", "172.16.10.64/27", 
            "172.16.10.96/27", "172.16.10.128/27", "172.16.10.160/27", 
            "172.16.10.192/26"]
     assert_equal arr, @network.subnet(7).map {|s| s.to_string}
     arr = ["172.16.10.0/27", "172.16.10.32/27", "172.16.10.64/27", 
            "172.16.10.96/27", "172.16.10.128/26", "172.16.10.192/26"]
     assert_equal arr, @network.subnet(6).map {|s| s.to_string}
     arr = ["172.16.10.0/27", "172.16.10.32/27", "172.16.10.64/27", 
            "172.16.10.96/27", "172.16.10.128/25"]
     assert_equal arr, @network.subnet(5).map {|s| s.to_string}
     arr = ["172.16.10.0/26", "172.16.10.64/26", "172.16.10.128/26", 
            "172.16.10.192/26"]
     assert_equal arr, @network.subnet(4).map {|s| s.to_string}
     arr = ["172.16.10.0/26", "172.16.10.64/26", "172.16.10.128/25"]
     assert_equal arr, @network.subnet(3).map {|s| s.to_string}
     arr = ["172.16.10.0/25", "172.16.10.128/25"]
     assert_equal arr, @network.subnet(2).map {|s| s.to_string}
     arr = ["172.16.10.0/24"]
     assert_equal arr, @network.subnet(1).map {|s| s.to_string}
   end

   def test_method_supernet
     assert_raise(ArgumentError) {@ip.supernet(0)}
     assert_raise(ArgumentError) {@ip.supernet(24)}     
     assert_equal "172.16.10.0/23", @ip.supernet(23).to_string
     assert_equal "172.16.8.0/22", @ip.supernet(22).to_string
   end

  def test_classmethod_parse_u32
    @decimal_values.each do  |addr,int|
      ip = @klass.parse_u32(int)
      ip.prefix = addr.split("/").last.to_i
      assert_equal ip.to_string, addr
    end
  end

  def test_classhmethod_extract
    str = "foobar172.16.10.1barbaz"
    assert_equal "172.16.10.1", @klass.extract(str).to_s
  end

  def test_classmethod_summarize
    
    # Should return self if only one network given
    assert_equal [@ip.network], @klass.summarize(@ip)

    # Summarize homogeneous networks
    ip1 = @klass.new("172.16.10.1/24")
    ip2 = @klass.new("172.16.11.2/24")
    assert_equal ["172.16.10.0/23"], @klass.summarize(ip1,ip2).map{|i| i.to_string}

    ip1 = @klass.new("10.0.0.1/24")
    ip2 = @klass.new("10.0.1.1/24")
    ip3 = @klass.new("10.0.2.1/24")
    ip4 = @klass.new("10.0.3.1/24")
    assert_equal ["10.0.0.0/22"], @klass.summarize(ip1,ip2,ip3,ip4).map{|i| i.to_string}
    assert_equal ["10.0.0.0/22"], @klass.summarize(ip4,ip3,ip2,ip1).map{|i| i.to_string}

    # Summarize non homogeneous networks
    ip1 = @klass.new("10.0.0.0/23")
    ip2 = @klass.new("10.0.2.0/24")
    assert_equal ["10.0.0.0/23","10.0.2.0/24"], @klass.summarize(ip1,ip2).map{|i| i.to_string}

    ip1 = @klass.new("10.0.0.0/16")
    ip2 = @klass.new("10.0.2.0/24")
    assert_equal ["10.0.0.0/16"], @klass.summarize(ip1,ip2).map{|i| i.to_string}

    ip1 = @klass.new("10.0.0.0/23")
    ip2 = @klass.new("10.1.0.0/24")
    assert_equal ["10.0.0.0/23","10.1.0.0/24"], @klass.summarize(ip1,ip2).map{|i| i.to_string}

    ip1 = @klass.new("10.0.0.0/23")
    ip2 = @klass.new("10.0.2.0/23")
    ip3 = @klass.new("10.0.4.0/24")
    ip4 = @klass.new("10.0.6.0/24")
    assert_equal ["10.0.0.0/22","10.0.4.0/24","10.0.6.0/24"], 
              @klass.summarize(ip1,ip2,ip3,ip4).map{|i| i.to_string}

    ip1 = @klass.new("10.0.1.1/24")
    ip2 = @klass.new("10.0.2.1/24")
    ip3 = @klass.new("10.0.3.1/24")
    ip4 = @klass.new("10.0.4.1/24")
    result = ["10.0.1.0/24","10.0.2.0/23","10.0.4.0/24"]
    assert_equal result, @klass.summarize(ip1,ip2,ip3,ip4).map{|i| i.to_string}
    assert_equal result, @klass.summarize(ip4,ip3,ip2,ip1).map{|i| i.to_string}

    ip1 = @klass.new("10.0.1.1/24")
    ip2 = @klass.new("10.10.2.1/24")
    ip3 = @klass.new("172.16.0.1/24")
    ip4 = @klass.new("172.16.1.1/24")
    result = ["10.0.1.0/24","10.10.2.0/24","172.16.0.0/23"]
    assert_equal result, @klass.summarize(ip1,ip2,ip3,ip4).map{|i| i.to_string}

    ips = [@klass.new("10.0.0.12/30"),
           @klass.new("10.0.100.0/24")]
    result = ["10.0.0.12/30", "10.0.100.0/24"]
    assert_equal result, @klass.summarize(*ips).map{|i| i.to_string}

    ips = [@klass.new("172.16.0.0/31"),
           @klass.new("10.10.2.1/32")]
    result = ["10.10.2.1/32", "172.16.0.0/31"]
    assert_equal result, @klass.summarize(*ips).map{|i| i.to_string}    
           
    ips = [@klass.new("172.16.0.0/32"),
           @klass.new("10.10.2.1/32")]
    result = ["10.10.2.1/32", "172.16.0.0/32"]
    assert_equal result, @klass.summarize(*ips).map{|i| i.to_string}    

  end

  def test_classmethod_parse_data
    ip = @klass.parse_data "\254\020\n\001"
    assert_instance_of @klass, ip
    assert_equal "172.16.10.1", ip.address
    assert_equal "172.16.10.1/32", ip.to_string
  end

  def test_classmethod_parse_classful
    @classful.each do |ip,prefix|
      res = @klass.parse_classful(ip)
      assert_equal prefix, res.prefix
      assert_equal "#{ip}/#{prefix}", res.to_string
    end
    assert_raise(ArgumentError){ @klass.parse_classful("192.168.256.257") }
  end
  
end # class IPv4Test

  
 
class IPv6Test < Test::Unit::TestCase
  
  def setup
    @klass = IPAddress::IPv6
    
    @compress_addr = {      
      "2001:db8:0000:0000:0008:0800:200c:417a" => "2001:db8::8:800:200c:417a",
      "2001:db8:0:0:8:800:200c:417a" => "2001:db8::8:800:200c:417a",
      "ff01:0:0:0:0:0:0:101" => "ff01::101",
      "0:0:0:0:0:0:0:1" => "::1",
      "0:0:0:0:0:0:0:0" => "::"}

    @valid_ipv6 = { # Kindly taken from the python IPy library
      "FEDC:BA98:7654:3210:FEDC:BA98:7654:3210" => 338770000845734292534325025077361652240,
      "1080:0000:0000:0000:0008:0800:200C:417A" => 21932261930451111902915077091070067066,
      "1080:0:0:0:8:800:200C:417A" => 21932261930451111902915077091070067066,
      "1080:0::8:800:200C:417A" => 21932261930451111902915077091070067066,
      "1080::8:800:200C:417A" => 21932261930451111902915077091070067066,
      "FF01:0:0:0:0:0:0:43" => 338958331222012082418099330867817087043,
      "FF01:0:0::0:0:43" => 338958331222012082418099330867817087043,
      "FF01::43" => 338958331222012082418099330867817087043,
      "0:0:0:0:0:0:0:1" => 1,
      "0:0:0::0:0:1" => 1,
      "::1" => 1,
      "0:0:0:0:0:0:0:0" => 0,
      "0:0:0::0:0:0" => 0,
      "::" => 0,
      "1080:0:0:0:8:800:200C:417A" => 21932261930451111902915077091070067066,
      "1080::8:800:200C:417A" => 21932261930451111902915077091070067066}
      
    @invalid_ipv6 = [":1:2:3:4:5:6:7",
                     ":1:2:3:4:5:6:7"]
    
    @ip = @klass.new "2001:db8::8:800:200c:417a/64"
    @network = @klass.new "2001:db8:8:800::/64"
    @arr = [8193,3512,0,0,8,2048,8204,16762]
    @hex = "20010db80000000000080800200c417a"
  end
  
  def test_attribute_address
    addr = "2001:0db8:0000:0000:0008:0800:200c:417a"
    assert_equal addr, @ip.address
  end

  def test_initialize
    assert_instance_of @klass, @ip
    @valid_ipv6.keys.each do |ip|
      assert_nothing_raised {@klass.new ip}
    end
    @invalid_ipv6.each do |ip|
      assert_raise(ArgumentError) {@klass.new ip}
    end
    assert_equal 64, @ip.prefix

    assert_raise(ArgumentError) {
      @klass.new "::10.1.1.1"
    }
  end
  
  def test_attribute_groups
    assert_equal @arr, @ip.groups
  end

  def test_method_hexs
    arr = "2001:0db8:0000:0000:0008:0800:200c:417a".split(":")
    assert_equal arr, @ip.hexs
  end
  
  def test_method_to_i
    @valid_ipv6.each do |ip,num|
      assert_equal num, @klass.new(ip).to_i
    end
  end

  def test_method_bits
    bits = "0010000000000001000011011011100000000000000000000" +
      "000000000000000000000000000100000001000000000000010000" + 
      "0000011000100000101111010"
    assert_equal bits, @ip.bits
  end

  def test_method_prefix=()
    ip = @klass.new "2001:db8::8:800:200c:417a"
    assert_equal 128, ip.prefix
    ip.prefix = 64
    assert_equal 64, ip.prefix
    assert_equal "2001:db8::8:800:200c:417a/64", ip.to_string
  end

  def test_method_mapped?
    assert_equal false, @ip.mapped?
    ip6 = @klass.new "::ffff:1234:5678"
    assert_equal true, ip6.mapped?
  end

  def test_method_literal
    str = "2001-0db8-0000-0000-0008-0800-200c-417a.ipv6-literal.net"
    assert_equal str, @ip.literal
  end

  def test_method_group
    @arr.each_with_index do |val,index|
      assert_equal val, @ip[index]
    end
  end

  def test_method_ipv4?
    assert_equal false, @ip.ipv4?
  end
  
  def test_method_ipv6?
    assert_equal true, @ip.ipv6?
  end

  def test_method_network?
    assert_equal true, @network.network?
    assert_equal false, @ip.network?
  end

  def test_method_network_u128
    assert_equal 42540766411282592856903984951653826560, @ip.network_u128
  end
  
  def test_method_include?
    assert_equal true, @ip.include?(@ip)
    # test prefix on same address
    included = @klass.new "2001:db8::8:800:200c:417a/128"
    not_included = @klass.new "2001:db8::8:800:200c:417a/46"
    assert_equal true, @ip.include?(included)
    assert_equal false, @ip.include?(not_included)
    # test address on same prefix 
    included = @klass.new "2001:db8::8:800:200c:0/64"
    not_included = @klass.new "2001:db8:1::8:800:200c:417a/64"
    assert_equal true, @ip.include?(included)
    assert_equal false, @ip.include?(not_included)
    # general test
    included = @klass.new "2001:db8::8:800:200c:1/128"
    not_included = @klass.new "2001:db8:1::8:800:200c:417a/76"
    assert_equal true, @ip.include?(included)
    assert_equal false, @ip.include?(not_included)
  end
  
  def test_method_to_hex
    assert_equal @hex, @ip.to_hex
  end
  
  def test_method_to_s
    assert_equal "2001:db8::8:800:200c:417a", @ip.to_s
  end

  def test_method_to_string
    assert_equal "2001:db8::8:800:200c:417a/64", @ip.to_string
  end

  def test_method_to_string_uncompressed
    str = "2001:0db8:0000:0000:0008:0800:200c:417a/64" 
    assert_equal str, @ip.to_string_uncompressed
  end
  
  def test_method_data
    str = " \001\r\270\000\000\000\000\000\b\b\000 \fAz"
    assert_equal str, @ip.data
  end

  def test_method_reverse
    str = "f.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.2.0.0.0.5.0.5.0.e.f.f.3.ip6.arpa"
    assert_equal str, @klass.new("3ffe:505:2::f").reverse
  end

  def test_method_compressed
    assert_equal "1:1:1::1", @klass.new("1:1:1:0:0:0:0:1").compressed
    assert_equal "1:0:1::1", @klass.new("1:0:1:0:0:0:0:1").compressed
    assert_equal "1:0:0:1::1", @klass.new("1:0:0:1:0:0:0:1").compressed
    assert_equal "1::1:0:0:1", @klass.new("1:0:0:0:1:0:0:1").compressed
    assert_equal "1::1", @klass.new("1:0:0:0:0:0:0:1").compressed
  end
  
  def test_method_unspecified?
    assert_equal true, @klass.new("::").unspecified?
    assert_equal false, @ip.unspecified?    
  end
  
  def test_method_loopback?
    assert_equal true, @klass.new("::1").loopback?
    assert_equal false, @ip.loopback?        
  end
  
  def test_classmethod_expand
    compressed = "2001:db8:0:cd30::"
    expanded = "2001:0db8:0000:cd30:0000:0000:0000:0000"
    assert_equal expanded, @klass.expand(compressed)
    assert_not_equal expanded, @klass.expand("2001:0db8:0:cd3")
    assert_not_equal expanded, @klass.expand("2001:0db8::cd30")
    assert_not_equal expanded, @klass.expand("2001:0db8::cd3")
  end
  
  def test_classmethod_compress
    compressed = "2001:db8:0:cd30::"
    expanded = "2001:0db8:0000:cd30:0000:0000:0000:0000"
    assert_equal compressed, @klass.compress(expanded)
    assert_not_equal compressed, @klass.compress("2001:0db8:0:cd3")
    assert_not_equal compressed, @klass.compress("2001:0db8::cd30")
    assert_not_equal compressed, @klass.compress("2001:0db8::cd3")
  end

  def test_classmethod_parse_data
    str = " \001\r\270\000\000\000\000\000\b\b\000 \fAz"
    ip = @klass.parse_data str
    assert_instance_of @klass, ip
    assert_equal "2001:0db8:0000:0000:0008:0800:200c:417a", ip.address
    assert_equal "2001:db8::8:800:200c:417a/128", ip.to_string
  end

  def test_classhmethod_parse_u128
    @valid_ipv6.each do |ip,num|
      assert_equal @klass.new(ip).to_s, @klass.parse_u128(num).to_s
    end
  end

  def test_classmethod_parse_hex
    assert_equal @ip.to_s, @klass.parse_hex(@hex,64).to_s
  end

end # class IPv6Test

class IPv6UnspecifiedTest < Test::Unit::TestCase
  
  def setup
    @klass = IPAddress::IPv6::Unspecified
    @ip = @klass.new
    @s = "::"
    @str = "::/128"
    @string = "0000:0000:0000:0000:0000:0000:0000:0000/128"
    @u128 = 0
    @address = "::"
  end

  def test_initialize
    assert_nothing_raised {@klass.new}
    assert_instance_of @klass, @ip
  end

  def test_attributes
    assert_equal @address, @ip.compressed
    assert_equal 128, @ip.prefix
    assert_equal true, @ip.unspecified?
    assert_equal @s, @ip.to_s
    assert_equal @str, @ip.to_string
    assert_equal @string, @ip.to_string_uncompressed
    assert_equal @u128, @ip.to_u128
  end

  def test_method_ipv6?
    assert_equal true, @ip.ipv6?
  end
  
end # class IPv6UnspecifiedTest


class IPv6LoopbackTest < Test::Unit::TestCase
  
  def setup
    @klass = IPAddress::IPv6::Loopback
    @ip = @klass.new
    @s = "::1"
    @str = "::1/128"
    @string = "0000:0000:0000:0000:0000:0000:0000:0001/128"
    @u128 = 1
    @address = "::1"
  end

  def test_initialize
    assert_nothing_raised {@klass.new}
    assert_instance_of @klass, @ip
  end

  def test_attributes
    assert_equal @address, @ip.compressed
    assert_equal 128, @ip.prefix
    assert_equal true, @ip.loopback?
    assert_equal @s, @ip.to_s
    assert_equal @str, @ip.to_string
    assert_equal @string, @ip.to_string_uncompressed
    assert_equal @u128, @ip.to_u128
  end

  def test_method_ipv6?
    assert_equal true, @ip.ipv6?
  end
  
end # class IPv6LoopbackTest

class IPv6MappedTest < Test::Unit::TestCase
  
  def setup
    @klass = IPAddress::IPv6::Mapped
    @ip = @klass.new("::172.16.10.1")
    @s = "::ffff:172.16.10.1"
    @str = "::ffff:172.16.10.1/128"
    @string = "0000:0000:0000:0000:0000:ffff:ac10:0a01/128"
    @u128 = 281473568475649
    @address = "::ffff:ac10:a01"

    @valid_mapped = {'::13.1.68.3' => 281470899930115,
      '0:0:0:0:0:ffff:129.144.52.38' => 281472855454758,
      '::ffff:129.144.52.38' => 281472855454758}

    @valid_mapped_ipv6 = {'::0d01:4403' => 281470899930115,
      '0:0:0:0:0:ffff:8190:3426' => 281472855454758,
      '::ffff:8190:3426' => 281472855454758}

    @valid_mapped_ipv6_conversion = {'::0d01:4403' => "13.1.68.3",
      '0:0:0:0:0:ffff:8190:3426' => "129.144.52.38",
      '::ffff:8190:3426' => "129.144.52.38"}

  end

  def test_initialize
    assert_nothing_raised {@klass.new("::172.16.10.1")}
    assert_instance_of @klass, @ip
    @valid_mapped.each do |ip, u128|
      assert_nothing_raised {@klass.new ip}
      assert_equal u128, @klass.new(ip).to_u128
    end
    @valid_mapped_ipv6.each do |ip, u128|
      assert_nothing_raised {@klass.new ip}
      assert_equal u128, @klass.new(ip).to_u128
    end
  end

  def test_mapped_from_ipv6_conversion
    @valid_mapped_ipv6_conversion.each do |ip6,ip4|
      assert_equal ip4, @klass.new(ip6).ipv4.to_s
    end
  end

  def test_attributes
    assert_equal @address, @ip.compressed
    assert_equal 128, @ip.prefix
    assert_equal @s, @ip.to_s
    assert_equal @str, @ip.to_string
    assert_equal @string, @ip.to_string_uncompressed
    assert_equal @u128, @ip.to_u128
  end

  def test_method_ipv6?
    assert_equal true, @ip.ipv6?
  end

  def test_mapped?
    assert_equal true, @ip.mapped?
  end
  
end # class IPv6MappedTest



