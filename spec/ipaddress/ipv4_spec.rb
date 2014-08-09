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

    describe "#network?" do
      it "checks if the IP address is actually a network" do
        expect(ip.network?).to be_falsey
        expect(IPAddress::IPv4.new("10.0.0.0/24").network?).to be_truthy
      end
    end

    describe "#broadcast" do
      it "returns the broadcast address for the given IP" do
        expect(ip.broadcast).to be_instance_of(IPAddress::IPv4)
        bcast = ip.broadcast.to_s
        expect(bcast).to eq "10.255.255.255"
        bcast = ip.broadcast.to_string
        expect(bcast).to eq "10.255.255.255/8"
      end
    end

    describe "#network" do
      it "returns the network number" do
        expect(ip.network.to_s).to eq "10.0.0.0"
        expect(ip.network).to be_instance_of(IPAddress::IPv4)
      end
    end

    describe "#bits" do
      it "returns the IP in binary format" do
        expect(ip.bits).to eq "00001010000000000000000000000001"
      end
    end

    describe "#first" do
      context "when the ip is a network" do
        let(:ip) { IPAddress::IPv4.new("192.168.100.0/24") }
        it "returns the first host IP address in the range" do
          expect(ip).to be_instance_of(IPAddress::IPv4)
          expect(ip.first.to_s).to eq "192.168.100.1"
        end
      end
      context "when the ip is a simple ip" do
        let(:ip) { IPAddress::IPv4.new("192.168.100.50/24") }
        it "returns the first host IP address in the range" do
          expect(ip).to be_instance_of(IPAddress::IPv4)
          expect(ip.first.to_s).to eq "192.168.100.1"
        end
      end
    end

    describe "#last" do
      context "when the ip is a network" do
        let(:ip) { IPAddress::IPv4.new("192.168.100.0/24") }
        it "returns the last host IP address in the range" do
          expect(ip).to be_instance_of(IPAddress::IPv4)
          expect(ip.last.to_s).to eq "192.168.100.254"
        end
      end
      context "when the ip is a simple ip" do
        let(:ip) { IPAddress::IPv4.new("192.168.100.50/24") }
        it "returns the last host IP address in the range" do
          expect(ip).to be_instance_of(IPAddress::IPv4)
          expect(ip.last.to_s).to eq "192.168.100.254"
        end
      end
    end

    describe "#each_host" do
      let(:pref) { 29 }
      it "iterates over all the hosts IP addresses" do
        arr = []
        ip.each_host {|i| arr << i.to_s}
        expected = ["10.0.0.1","10.0.0.2","10.0.0.3",
                    "10.0.0.4","10.0.0.5","10.0.0.6"]
        expect(arr).to eq expected
      end
    end

    describe "#each" do
      let(:pref) { 29 }
      it "iterates over all the IP addresses" do
        arr = []
        ip.each {|i| arr << i.to_s}
        expected = ["10.0.0.0","10.0.0.1","10.0.0.2",
                    "10.0.0.3","10.0.0.4","10.0.0.5",
                    "10.0.0.6","10.0.0.7"]
        expect(arr).to eq expected
      end
    end

    describe "#size" do
      it "returns the number of IP addresses included in the network" do
        expect(ip.size).to eq 16777216
      end
    end

    describe "#hosts" do
      let(:pref) { 29 }
      it "returns all the hosts in the network" do
        expected = ["10.0.0.1","10.0.0.2","10.0.0.3",
                    "10.0.0.4","10.0.0.5","10.0.0.6"]
        expect(ip.hosts.map{|i| i.to_s}).to eq expected
      end
    end

    describe "#network_u32" do
      it "returns the network number in Unsigned 32bits format" do
        expect(ip.network_u32).to eq 167772160
      end
    end

    describe "#broadcast_u32" do
      it "returns the broadcast in Unsigned 32bits format" do
        expect(ip.broadcast_u32).to eq 184549375
      end
    end

    describe "#include" do
      pending
    end

    describe "#ipv4?" do
      it "checks if the object is an IPv4 address" do
        expect(ip).to be_ipv4
      end
    end

    describe "#ipv6?" do
      it "checks if the object is an IPv6 address" do
        expect(ip).not_to be_ipv6
      end
    end

    describe "#private" do
      shared_examples "private address" do |address|
        let(:ip) { IPAddress::IPv4.new(address) }
        it "is private" do
          expect(ip).to be_private  
        end
      end
      context "when the address is a private 192.168/16" do
        it_behaves_like "private address", "192.168.10.50/24"
        it_behaves_like "private address", "192.168.10.50/16"
      end
      context "when the address is a private 172.16/12" do
        it_behaves_like "private address", "172.16.10.50/24"
        it_behaves_like "private address", "172.16.10.50/14"
      end
      context "when the address is a private 10/8" do
        it_behaves_like "private address", "10.10.10.50/10"
        it_behaves_like "private address", "10.0.0.0/8"
      end
      context "when the address is not private" do
        let(:addr) { "3.0.0.0" }
        it "should not be private" do
          expect(ip).not_to be_private 
        end
      end
    end

    context "classes (A, B, C)" do
      let(:class_a) { IPAddress::IPv4.new("10.0.0.1/8") }
      let(:class_b) { IPAddress::IPv4.new("172.16.0.1/16") }
      let(:class_c) { IPAddress::IPv4.new("192.168.0.1/24") }

      describe "#a" do
        it "checks if it's a RFC 791 CLASS A address" do
          expect(class_a.a?).to be_truthy
          expect(class_b.a?).to be_falsey
          expect(class_c.a?).to be_falsey
        end
      end
      describe "#b" do
        it "checks if it's a RFC 791 CLASS B address" do
          expect(class_a).not_to be_b
          expect(class_b).to be_b
          expect(class_c).not_to be_b
        end
      end
      describe "#c" do
        it "checks if it's a RFC 791 CLASS C address" do
          expect(class_a).not_to be_c
          expect(class_b).not_to be_c
          expect(class_c).to be_c
        end
      end
    end

    describe "#to_ipv6" do
      it "return the IPv6 Mapped IPv4 address" do
        expect(ip.to_ipv6).to eq "0a00:0001"
      end
    end

    describe "#reverse" do
      it "returns the IP address in in-addr.arpa format" do
        expect(ip.reverse).to eq "1.0.0.10.in-addr.arpa"
      end
    end

    describe "#[]" do
      it "returns the octet specified" do
        expect(ip[0]).to eq 10
        expect(ip[1]).to eq 0
        expect(ip[3]).to eq 1
      end
    end

    describe "#<=>" do
      pending
    end

    describe "#-" do
      let(:ip1) { IPAddress::IPv4.new("10.1.1.1/8") }
      let(:ip2) { IPAddress::IPv4.new("10.1.1.10/8") }
      it "returns the difference between two IP addresses" do
        expect(ip2-ip1).to eq 9
        expect(ip1-ip2).to eq 9
      end
    end

    describe "#+" do
      context "when two contiguous networks" do
        let(:ip1) { IPAddress::IPv4.new("172.16.10.1/24") }
        let(:ip2) { IPAddress::IPv4.new("172.16.11.2/24") }
        it "returns the summarized network" do
          expect((ip1+ip2).map{|i| i.to_string}).to eq ["172.16.10.0/23"]
        end
      end
      context "when one network includes the other" do
        let(:ip1) { IPAddress::IPv4.new("10.0.0.0/16") }
        let(:ip2) { IPAddress::IPv4.new("10.0.2.0/24") }
        it "returns the including network unchanged" do
          expect((ip1+ip2).map{|i| i.to_string}).to eq ["10.0.0.0/16"]
        end
      end
      context "when not contiguous networks" do
        let(:ip1) { IPAddress::IPv4.new("10.0.0.0/23") }
        let(:ip2) { IPAddress::IPv4.new("10.1.0.0/24") }
        it "returns the two networks unchanged" do
          expect((ip1+ip2).map{|i| i.to_string}).to eq ["10.0.0.0/23", "10.1.0.0/24"]
        end
      end
    end

    
    
  end # describe IPv4

end # module IPAddress
