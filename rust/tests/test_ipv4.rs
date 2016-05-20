mod tests {
struct IPv4Prefix {
    ip: String,
    prefix: u8
}

struct IPv4Test {
    valid_ipv4: HashMap<String, IPv4Prefix>,
    invalid_ipv4: Vec<String>,
    valid_ipv4_range: Vec<String>,
    netmask_values: HashMap<String, String>,
    decimal_values: HashMap<String, u32>,
    ip: IPAddress,
    network: IPAddress,
    networks: HashMap<String, String>,
    class_a: IPAddress,
    class_b: IPAddress,
    class_c: IPAddress,
    classful: HashMap<String, u8>,
}

impl IPv4Test

  fn setup() {
      let mut ipv4t = IPv4Test {
        valid_ipv4: HashMap::new()
        invalid_ipv4: vec!["10.0.0.256", "10.0.0.0.0", "10.0.0", "10.0"],
        valid_ipv4_range: vec!["10.0.0.1-254", "10.0.1-254.0", "10.1-254.0.0"],
        netmask_values: HashMap::new()
      };
      ipv4t.valid_ipv4.insert("0.0.0.0/0", IPv4Prefix { ip: "0.0.0.0", prefix: 0 });
      ipv4t.valid_ipv4.insert("10.0.0.0", IPv4Prefix { ip: "10.0.0.0", prefix: 32});
      ipv4t.valid_ipv4.insert("10.0.0.1", IPv4Prefix { ip: "10.0.0.1", prefix: 32});
      ipv4t.valid_ipv4.insert("10.0.0.1/24", IPv4Prefix { ip: "10.0.0.1", prefix: 24});
      ipv4t.valid_ipv4.insert("10.0.0.1/255.255.255.0", IPv4Prefix { ip: "10.0.0.1", prefix: 24});

      ipv4t.netmask_values.insert("0.0.0.0/0"        , "0.0.0.0");
      ipv4t.netmask_values.insert("10.0.0.0/8"       , "255.0.0.0");
      ipv4t.netmask_values.insert("172.16.0.0/16"    , "255.255.0.0");
      ipv4t.netmask_values.insert("192.168.0.0/24"   , "255.255.255.0");
      ipv4t.netmask_values.insert("192.168.100.4/30" , "255.255.255.252");

      ipv4t.decimal_values.insert("0.0.0.0/0"        , 0);
      ipv4t.decimal_values.insert("10.0.0.0/8"       , 167772160);
      ipv4t.decimal_values.insert("172.16.0.0/16"    , 2886729728);
      ipv4t.decimal_values.insert("192.168.0.0/24"   , 3232235520);
      ipv4t.decimal_values.insert("192.168.100.4/30" , 3232261124);

      ipv4t.ip = IPAddress.parse("172.16.10.1/24");
      ipv4t.network = IPAddress.parse("172.16.10.0/24");

      ipv4t.broadcast.insert("10.0.0.0/8"       , "10.255.255.255/8");
      ipv4t.broadcast.insert("172.16.0.0/16"    , "172.16.255.255/16");
      ipv4t.broadcast.insert("192.168.0.0/24"   , "192.168.0.255/24");
      ipv4t.broadcast.insert("192.168.100.4/30" , "192.168.100.7/30");

      ipv4t.networks.insert("10.5.4.3/8"       , "10.0.0.0/8");
      ipv4t.networks.insert("172.16.5.4/16"    , "172.16.0.0/16");
      ipv4t.networks.insert("192.168.4.3/24"   , "192.168.4.0/24");
      ipv4t.networks.insert("192.168.100.5/30" , "192.168.100.4/30");

    ipv4t.class_a = IPAddress.parse("10.0.0.1/8");
    ipv4t.class_b = IPAddress.parse("172.16.0.1/16");
    ipv4t.class_c = IPAddress.parse("192.168.0.1/24");

      ipv4t.classful.insert("10.1.1.1"  , 8);
      ipv4t.classful.insert("150.1.1.1" , 16);
      ipv4t.classful.insert("200.1.1.1" , 24);

  }

  pub fn test_initialize() {
    let setup = setup()
    for i in setup.valid_ipv4 {
      let ip = IPAddress::parse(i);
      assert!(ip.ipv4 && !ip.is_ipv6);
    }
    assert_eq!(32, setup.prefix.ip_bits);
    assert!(IPAddress.parse("1.f.13.1/-3").is_err());
    assert!(IPAddress.parse("10.0.0.0/8").is_ok());
  }

  pub fn test_initialize_format_error() {
    for i in setup().invalid_ipv4 {
        assert!(IPAddress.parse(i).is_err());
    }
    assert!(IPAddress.parse("10.0.0.0/asd").is_err());
  }

  pub fn test_initialize_without_prefix() {
    assert!(IPAddress.parse("10.10.0.0").is_ok());
    let ip = IPAddress.parse("10.10.0.0");
    assert!(!ip.is_ipv6 && ip.is_ipv4);
    assert_eq!(32, ip.prefix.num);
}

  pub fn test_attributes() {
    for (arg, attr) in setup().valid_ipv4 {
      let ip = IPAddress::parse(arg);
      assert_eq!(attr.first, ip.address)
      assert_eq!(attr.last, ip.prefix.to_i)
  }
}

  pub fn test_octets() {
    let ip = IPAddress::parse("10.1.2.3/8");
    assert_eq!(ip.ip_bits.parts(), vec![10,1,2,3])
}

  pub fn test_method_to_string() {
    for (arg, attr) in setup().valid_ipv4 {
      let ip = IPAddress.parse(arg);
      assert_eq!(format!("{}/{}", attr.ip, attr.prefix), ip.to_string());
    }
  }

  pub fn test_method_to_s() {
    for (arg, attr) in setup().valid_ipv4 {
      let ip = IPAddress.parse(arg);
      assert_eq!(attr.ip, ip.to_s());
      let ip_c = IPAddress.parse(arg);
      assert_eq!(attr.ip, ip.compressed());
  }
  }

  pub fn test_netmask() {
    for (addr, mask) in setup().netmask_values {
      let ip = IPAddress.parse(addr);
      assert_eq!(mask, ip.netmask_str());
    }
  }

  pub fn test_method_to_u32() {
    for (addr, int) in setup().decimal_values {
      let ip = IPAddress.parse(addr);
      assert_eq!(int, ip.address().to_u32());
    }
  }

  pub fn test_method_is_network() {
    assert_eq!(true,  setup().network.is_network());
    assert_eq!(false, setup().ip.is_network());
  }

  pub fn test_one_address_network() {
    let network = IPAddress.parse("172.16.10.1/32");
    assert_eq!(false, network.is_network());
  }

  pub fn test_method_broadcast() {
    for (addr, bcast) in setup().broadcast {
      let ip = IPAddress.parse(addr);
      assert_eq!(bcast, ip.broadcast.to_string());
    }
  }

  pub fn test_method_network() {
    for (addr, net) in setup().networks {
      let ip = IPAddress.parse(addr);
      assert_eq!(net, ip.network.to_string());
    }
  }

  pub fn test_method_bits() {
    let ip = IPAddress.parse("127.0.0.1");
    assert_eq!("01111111000000000000000000000001", ip.bits());
  }

  pub fn test_method_first() {
    let mut ip = IPAddress.parse("192.168.100.0/24");
    assert_eq!("192.168.100.1", ip.first().to_s());
    ip = IPAddress.parse("192.168.100.50/24");
    assert_eq!("192.168.100.1", ip.first().to_s());
  }

  pub fn test_method_last() {
    let mut ip = IPAddress.parse("192.168.100.0/24");
    assert_eq!("192.168.100.254", ip.last().to_s());
    ip = IPAddress.parse("192.168.100.50/24");
    assert_eq!("192.168.100.254", ip.last().to_s());
  }

  pub fn test_method_each_host() {
    let ip = IPAddress.parse("10.0.0.1/29");
    let mut arr : Vec<String> = [];
    ip.each_host(|i| arr.push(i.to_s()));
    let expected = vec!["10.0.0.1","10.0.0.2","10.0.0.3",
                        "10.0.0.4","10.0.0.5","10.0.0.6"];
    assert_eq!(expected, arr);
  }

  pub fn test_method_each() {
    let ip = IPAddress.parse("10.0.0.1/29");
    let mut arr : Vec<String> = [];
    ip.each(|i| arr.push(i.to_s()));
    let expected = vec!["10.0.0.0","10.0.0.1","10.0.0.2",
                    "10.0.0.3","10.0.0.4","10.0.0.5",
                    "10.0.0.6","10.0.0.7"];
    assert_eq!(expected, arr);
  }

  pub fn test_method_size() {
    let ip = IPAddress.parse("10.0.0.1/29");
    assert_eq!(8, ip.size());
}

  pub fn test_method_network_u32() {
    assert_eq!(2886732288, setup().ip.network().to_u32());
  }

  pub fn test_method_broadcast_u32() {
    assert_eq!(2886732543, setup().ip.broadcast().to_u32());
  }

  pub fn test_method_include() {
    let mut ip = IPAddress.parse("192.168.10.100/24");
    let addr = IPAddress.parse("192.168.10.102/24");
    assert_eq!(true, ip.includes(addr));
    assert_eq!(false, ip.include?(IPAddress.parse("172.16.0.48")));
    ip = IPAddress.parse("10.0.0.0/8");
    assert_eq!(true, ip.includes(IPAddress.parse("10.0.0.0/9")));
    assert_eq!(true, ip.includes(IPAddress.parse("10.1.1.1/32")));
    assert_eq!(true, ip.includes(IPAddress.parse("10.1.1.1/9")));
    assert_eq!(false, ip.includes(IPAddress.parse("172.16.0.0/16")));
    assert_eq!(false, ip.includes(IPAddress.parse("10.0.0.0/7")));
    assert_eq!(false, ip.includes(IPAddress.parse("5.5.5.5/32")));
    assert_eq!(false, ip.includes(IPAddress.parse("11.0.0.0/8")));
    ip = IPAddress.parse("13.13.0.0/13");
    assert_eq!(false, ip.includes(IPAddress.parse("13.16.0.0/32")));
  }

  pub fn test_method_include_all() {
    let ip = IPAddress.parse("192.168.10.100/24");
    let addr1 = IPAddress.parse("192.168.10.102/24");
    let addr2 = IPAddress.parse("192.168.10.103/24");
    assert_eq!(true, ip.includes_all([addr1,addr2]));
    assert_eq!(false, ip.includes_all([addr1, IPAddress.parse("13.16.0.0/32")]));
}

  pub fn test_method_ipv4() {
    assert_eq!(true, setup().ip.is_ipv4());
}

  pub fn test_method_ipv6() {
    assert_eq!(false, setup().ip.is_ipv6());
}

  pub fn test_method_private() {
    assert_eq!(true, IPAddress.parse("192.168.10.50/24").is_private());
    assert_eq!(true, IPAddress.parse("192.168.10.50/16").is_private());
    assert_eq!(true, IPAddress.parse("172.16.77.40/24").is_private());
    assert_eq!(true, IPAddress.parse("172.16.10.50/14").is_private());
    assert_eq!(true, IPAddress.parse("10.10.10.10/10").is_private());
    assert_eq!(true, IPAddress.parse("10.0.0.0/8").is_private());
    assert_eq!(false, IPAddress.parse("192.168.10.50/12").is_private());
    assert_eq!(false, IPAddress.parse("3.3.3.3").is_private());
    assert_eq!(false, IPAddress.parse("10.0.0.0/7").is_private());
    assert_eq!(false, IPAddress.parse("172.32.0.0/12").is_private());
    assert_eq!(false, IPAddress.parse("172.16.0.0/11").is_private());
    assert_eq!(false, IPAddress.parse("192.0.0.2/24").is_private());
}

  pub fn test_method_octet() {
    assert_eq!(172, setup().ip.parts.get(0).unwrap());
    assert_eq!(16, setup().ip.parts.get(1).unwrap());
    assert_eq!(10, setup().ip.parts.get(2).unwrap());
    assert_eq!(1, setup().ip.parts.get(3).unwrap());
}

  pub fn test_method_a() {
    assert_eq!(true, setup().class_a.is_class_a());
    assert_eq!(false, setup().class_b.is_class_a());
    assert_eq!(false, setup().class_c.is_class_a());
}

  pub fn test_method_b() {
    assert_eq!(true, setup().class_b.is_class_b());
    assert_eq!(false, setup().class_a.is_class_b());
    assert_eq!(false, setup().class_c.is_class_b());
}

  pub fn test_method_c() {
    assert_eq!(true, setup().class_c.is_class_c());
    assert_eq!(false, setup().class_a.is_class_c());
    assert_eq!(false, setup().class_b.is_class_c());
}

  pub fn test_method_to_ipv6() {
    assert_eq!("ac10:0a01", setup().ip.to_ipv6().to_string());
  }

  pub fn test_method_reverse() {
    assert_eq!("1.10.16.172.in-addr.arpa", setup().ip.reverse());
  }

  pub fn test_method_rev_domains() {
    assert_eq!(["4.17.173.in-addr.arpa", "5.17.173.in-addr.arpa"], IPAddress.parse("173.17.5.1/23").rev_domains());
    assert_eq!(["16.173.in-addr.arpa", "17.173.in-addr.arpa"], IPAddress.parse("173.17.1.1/15").rev_domains());
    assert_eq!(["172.in-addr.arpa", "173.in-addr.arpa"], IPAddress.parse("173.17.1.1/7").rev_domains());
    assert_eq!(["1.17.173.in-addr.arpa"], IPAddress.parse("173.17.1.1/29").rev_domains());
    assert_eq!(["1.17.174.in-addr.arpa"], IPAddress.parse("174.17.1.1/24").rev_domains());
    assert_eq!(["17.175.in-addr.arpa"], IPAddress.parse("175.17.1.1/16").rev_domains());
    assert_eq!(["176.in-addr.arpa"], IPAddress.parse("176.17.1.1/8").rev_domains());
    assert_eq!(["0.in-addr.arpa"], IPAddress.parse("177.17.1.1/0").rev_domains());
    assert_eq!(["1.17.178.in-addr.arpa"], IPAddress.parse("178.17.1.1/32").rev_domains());
  }

  pub fn test_method_compare() {
    let mut ip1 = IPAddress.parse("10.1.1.1/8")
    let mut ip2 = IPAddress.parse("10.1.1.1/16")
    let mut ip3 = IPAddress.parse("172.16.1.1/14")
    let ip4 = IPAddress.parse("10.1.1.1/8")

    // ip2 should be greater than ip1
    assert_eq!(true, ip1 < ip2);
    assert_eq!(false, ip1 > ip2);
    assert_eq!(false, ip2 < ip1);
    // ip2 should be less than ip3
    assert_eq!(true, ip2 < ip3);
    assert_eq!(false, ip2 > ip3);
    // ip1 should be less than ip3
    assert_eq!(true, ip1 < ip3);
    assert_eq!(false, ip1 > ip3);
    assert_eq!(false, ip3 < ip1);
    // ip1 should be equal to itself
    assert_eq!(true, ip1 == ip1);
    // ip1 should be equal to ip4
    assert_eq!(true, ip1 == ip4);
    // test sorting
    let mut arr = vec!["10.1.1.1/8","10.1.1.1/16","172.16.1.1/14"];
    assert_eq!(arr, [ip1,ip2,ip3].sort.map(|s| s.to_string()));
    // test same prefix
    ip1 = IPAddress.parse("10.0.0.0/24");
    ip2 = IPAddress.parse("10.0.0.0/16");
    ip3 = IPAddress.parse("10.0.0.0/8");
    arr = ["10.0.0.0/8","10.0.0.0/16","10.0.0.0/24"];
    assert_eq!(arr, [ip1,ip2,ip3].sort.map(|s| s.to_string()));
  }

  pub fn test_method_minus() {
    let ip1 = IPAddress.parse("10.1.1.1/8");
    let ip2 = IPAddress.parse("10.1.1.10/8");
    assert_eq!(9, ip2 - ip1);
    assert_eq!(9, ip1 - ip2);
  }

  pub fn test_method_plus() {
    let mut ip1 = IPAddress.parse("172.16.10.1/24");
    let mut ip2 = IPAddress.parse("172.16.11.2/24");
    assert_eq!(["172.16.10.0/23"], (ip1+ip2).map(|i| i.to_string()));

    ip2 = IPAddress.parse("172.16.12.2/24");
    assert_eq!([ip1.network.to_string(), ip2.network.to_string()], (ip1+ip2).map(|i| i.to_string()));

    ip1 = IPAddress.parse("10.0.0.0/23");
    ip2 = IPAddress.parse("10.0.2.0/24");
    assert_eq!(["10.0.0.0/23","10.0.2.0/24"], (ip1+ip2).map(|i| i.to_string()));

    ip1 = IPAddress.parse("10.0.0.0/23");
    ip2 = IPAddress.parse("10.0.2.0/24");
    assert_eq!(["10.0.0.0/23","10.0.2.0/24"], (ip2+ip1).map(|i| i.to_string()));

    ip1 = IPAddress.parse("10.0.0.0/16");
    ip2 = IPAddress.parse("10.0.2.0/24");
    assert_eq!(["10.0.0.0/16"], (ip1+ip2).map(|i| i.to_string()));

    ip1 = IPAddress.parse("10.0.0.0/23");
    ip2 = IPAddress.parse("10.1.0.0/24");
    assert_eq!(["10.0.0.0/23","10.1.0.0/24"], (ip1+ip2).map(|i| i.to_string()));
  }

  pub fn test_method_netmask_equal() {
    let ip = IPAddress.parse("10.1.1.1/16");
    assert_eq!(16, ip.prefix.to_i());
    let ip2 = ip.change_netmask("255.255.255.0");
    assert_eq!(24, ip2.prefix.to_i());
  }

  pub fn test_method_split() {
    assert!(setup().ip.split(0).is_err());
    assert!(setup().ip.split(257).is_err());

    assert_eq!(setup().ip.network, setup().ip.split(1).first());

    let mut arr = ["172.16.10.0/27", "172.16.10.32/27", "172.16.10.64/27",
           "172.16.10.96/27", "172.16.10.128/27", "172.16.10.160/27",
           "172.16.10.192/27", "172.16.10.224/27"];
    assert_eq!(arr, setup().network.split(8).unwrap().map(|s| s.to_string()));

    arr = ["172.16.10.0/27", "172.16.10.32/27", "172.16.10.64/27",
           "172.16.10.96/27", "172.16.10.128/27", "172.16.10.160/27",
           "172.16.10.192/26"];
    assert_eq!(arr, setup().network.split(7).unwrap().map(|s| s.to_string()));

    arr = ["172.16.10.0/27", "172.16.10.32/27", "172.16.10.64/27",
           "172.16.10.96/27", "172.16.10.128/26", "172.16.10.192/26"];
    assert_eq!(arr, setup().network.split(6).unwrap().map(|s| s.to_string()));
    arr = ["172.16.10.0/27", "172.16.10.32/27", "172.16.10.64/27",
           "172.16.10.96/27", "172.16.10.128/25"];
    assert_eq!(arr, setup().network.split(5).unwrap().map(|s| s.to_string()));
    arr = ["172.16.10.0/26", "172.16.10.64/26", "172.16.10.128/26",
           "172.16.10.192/26"];
    assert_eq!(arr, setup().network.split(4).unwrap().map(|s| s.to_string()));
    arr = ["172.16.10.0/26", "172.16.10.64/26", "172.16.10.128/25"];
    assert_eq!(arr, setup().network.split(3).unwrap().map(|s| s.to_string()));
    arr = ["172.16.10.0/25", "172.16.10.128/25"];
    assert_eq!(arr, setup().network.split(2).unwrap().map(|s| s.to_string()));
    arr = ["172.16.10.0/24"];
    assert_eq!(arr, setup().network.split(1).unwrap().map(|s| s.to_string()));
  }

  pub fn test_method_subnet() {
    assert!(setup().network.subnet(23).is_err());
    assert!(setup().network.subnet(33).is_err());
    assert!(setup().ip.subnet(30).is_ok());
    let mut arr = ["172.16.10.0/26", "172.16.10.64/26", "172.16.10.128/26",
           "172.16.10.192/26"];
    assert_eq!(arr, setup().network.subnet(26).unwrap().map(|s| s.to_string()));
    arr = ["172.16.10.0/25", "172.16.10.128/25"];
    assert_eq!(arr, setup().network.subnet(25).unwrap().map(|s| s.to_string()));
    arr = ["172.16.10.0/24"];
    assert_eq!(arr, setup().network.subnet(24).unwrap().map(|s| s.to_string()));
  }

  pub fn test_method_supernet() {
    assert!(setup().ip.supernet(24).is_err());
    assert_eq!("0.0.0.0/0", setup().ip.supernet(0).to_string());
    assert_eq!("0.0.0.0/0", setup().ip.supernet(-2).to_string());
    assert_eq!("172.16.10.0/23", setup().ip.supernet(23).to_string());
    assert_eq!("172.16.8.0/22", setup().ip.supernet(22).to_string());
  }

  pub fn test_classmethod_parse_u32() {
    for (addr, int) in setup().decimal_values {
      let ip = IPv4.parse_u32(int, 32);
      let ip2 = ip.change_prefix(addr.split("/").last.to_u8().unwrap());
      assert_eq!(ip2.to_string(), addr);
    }
  }

  // pub fn test_classhmethod_extract() {
  //   let str = "foobar172.16.10.1barbaz";
  //   assert_eq!("172.16.10.1", IPAddress::extract(str).to_s
  // }

  pub fn test_classmethod_summarize() {

    // Should return self if only one network given
    assert_eq!([setup().ip.network()], IPAddress::summarize(setup().ip));

    // Summarize homogeneous networks
    let mut ip1 = IPAddress.parse("172.16.10.1/24");
    let mut ip2 = IPAddress.parse("172.16.11.2/24");
    assert_eq!(["172.16.10.0/23"], IPAddress::summarize(ip1,ip2).map(|i| i.to_string()));

    ip1 = IPAddress.parse("10.0.0.1/24");
    ip2 = IPAddress.parse("10.0.1.1/24");
    let mut ip3 = IPAddress.parse("10.0.2.1/24");
    let mut ip4 = IPAddress.parse("10.0.3.1/24");
    assert_eq!(["10.0.0.0/22"], IPAddress::summarize(ip1,ip2,ip3,ip4).map(|i| i.to_string()));
    assert_eq!(["10.0.0.0/22"], IPAddress::summarize(ip4,ip3,ip2,ip1).map(|i| i.to_string()));

    // Summarize non homogeneous networks
    ip1 = IPAddress.parse("10.0.0.0/23");
    ip2 = IPAddress.parse("10.0.2.0/24");
    assert_eq!(["10.0.0.0/23","10.0.2.0/24"], IPAddress::summarize(ip1,ip2).map(|i| i.to_string()));

    ip1 = IPAddress.parse("10.0.0.0/16");
    ip2 = IPAddress.parse("10.0.2.0/24");
    assert_eq!(["10.0.0.0/16"], IPAddress::summarize(ip1,ip2).map(|i| i.to_string()));

    ip1 = IPAddress.parse("10.0.0.0/23");
    ip2 = IPAddress.parse("10.1.0.0/24");
    assert_eq!(["10.0.0.0/23","10.1.0.0/24"], IPAddress::summarize(ip1,ip2).map(|i| i.to_string()));

    ip1 = IPAddress.parse("10.0.0.0/23");
    ip2 = IPAddress.parse("10.0.2.0/23");
    ip3 = IPAddress.parse("10.0.4.0/24");
    ip4 = IPAddress.parse("10.0.6.0/24");
    assert_eq!(["10.0.0.0/22","10.0.4.0/24","10.0.6.0/24"],
              IPAddress::summarize(ip1,ip2,ip3,ip4).map(|i| i.to_string()));

    ip1 = IPAddress.parse("10.0.1.1/24");
    ip2 = IPAddress.parse("10.0.2.1/24");
    ip3 = IPAddress.parse("10.0.3.1/24");
    ip4 = IPAddress.parse("10.0.4.1/24");
    let mut result = ["10.0.1.0/24","10.0.2.0/23","10.0.4.0/24"];
    assert_eq!(result, IPAddress::summarize(ip1,ip2,ip3,ip4).map(|i| i.to_string()));
    assert_eq!(result, IPAddress::summarize(ip4,ip3,ip2,ip1).map(|i| i.to_string()));

    ip1 = IPAddress.parse("10.0.1.1/24");
    ip2 = IPAddress.parse("10.10.2.1/24");
    ip3 = IPAddress.parse("172.16.0.1/24");
    ip4 = IPAddress.parse("172.16.1.1/24");
    result = ["10.0.1.0/24","10.10.2.0/24","172.16.0.0/23"];
    assert_eq!(result, IPAddress::summarize(ip1,ip2,ip3,ip4).map(|i| i.to_string()));

    let mut ips = [IPAddress.parse("10.0.0.12/30"),
           IPAddress.parse("10.0.100.0/24")];
    result = ["10.0.0.12/30", "10.0.100.0/24"];
    assert_eq!(result, IPAddress::summarize(ips).map(|i| i.to_string()));

    ips = [IPAddress.parse("172.16.0.0/31"),
           IPAddress.parse("10.10.2.1/32")];
    result = ["10.10.2.1/32", "172.16.0.0/31"];
    assert_eq!(result, IPAddress::summarize(ips).map(|i| i.to_string()));

    ips = [IPAddress.parse("172.16.0.0/32"),
           IPAddress.parse("10.10.2.1/32")];
    result = ["10.10.2.1/32", "172.16.0.0/32"];
    assert_eq!(result, IPAddress::summarize(ips).map(|i| i.to_string()));
  }


  pub fn test_classmethod_parse_classful() {
    for (ip, prefix) in setup().classful {
      let res = IPAddress::parse_classful(ip);
      assert_eq!(prefix, res.prefix.to_i());
      assert_eq!(format!("{}/{}", ip, prefix), res.to_string());
    }
    assert_raise(ArgumentError){ IPAddress::parse_classful("192.168.256.257") }
  }

} // class IPv4Test

}
