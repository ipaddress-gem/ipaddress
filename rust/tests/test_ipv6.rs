mod tests {
struct IPv6Test {
    compress_addr: HashMap<String, String>,
    valid_ipv6: HashMap<String, BigUint>,
    invalid_ipv6: Vec<String>,
    networks: HashMap<String, String>,
    ip: IPAddress,
    network: IPAddress,
    arr: Vec<String>,
    hex: String
}

impl IPv6Test {

  pub fn setup() {
  let mut ip6t = IPv6Test {
     compress_addr: HashMap::new(),
     valid_ipv6: HashMap::new(),
     invalid_ipv6: vec![":1:2:3:4:5:6:7", ":1:2:3:4:5:6:7", "2002:516:2:200", "dd"],
     ip:  IPAddress::parse("2001:db8::8:800:200c:417a/64"),
     network: IPAddress::parse("2001:db8:8:800::/64"),
     arr: vec![8193,3512,0,0,8,2048,8204,16762],
     hex: "20010db80000000000080800200c417a"
  };

  ip6t.compress_addr.insert("2001:db8:0000:0000:0008:0800:200c:417a" , "2001:db8::8:800:200c:417a");
  ip6t.compress_addr.insert("2001:db8:0:0:8:800:200c:417a" , "2001:db8::8:800:200c:417a");
  ip6t.compress_addr.insert("ff01:0:0:0:0:0:0:101" , "ff01::101");
  ip6t.compress_addr.insert("0:0:0:0:0:0:0:1" , "::1");
  ip6t.compress_addr.insert("0:0:0:0:0:0:0:0" , "::");

  ip6t.valid_ipv6.insert("FEDC:BA98:7654:3210:FEDC:BA98:7654:3210" , 338770000845734292534325025077361652240);
  ip6t.valid_ipv6.insert("1080:0000:0000:0000:0008:0800:200C:417A" , 21932261930451111902915077091070067066);
  ip6t.valid_ipv6.insert("1080:0:0:0:8:800:200C:417A" , 21932261930451111902915077091070067066);
  ip6t.valid_ipv6.insert("1080:0::8:800:200C:417A" , 21932261930451111902915077091070067066);
  ip6t.valid_ipv6.insert("1080::8:800:200C:417A" , 21932261930451111902915077091070067066);
  ip6t.valid_ipv6.insert("FF01:0:0:0:0:0:0:43" , 338958331222012082418099330867817087043);
  ip6t.valid_ipv6.insert("FF01:0:0::0:0:43" , 338958331222012082418099330867817087043);
  ip6t.valid_ipv6.insert("FF01::43" , 338958331222012082418099330867817087043);
  ip6t.valid_ipv6.insert("0:0:0:0:0:0:0:1" , 1);
  ip6t.valid_ipv6.insert("0:0:0::0:0:1" , 1);
  ip6t.valid_ipv6.insert("::1" , 1);
  ip6t.valid_ipv6.insert("0:0:0:0:0:0:0:0" , 0);
  ip6t.valid_ipv6.insert("0:0:0::0:0:0" , 0);
  ip6t.valid_ipv6.insert("::" , 0);
  ip6t.valid_ipv6.insert("::/0" , 0);
  ip6t.valid_ipv6.insert("1080:0:0:0:8:800:200C:417A" , 21932261930451111902915077091070067066);
  ip6t.valid_ipv6.insert("1080::8:800:200C:417A" , 21932261930451111902915077091070067066);

  ip6t.networks.insert("2001:db8:1:1:1:1:1:1/32" , "2001:db8::/32");
  ip6t.networks.insert("2001:db8:1:1:1:1:1::/32" , "2001:db8::/32");
  ip6t.networks.insert("2001:db8::1/64" , "2001:db8::/64");

  }

  fn test_attribute_address() {
    let addr = "2001:0db8:0000:0000:0008:0800:200c:417a"
    assert_eq!(addr, setup().ip.address)
  }

  fn test_initialize() {
    assert_eq! setup().ip.is_ipv4

    for ip in setup().valid_ipv6.keys() {
      assert!(IPAddress.parse(ip).is_ok())
    }
    for ip in setup().invalid_ipv6 {
      assert!(IPAddress.parse(ip).is_err())
    }
    assert_eq!(64, setup().ip.prefix.num);

    assert!(IPAddress.parse("::10.1.1.1").is_err())
  }

  fn test_attribute_groups() {
    let setup = setup();
    assert_eq!(setup.arr, seup.ip.ip_parts())
  }

  fn test_method_hexs() {
    let arr = vec!["2001","0db8","0000","0000","0008","0800","200c","417a"];
    assert_eq!(arr, setup().ip.hexs())
  }

  fn test_method_to_i() {
    for (ip,num) in setup().valid_ipv6 {
      assert_eq!(num, IPAddress::parse(ip).address.to_str_radix(10))
  }
}

  fn test_method_bits() {
    let bits = "0010000000000001000011011011100000000000000000000" +
      "000000000000000000000000000100000001000000000000010000" +
      "0000011000100000101111010";
    assert_eq!(bits, ip.address.to_str_radix(2));
}

  fn test_method_set_prefix() {
    let ip = IPAddress::parse( "2001:db8::8:800:200c:417a");
    assert_eq!(128, ip.prefix.num);
    ip.set_prefix(64);
    assert_eq!(64, ip.prefix.num);
    assert_eq!("2001:db8::8:800:200c:417a/64", ip.to_string());
}

  fn test_method_mapped() {
    assert_eq!(false, setup().ip.is_mapped());
    let ip6 = IPAddress::parse("::ffff:1234:5678");
    assert_eq!(true, ip6.is_mapped());
  }

  fn test_method_literal() {
    let str = "2001-0db8-0000-0000-0008-0800-200c-417a.ipv6-literal.net";
    assert_eq!(str, setup().ip.literal());
}

  fn test_method_group() {
    let s = setup();
    for (index = 0; index < s.len(); index++) {
      assert_eq!(val, s.ip.get(index));
    }
}

  fn test_method_ipv4() {
    assert_eq!(false, setup().ip.is_ipv4());
  }

  fn test_method_ipv6() {
    assert_eq!(true, setup().ip.is_ipv6());
  }

  fn test_method_network() {
    assert_eq!(true, setup().network.is_network());
    assert_eq!(false, setup().ip.is_network());
  }

  fn test_method_network_u128() {
    assert_eq!(IPAddress.parse("42540766411282592856903984951653826560", 10), setup().ip.network());
  }

  fn test_method_broadcast_u128() {
    assert_eq!(IPAddress.parse("42540766411282592875350729025363378175", 10), setup().ip.broadcast());
}

  fn test_method_size() {
    let mut ip = IPAddress.parse("2001:db8::8:800:200c:417a/64");
    assert_eq!(BigUint::one()<<64, ip.size());
    ip = IPAddress.parse("2001:db8::8:800:200c:417a/32");
    assert_eq!(BigUint::one()<<96, ip.size());
    ip = IPAddress.parse("2001:db8::8:800:200c:417a/120");
    assert_eq!(BigUint::one()<<8, ip.size());
    ip = IPAddress.parse("2001:db8::8:800:200c:417a/124");
    assert_eq!(BigUint::one()<<4, ip.size());
  }

  fn test_method_includes() {
    let ip = setup().ip;
    assert_eq!(true, ip.include?(ip));
    // test prefix on same address
    let mut included = IPAddress.parse("2001:db8::8:800:200c:417a/128");
    let mut not_included = IPAddress.parse("2001:db8::8:800:200c:417a/46");
    assert_eq!(true, ip.includes(included));
    assert_eq!(false, ip.includes(not_included));
    // test address on same prefix
    included = IPAddress.parse("2001:db8::8:800:200c:0/64");
    not_included = IPAddress.parse("2001:db8:1::8:800:200c:417a/64");
    assert_eq!(true, ip.includes(included));
    assert_eq!(false, ip.includes(not_included));
    // general test
    included = IPAddress.parse("2001:db8::8:800:200c:1/128");
    not_included = IPAddress.parse("2001:db8:1::8:800:200c:417a/76");
    assert_eq!(true, ip.includes(included));
    assert_eq!(false, ip.includes(not_included));
}

  fn test_method_to_hex() {
    assert_eq!(setup().hex, setup().ip.to_hex());
  }

  fn test_method_to_s() {
    assert_eq!("2001:db8::8:800:200c:417a", setup().ip.to_s());
}

  fn test_method_to_string() {
    assert_eq!("2001:db8::8:800:200c:417a/64", setup().ip.to_string());
}

  fn test_method_to_string_uncompressed() {
    let str = "2001:0db8:0000:0000:0008:0800:200c:417a/64";
    assert_eq!(str, setup().ip.to_string_uncompressed());
  }

  fn test_method_reverse() {
    let str = "f.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.2.0.0.0.5.0.5.0.e.f.f.3.ip6.arpa";
    assert_eq!(str, IPAddress.parse("3ffe:505:2::f").dns_reverse());
}

  fn test_method_rev_domains() {
    assert_eq!(["e.ip6.arpa", "f.ip6.arpa"], IPAddress.parse("f000:f100::/3").rev_domains());
    assert_eq!(["2.a.e.f.ip6.arpa", "3.a.e.f.ip6.arpa"], IPAddress.parse("fea3:f120::/15").rev_domains());
    assert_eq!(["f.0.0.0.0.8.f.2.3.0.a.3.ip6.arpa"], IPAddress.parse("3a03:2f80:f::/48").rev_domains());

    assert_eq!(["0.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                  "1.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                  "2.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                  "3.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                  "4.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                  "5.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                  "6.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa",
                  "7.3.2.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1.f.0.0.0.f.ip6.arpa"],
                  IPAddress.parse("f000:f100::1234/125").rev_domains());
  }

  fn test_method_compressed() {
    assert_eq!("1:1:1::1", IPAddress.parse("1:1:1:0:0:0:0:1").unwrap().compressed());
    assert_eq!("1:0:1::1", IPAddress.parse("1:0:1:0:0:0:0:1").unwrap().compressed());
    assert_eq!("1::1:1:1:2:3:1", IPAddress.parse("1:0:1:1:1:2:3:1").unwrap().compressed());
    assert_eq!("1::1:1:0:2:3:1", IPAddress.parse("1:0:1:1::2:3:1").unwrap().compressed());
    assert_eq!("1:0:0:1::1", IPAddress.parse("1:0:0:1:0:0:0:1").unwrap().compressed());
    assert_eq!("1::1:0:0:1", IPAddress.parse("1:0:0:0:1:0:0:1").unwrap().compressed());
    assert_eq!("1::1", IPAddress.parse("1:0:0:0:0:0:0:1").unwrap().compressed());
    //assert_eq!("1:1::1:2:0:0:1", IPAddress.parse("1:1:0:1:2::1").unwrap().compressed
   }

  fn test_method_unspecified() {
    assert_eq!(true, IPAddress.parse("::").unwrap().is_unspecified());
    assert_eq!(false, setup().ip.is_unspecified());
  }

  fn test_method_loopback() {
    assert_eq!(true, IPAddress.parse("::1").unwrap().is_loopback());
    assert_eq!(false, setup().ip.is_loopback());
  }

  fn test_method_network() {
    for (addr,net) in setup().networks {
      let ip = IPAddress.parse(addr).unwrap();
      assert_eq!(net, ip.network.to_string());
    }
}

  fn test_method_each() {
    let ip = IPAddress.parse("2001:db8::4/125").unwrap();
    let mut arr = vec![]
    ip.each(|i| arr.push(i.compressed()));
    let expected = vec!["2001:db8::","2001:db8::1","2001:db8::2",
                    "2001:db8::3","2001:db8::4","2001:db8::5",
                    "2001:db8::6","2001:db8::7"];
    assert_eq!(expected, arr);
  }

  fn test_method_each_net() {
    let mut test_addrs = vec![];
    for nibble in 0..15 {
      test_addrs.push( (0..7).to_a.map{(0..3).to_a.map{"%x"%nibble}.join("")}.join(":"))
    }
    for prefix in 0..128 {
      let nr_networks = BigUint::one()<<((128-prefix)%4);
      for adr in test_addrs {
        let net_adr = IPAddress.parse("{}/{}", adr, prefix);
        let ret = net_adr.four_bit_networks();
        assert_eq!(ret.first.prefix.to_i()%4, 0);
        assert_eq!(ret.size, nr_networks);
        assert_eq!(net_adr.network(), ret.first.network());
        assert_eq!(net_adr.broadcast(), ret.last.broadcast());
//        puts "//{adr}///{prefix} //{nr_networks} //{ret}"
      }
    }
    assert_eq!(["e000::/4","f000::/4"], IPAddress.parse("fd01:db8::4/3").four_bit_networks.map(|i| i.to_string()));
    assert_eq!(["3a03:2f80:f::/48"], IPAddress.parse("3a03:2f80:f::/48").four_bit_networks.map(|i| i.to_string()));
  }

  fn test_method_compare() {
    let ip1 = IPAddress.parse("2001:db8:1::1/64");
    let ip2 = IPAddress.parse("2001:db8:2::1/64");
    let ip3 = IPAddress.parse("2001:db8:1::2/64");
    let ip4 = IPAddress.parse("2001:db8:1::1/65");

    // ip2 should be greater than ip1
    assert_eq!(true, ip2 > ip1);
    assert_eq!(false, ip1 > ip2);
    assert_eq!(false, ip2 < ip1);
    // ip3 should be less than ip2
    assert_eq!(true, ip2 > ip3);
    assert_eq!(false, ip2 < ip3);
    // ip1 should be less than ip3
    assert_eq!(true, ip1 < ip3);
    assert_eq!(false, ip1 > ip3);
    assert_eq!(false, ip3 < ip1);
    // ip1 should be equal to itself
    assert_eq!(true, ip1 == ip1);
    // ip4 should be greater than ip1
    assert_eq!(true, ip1 < ip4);
    assert_eq!(false, ip1 > ip4);
    // test sorting
    arr = ["2001:db8:1::1/64","2001:db8:1::1/65",
           "2001:db8:1::2/64","2001:db8:2::1/64"]
    assert_eq!(arr, [ip1,ip2,ip3,ip4].sort.map(|s| s.to_string()));
  }

  fn test_classmethod_expand() {
    let compressed = "2001:db8:0:cd30::";
    let expanded = "2001:0db8:0000:cd30:0000:0000:0000:0000";
    assert_eq!(expanded, @klass.expand(compressed));
    assert_not_equal expanded, @klass.expand("2001:0db8:0::cd3")
    assert_not_equal expanded, @klass.expand("2001:0db8::cd30")
    assert_not_equal expanded, @klass.expand("2001:0db8::cd3")
  }

  fn test_classmethod_compress() {
    let compressed = "2001:db8:0:cd30::";
    let expanded = "2001:0db8:0000:cd30:0000:0000:0000:0000";
    assert_eq!(compressed, IPAddress.parse(expanded).to_s());
    assert_eq!("2001:0db8::cd3", IPAddress.parse("2001:0db8:0::cd3").to_s());
    assert_eq!("2001:0db8::cd30", IPAddress.parse("2001:0db8::cd30").to_s());
    assert_eq!("2001:0db8::cd3", IPAddress.parse("2001:0db8::cd3").to_s());
  }

  fn test_classhmethod_parse_u128() {
    for (ip,num) in setup().valid_ipv6 {
      assert_eq!(IPAddress.parse(ip).to_s(), IPv6.parse_from_int(num).to_s());
    }
  }

  fn test_classmethod_parse_hex() {
    assert_eq!(setup().ip.to_string(), IPv6.parse_hex(setup().hex, 64).to_string());
  }

} // class IPv6Test
}
