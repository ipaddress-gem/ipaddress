module IPAddress

  describe IPv4 do

    subject (:ip) { IPAddress::IPv4.new("#{addr}/#{pref}") }

    let(:addr) { "10.0.0.1" }
    let(:pref) { 8 }

    it "does not raise any error" do
      expect {ip}.not_to raise_error
    end

    it "is an instance of IPAddress::IPv4" do
      expect(ip).to be_instance_of(IPAddress::IPv4)
    end

    describe "#prefix" do
      it "is an instance of IPAddress::Prefix32" do
        expect(ip.prefix).to be_instance_of(IPAddress::Prefix32)
      end
    end

    context "when the prefix is not specified" do
      let(:pref) { nil }
      it "creates a new IPv4 address with default prefix /32" do
        expect(ip.prefix).to eq 32
      end
    end

    describe "creation of an invalid address" do

      shared_examples "invalid address" do
        it "is invalid and will raise error" do
          expect {ip}.to raise_error
        end
      end

      context "when the address is out of range" do
        let(:addr) { "10.0.0.256" }
        it_behaves_like "invalid address"
      end

      context "when the address has only 2 octets" do
        let(:addr) { "10.0" }
        it_behaves_like "invalid address"
      end

      context "when the address has only 3 octets" do
        let(:addr) { "10.0.0" }
        it_behaves_like "invalid address"
      end

      context "when the address has more than 4 octets" do
        let(:addr) { "10.0.0.0.2" }
        it_behaves_like "invalid address"
      end

      context "when the prefix is not a Fixnum" do
        let(:pref) { "ars" }
        it_behaves_like "invalid address"
      end

    end

    describe "#octets" do
      it "returns an array with the octets" do
        expect(ip.octets).to eq [10,0,0,1]
      end
    end

    describe "#data" do
      it "returns the ip in network byte order format" do
        expect(ip.data).to eq "\n\x00\x00\x01"
      end
    end

    describe "#to_string" do
      it "returns the IP address in canonical form" do
        expect(ip.to_string).to eq "10.0.0.1/8"
      end
    end

    describe "#to_s" do
      it "returns the address portion of the IPv4 object" do
        expect(ip.to_s).to eq "10.0.0.1"
      end
    end

    describe "#netmask" do
      it "returns the prefix as a string in IP format" do
        expect(ip.netmask).to eq "255.0.0.0"
      end
    end

    describe "#to_u32" do
      it "returns the address portion as unsigned 32 bits integer" do
        expect(ip.to_u32).to eq 167772161
      end
    end


  end # describe IPv4

end # module IPAddress
