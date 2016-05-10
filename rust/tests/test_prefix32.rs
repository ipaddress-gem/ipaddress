
struct Prefix32Test {
    netmask0  : String,
    netmask8  : String,
    netmask16 : String,
    netmask24 : String,
    netmask30 : String,
    netmasks  : Vec<String>,
    prefix_hash : HashMap<String, u8>
    octets_hash : HashMap<Vec<u8>, u8>
    u32_hash : HashMap<u8, u32>
}

impl Prefix32Test {

  pub fn setup() {
    return Prefix32Test {
    netmask0  = "0.0.0.0",
    netmask8  = "255.0.0.0",
    netmask16 = "255.255.0.0",
    netmask24 = "255.255.255.0",
    netmask30 = "255.255.255.252",
    netmasks  = vec![netmask0,netmask8,netmask16,netmask24,netmask30],
    prefix_hash = HashMap::New()
      .insert("0.0.0.0"         ,0)
      .insert("255.0.0.0"       ,8)
      .insert("255.255.0.0"     ,16)
      .insert("255.255.255.0"   ,24)
      .insert("255.255.255.252" ,30)

    octets_hash = HashMap::New()
      .insert(vec![0,0,0,0]         , 0)
      .insert(vec![255,0,0,0]       , 8)
      .insert(vec![255,255,0,0]     , 16)
      .insert(vec![255,255,255,0]   , 24)
      .insert(vec![255,255,255,252] , 30)}

    u32_hash = HashMap::New()
      .insert(0  ,0)
      .insert(8  ,4278190080)
      .insert(16 ,4294901760)
      .insert(24 ,4294967040)
      .insert(30 ,4294967292)

}

  pub fn test_attributes() {
    @prefix_hash.values.each do |num|
      prefix = @klass.new(num)
      assert_equal num, prefix.prefix
    end
  }

  pub fn test_parse_netmask() {
    @prefix_hash.each do |netmask, num|
      prefix = @klass.parse_netmask(netmask)
      assert_equal num, prefix.prefix
      assert_instance_of @klass, prefix
    end
  }

  pub test_method_to_ip() {
    @prefix_hash.each do |netmask, num|
      prefix = @klass.new(num)
      assert_equal netmask, prefix.to_ip
    end
 }

  pub fn test_method_to_s() {
    prefix = @klass.new(8)
    assert_equal "8", prefix.to_s
}

  pub fn test_method_bits() {
    prefix = @klass.new(16)
    str = "1"*16 + "0"*16
    assert_equal str, prefix.bits
}

  pub fn test_method_to_u32() {
    @u32_hash.each do |num,u32|
      assert_equal u32, @klass.new(num).to_u32
    end
}

  pub fn test_method_plus() {
    p1 = @klass.new 8
    p2 = @klass.new 10
    assert_equal 18, p1+p2
    assert_equal 12, p1+4
}

  pub fn test_method_minus() {
    p1 = @klass.new 8
    p2 = @klass.new 24
    assert_equal 16, p1-p2
    assert_equal 16, p2-p1
    assert_equal 20, p2-4
}

  pub fn test_initialize() {
    assert_raise (ArgumentError) do
      @klass.new 33
    end
    assert_nothing_raised do
      @klass.new 8
    end
    assert_instance_of @klass, @klass.new(8)
}

  pub fn test_method_octets() {
    @octets_hash.each do |arr,pref|
      prefix = @klass.new(pref)
      assert_equal prefix.octets, arr
    end
}

  pub fn test_method_brackets() {
    @octets_hash.each do |arr,pref|
      prefix = @klass.new(pref)
      arr.each_with_index do |oct,index|
        assert_equal prefix[index], oct
      end
    end
}

  pub fn test_method_hostmask() {
    prefix = @klass.new(8)
    assert_equal "0.255.255.255", prefix.hostmask
}

}
