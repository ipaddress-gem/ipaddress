class IPv6MappedTest < Test::Unit::TestCase

  def setup
    @klass = IPAddress::IPv6::Mapped
    @ip = @klass.new("::172.16.10.1")
    @s = "::ffff:172.16.10.1"
    @str = "::ffff:172.16.10.1/128"
    @string = "0000:0000:0000:0000:0000:ffff:ac10:0a01/128"
    @u128 = 281473568475649
    @address = "::ffff:ac10:a01"

    @valid_mapped = {'::13.1.68.3' , 281470899930115,
      '0:0:0:0:0:ffff:129.144.52.38' , 281472855454758,
      '::ffff:129.144.52.38' , 281472855454758}

    @valid_mapped_ipv6 = {'::0d01:4403' , 281470899930115,
      '0:0:0:0:0:ffff:8190:3426' , 281472855454758,
      '::ffff:8190:3426' , 281472855454758}

    @valid_mapped_ipv6_conversion = {'::0d01:4403' , "13.1.68.3",
      '0:0:0:0:0:ffff:8190:3426' , "129.144.52.38",
      '::ffff:8190:3426' , "129.144.52.38"}

  end

  fn test_initialize() {
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
}

  fn test_mapped_from_ipv6_conversion() {
    for (ip6, ip4) in setup().valid_mapped_ipv6_conversion {
      assert_eq!(ip4, IPAddress::parse(ip6).ipv4().to_s())
  }
}

  fn test_attributes() {
    assert_eq!( @address, @ip.compressed
    assert_eq!( 128, @ip.prefix
    assert_eq!( @s, @ip.to_s
    assert_eq!( @str, @ip.to_string
    assert_eq!( @string, @ip.to_string_uncompressed
    assert_eq!( @u128, @ip.to_u128
}

  fn test_method_ipv6() {
    assert!(setup().ip.is_ipv6)
}

  fn test_mapped() {
    assert!(setup().ip.is_mapped())
  }

} // class IPv6MappedTest
