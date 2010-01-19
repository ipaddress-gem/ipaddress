require 'test_helper'
 
class IPv6Test < Test::Unit::TestCase
  
  def setup
    @klass = IPAddress::IPv6
    
    @compress_addr = {      
      "2001:db8:0000:0000:0008:0800:200c:417a" => "2001:db8::8:800:200c:417a",
      "2001:db8:0:0:8:800:200c:417a" => "2001:db8::8:800:200c:417a",
      "ff01:0:0:0:0:0:0:101" => "ff01::101",
      "0:0:0:0:0:0:0:1" => "::1",
      "0:0:0:0:0:0:0:0" => "::"}
    
    @ip = @klass.new "2001:db8::8:800:200c:417a/64"
  end
  
  
  def test_attribute_address
    addr = "2001:0db8:0000:0000:0008:0800:200c:417a"
    assert_equal addr, @ip.address
  end

  def test_initialize
    assert_equal 128, @klass.new("::").prefix
    assert_equal false, "write the initialize tests!!"
  end
  
  def test_attribute_groups
    arr = [8193,3512,0,0,8,2048,8204,16762]
    assert_equal arr, @ip.groups
  end

  def test_method_hexs
    arr = "2001:0db8:0000:0000:0008:0800:200c:417a".split(":")
    assert_equal arr, @ip.hexs
  end
  
  def test_method_to_i
    bigint = 42540766411282592856906245548098208122 
    assert_equal bigint, @ip.to_i
  end

  def test_method_to_hex
    hex = "20010db80000000000080800200c417a"
    assert_equal hex, @ip.to_hex
  end
  
  def test_method_to_s
    assert_equal "2001:db8::8:800:200c:417a/64", @ip.to_s
  end

  def test_method_to_string
    str = "2001:0db8:0000:0000:0008:0800:200c:417a/64" 
    assert_equal str, @ip.to_string
  end
  
  def test_method_data
    str = " \001\r\270\000\000\000\000\000\b\b\000 \fAz"
    assert_equal str, @ip.data
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

#   def test_classmethod_create_unpecified
#     unspec = @klass.create_unspecified
#     assert_equal "::", unspec.address
#     assert_equal 128, unspec.prefix
#     assert_equal true, unspec.unspecified?
#     assert_instance_of @klass, unspec.class
#   end
  
#   def test_classmethod_create_loopback
#     loopb = @klass.create_loopback
#     assert_equal "::1", loopb.address
#     assert_equal 128, loopb.prefix
#     assert_equal true, loopb.loopback?
#     assert_instance_of @klass, loopb.class
#   end

  def test_classmethod_parse_data
    str = " \001\r\270\000\000\000\000\000\b\b\000 \fAz"
    ip = @klass.parse_data str
    assert_instance_of @klass, ip
    assert_equal "2001:0db8:0000:0000:0008:0800:200c:417a", ip.address
    assert_equal "2001:db8::8:800:200c:417a/128", ip.to_s
  end
  
end # class IPv4Test

  
