module IPAddress

  describe Prefix32 do

    prefix_hash = {
      "0.0.0.0"         => 0,
      "255.0.0.0"       => 8,
      "255.255.0.0"     => 16,
      "255.255.255.0"   => 24,
      "255.255.255.252" => 30}

    let (:klass) { IPAddress::Prefix32 }

    it "creates a new prefix from a Fixnum" do
      prefix = IPAddress::Prefix32.new(8)
      expect(prefix.prefix).to be(8)
    end

    it "creates a new prefix from a netmask" do
      prefix_hash.each do |netmask, num|
        pref = IPAddress::Prefix32.parse_netmask(netmask)
        expect(pref.prefix).to be(num)
        expect(pref).to be_an_instance_of(IPAddress::Prefix32)
      end
    end

    it "accepts only prefixes between 0 and 32" do
      expect { klass.new(0) }.not_to raise_error
      expect { klass.new(32) }.not_to raise_error
      expect { klass.new(-2) }.to raise_error(ArgumentError)
      expect { klass.new(33) }.to raise_error(ArgumentError)
    end

    describe "#prefix" do
      prefix = IPAddress::Prefix32.new(8)
      it "is an instance of Fixnum" do
        expect(prefix.prefix).to be_an_instance_of(Fixnum)
      end
    end

    describe "#to_ip" do
      prefix_hash = {
        "0.0.0.0"         => 0,
        "255.0.0.0"       => 8,
        "255.255.0.0"     => 16,
        "255.255.255.0"   => 24,
        "255.255.255.252" => 30}

      prefix = IPAddress::Prefix32.new(8)
      it "returns a prefix in dotted decimal (ip) form" do
        prefix_hash.each do |netmask, num|
          prefix = IPAddress::Prefix32.new(num)
          expect(prefix.to_ip).to eq netmask
        end
      end
    end

    describe "#to_s" do
      it "returns a string with the prefix" do
        prefix = IPAddress::Prefix32.new(8)
        expect(prefix.to_s).to eq "8"
      end
    end

    describe "#bits" do
      it "returns a strings with the prefix in base2 format" do
        prefix = IPAddress::Prefix32.new(16)
        expect(prefix.bits).to eq ("1"*16+"0"*16)
      end
    end

    describe "#u32" do
      u32_hash = {
        0  => 0,
        8  => 4278190080,
        16 => 4294901760,
        24 => 4294967040,
        30 => 4294967292}

      it "returns the prefix as ip address expressed in u32 format" do
        u32_hash.each do |num,u32|
          prefix = IPAddress::Prefix32.new(num)
          expect(prefix.to_u32).to eq u32
        end
      end
    end

    describe "#+" do
      p1 = IPAddress::Prefix32.new(8)
      p2 = IPAddress::Prefix32.new(10)
      it "sums two prefixes according to thier Fixnum value" do
        expect(p1+p2).to eq 18
      end
      it "sums a prefix with a fixnum value" do
        expect(p1+4).to eq 12
      end
    end

    describe "#-" do
      p1 = IPAddress::Prefix32.new(8)
      p2 = IPAddress::Prefix32.new(24)
      it "returns the absolute difference between two prefix" do
        expect(p1-p2).to eq 16
        expect(p2-p1).to eq 16
      end
      it "returns the absolute difference between a prefix and a fixnum" do
        expect(p2-4).to eq 20
      end
    end

    describe "#octets" do
      octets_hash = {
        [0,0,0,0]         => 0,
        [255,0,0,0]       => 8,
        [255,255,0,0]     => 16,
        [255,255,255,0]   => 24,
        [255,255,255,252] => 30}
      it "returns the prefix in ip form as an array of octets" do
        octets_hash.each do |arr, pref|
          prefix = IPAddress::Prefix32.new(pref)
          expect(prefix.octets).to eq arr
        end
      end
    end

    describe "#[]" do
      octets_hash = {
        [0,0,0,0]         => 0,
        [255,0,0,0]       => 8,
        [255,255,0,0]     => 16,
        [255,255,255,0]   => 24,
        [255,255,255,252] => 30}
      it "returns the given octet of the prefix in ip format" do
        octets_hash.each do |arr, pref|
          prefix = IPAddress::Prefix32.new(pref)
          arr.each_with_index do |oct, index|
            expect(prefix[index]).to eq oct
          end
        end
      end
    end

    describe "#hostmask" do
      it "returns the prefix as hostmask" do
        prefix = IPAddress::Prefix32.new(8)
        expect(prefix.hostmask).to eq "0.255.255.255"
      end
    end

  end
  
  describe Prefix128 do

    let (:klass) { IPAddress::Prefix128 }

    it "creates a new prefix from a Fixnum" do
      prefix = klass.new(128)
      expect(prefix.prefix).to eq 128
      expect(prefix.prefix).to be_an_instance_of(Fixnum)
      expect(prefix).to be_an_instance_of(klass)
    end

    it "accepts only prefixes between 0 and 128" do
      pending "issue #38"
      expect { klass.new(0) }.not_to raise_error
      expect { klass.new(128) }.not_to raise_error
      expect { klass.new(-2) }.to raise_error(ArgumentError)
      expect { klass.new(129) }.to raise_error(ArgumentError)
    end

    describe "#bits" do
      it "returns a strings with the prefix in base2 format" do
        prefix = IPAddress::Prefix128.new(64)
        expect(prefix.bits).to eq ("1"*64+"0"*64)
      end
    end

    describe "#to_u128" do
      u128_hash = {
        32  => 340282366841710300949110269838224261120,
        64 => 340282366920938463444927863358058659840,
        96 => 340282366920938463463374607427473244160,
        126 => 340282366920938463463374607431768211452}
      it "returns the prefix in u128 form" do
        u128_hash.each do |num, u128|
          prefix = IPAddress::Prefix128.new(num)
          expect(prefix.to_u128).to eq u128
        end
      end
    end
  end  

end
