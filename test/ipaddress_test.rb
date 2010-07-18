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


