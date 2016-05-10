
struct Prefix128Test {
    u128_hash: HashMap<u8, XXX>
}

impl Prefix128Test {
  pub fn setup() {
    u128_hash = HashMap::New()
      .insert(32,  340282366841710300949110269838224261120)
      .insert(64,  340282366920938463444927863358058659840)
      .insert(96,  340282366920938463463374607427473244160)
      .insert(126, 340282366920938463463374607431768211452)
}

  pub fn test_initialize() {
    assert_raise (ArgumentError) do
      @klass.new 129
    end
    assert_nothing_raised do
      @klass.new 64
    end
    assert_instance_of @klass, @klass.new(64)
}

  pub fn test_method_bits() {
    prefix = Prefix128::new(64)
    str = "1"*64 + "0"*64
    assert_equal str, prefix.bits
}

  pub fn test_method_to_u32() {
    @u128_hash.each do |num,u128|
      assert_equal u128, @klass.new(num).to_u128
    end
}

}
