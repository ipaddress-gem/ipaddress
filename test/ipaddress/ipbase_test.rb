require 'test_helper'

class IpaddressTest < Test::Unit::TestCase

  must "be valid ip" do
    assert_equal true, IPAddress::valid?("10.0.0.1")
    assert_equal true, IPAddress::valid?("10.0.0.0")
    assert_equal true, IPAddress::valid?("2002::1")
    assert_equal true, IPAddress::valid?("dead:beef:cafe:babe::f0ad")
  end

  must "be valid netmask" do
    assert_equal true, IPAddress::valid_ipv4_netmask?("255.255.255.0")
  end

  must "be invalid netmask" do
    assert_equal false, IPAddress::valid_ipv4_netmask?("10.0.0.1")
  end
 
  must "be invalid" do
    assert_equal false, IPAddress::valid?("10.0.0.256")
    assert_equal false, IPAddress::valid?("10.0.0.0.0")
    assert_equal false, IPAddress::valid?("10.0.0")
    assert_equal false, IPAddress::valid?("10.0")
    assert_equal false, IPAddress::valid?("2002:::1")
  end

end
