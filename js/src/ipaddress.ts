

abstract class IPAddress {
  /   /*
    # True if the object is an IPv4 address
    #
    #   ip = IPAddress("192.168.10.100/24")
    #
    #   ip.ipv4?
    #     #-> true
    */
    abstract is_ipv4() : bool;
    abstract is_ipv6() : bool;
    /*
   # Parse the argument string to create a new
   # IPv4, IPv6 or Mapped IP object
   #
   #   ip  = IPAddress.parse "172.16.10.1/24"
   #   ip6 = IPAddress.parse "2001:db8::8:800:200c:417a/64"
   #   ip_mapped = IPAddress.parse "::ffff:172.16.10.1/128"
   #
   # All the object created will be instances of the
   # correct class:
   #
   #  ip.class
   #    #=> IPAddress::IPv4
   #  ip6.class
   #    #=> IPAddress::IPv6
   #  ip_mapped.class
   #    #=> IPAddress::IPv6::Mapped
   */
   public static parse(str: string) : IpAddress {
   if (str.match(/:.+\./) {
      return new IPAddress.IPv6.Mapped(str);
    } else if (str.match(/\./)) {
      return new IPAddress.IPv4(str);
    } else if (str.match(/:/)) {
      return new IPAddress.IPv6(str);
    }
    return null;
   }

   /*
  # Checks if the given string is a valid IP address,
  # either IPv4 or IPv6
  #
  # Example:
  #
  #   IPAddress::valid? "2002::1"
  #     #=> true
  #
  #   IPAddress::valid? "10.0.0.256"
  #     #=> false
  */
  public static is_valid(addr: string) {
    return is_valid_ipv4(addr) || is_valid_ipv6(addr)
  }

  /*
  # Checks if the given string is a valid IPv4 address
  #
  # Example:
  #
  #   IPAddress::valid_ipv4? "2002::1"
  #     #=> false
  #
  #   IPAddress::valid_ipv4? "172.16.10.1"
  #     #=> true
  */
    public static split_to_u32(addr: string) : Big {
        let ip = 0;
        let shift = 1<<24;
        for (let i of addr.split(".")) {
          try {
            let part = new Big(i);
            if (part < 0 || 255 < part) {
              return null;
            }
            ip = ip | part.mul(shift);
            shift = shift.div(1<<8);
          } catch (e) {
            return null;
          }
        }
        return ip;
    }
    public static valid_ipv4(addr: string) : bool {
      return IPAddres.split_to_u32(addr) != null;
    }

    /*
  # Checks if the argument is a valid IPv4 netmask
  # expressed in dotted decimal format.
  #
  #   IPAddress.valid_ipv4_netmask? "255.255.0.0"
  #     #=> true
   */
   public static is_valid_ipv4_netmask(addr: string) : bool {
    return Prefix32.parse_netmask(addr) != null;
   }

   /*
  # Checks if the given string is a valid IPv6 address
  #
  # Example:
  #
  #   IPAddress::valid_ipv6? "2002::1"
  #     #=> true
  #
  #   IPAddress::valid_ipv6? "2002::DEAD::BEEF"
  #     #=> false
   */
   class SplitOnColon {
     public val: Big = new Big(0);
     public bits: number = 0;
   }
    public static split_on_colon(addr: string) : SplitOnColon {
        let parts = addr.trim().split(":");
        let soc = new SplitOnColon();
        if (parts[0].length==0) {
            return soc;
        }
        let shift = (parts.length - 1) * 16;
        for (let i of parts) {
          if (!i.match(/^[0-9a-fA-F]{0,4}$/)) {
            return null;
          }
            let part = parseInt(i, 16);
            if (part < 0 || 65536 <= part) {
              return null;
            }
            soc.val = soc.val.add((new Big(part)).mul((new Big(2)).pow(shift)));
            shift -= 16;
        }
        soc.number = parts.length
        return soc;
    }
    public split_to_num(addr: string) -> Big {
        let ip = new Big();
        let pre_post = addr.trim().split("::");
        if (pre_post.length > 2) {
            return null;
        }
        if (pre_post.length == 2) {
            let pre_soc = IPAddress.split_on_colon(pre_post[0]);
            if (pre_soc == null) {
                return null;
            }
            let post_soc = IPAddress.split_on_colon(pre_post[1]);
            if (post_soc == null)
                return null;
            }
            return pre_soc.val.mul((new Big(2)).pow(128 - (pre_parts * 16) - (post_parts * 16)))
              .add(pos_soc.val);
        }
        let soc = IPAddress.split_on_colon(addr);
        if (soc == null) {
          return null;
        }
        return soc.val;
    }

    public static is_valid_ipv6(addr: string) : bool {
        return IPAddress.split_to_num(addr) != null;
    }

  /*
  # private helper for summarize
  # assumes that networks is output from reduce_networks
  # means it should be sorted lowers first and uniq
  */

  public static aggregate(networks: IpAddressp[]) {
    let stack = networks.map(i=>i.network()).sort();
    let pos = 0
    while (true) {
      pos = pos < 0 ? 0 : pos; // start @beginning
      let first = stack[pos];
      if (!first) {
        break;
      }
      pos += 1;
      second = stack[pos];
      if (!second) {
        break;
      }
      pos += 1;
      if (first.includes(second))
        pos -= 2;
        stack.splice(pos+1,1);
      else
        first.prefix -= 1;
        if (first.prefix+1 == second.prefix && first.includes(second)) {
          pos -= 2;
          stack[pos] = first;
          stack.slice(pos+1, 1);
          pos -= 1; // backtrack
        } else {
          first.prefix += 1; //reset prefix
          pos -= 1; // do it with second as first
        }
      }
    }
    return stack.splice(0,pos-1);
  }




  /*
  # Summarization (or aggregation) is the process when two or more
  # networks are taken together to check if a supernet, including all
  # and only these networks, exists. If it exists then this supernet
  # is called the summarized (or aggregated) network.
  #
  # It is very important to understand that summarization can only
  # occur if there are no holes in the aggregated network, or, in other
  # words, if the given networks fill completely the address space
  # of the supernet. So the two rules are:
  #
  # 1) The aggregate network must contain +all+ the IP addresses of the
  #    original networks;
  # 2) The aggregate network must contain +only+ the IP addresses of the
  #    original networks;
  #
  # A few examples will help clarify the above. Let's consider for
  # instance the following two networks:
  #
  #   ip1 = IPAddress("172.16.10.0/24")
  #   ip2 = IPAddress("172.16.11.0/24")
  #
  # These two networks can be expressed using only one IP address
  # network if we change the prefix. Let Ruby do the work:
  #
  #   IPAddress::IPv4::summarize(ip1,ip2).to_s
  #     #=> "172.16.10.0/23"
  #
  # We note how the network "172.16.10.0/23" includes all the addresses
  # specified in the above networks, and (more important) includes
  # ONLY those addresses.
  #
  # If we summarized +ip1+ and +ip2+ with the following network:
  #
  #   "172.16.0.0/16"
  #
  # we would have satisfied rule #1 above, but not rule #2. So "172.16.0.0/16"
  # is not an aggregate network for +ip1+ and +ip2+.
  #
  # If it's not possible to compute a single aggregated network for all the
  # original networks, the method returns an array with all the aggregate
  # networks found. For example, the following four networks can be
  # aggregated in a single /22:
  #
  #   ip1 = IPAddress("10.0.0.1/24")
  #   ip2 = IPAddress("10.0.1.1/24")
  #   ip3 = IPAddress("10.0.2.1/24")
  #   ip4 = IPAddress("10.0.3.1/24")
  #
  #   IPAddress::IPv4::summarize(ip1,ip2,ip3,ip4).to_string
  #     #=> "10.0.0.0/22",
  #
  # But the following networks can't be summarized in a single network:
  #
  #   ip1 = IPAddress("10.0.1.1/24")
  #   ip2 = IPAddress("10.0.2.1/24")
  #   ip3 = IPAddress("10.0.3.1/24")
  #   ip4 = IPAddress("10.0.4.1/24")
  #
  #   IPAddress::IPv4::summarize(ip1,ip2,ip3,ip4).map{|i| i.to_string}
  #     #=> ["10.0.1.0/24","10.0.2.0/23","10.0.4.0/24"]
  */
  public static summarize(networks: IPAddress[]) {
    return aggregate(networks);
  }
  public static summarize(networks: String[]) {
    return aggregate(networks.map(i => IPAddress.parse(i)))
  }

  static splitPrefix(addr: string) : string[] {
    let slash = addr.split("/");
    if (slash.length == 1) {
      return [slash[0], ""];
    }
    return [slash[0], slash[1]];
  }
  static isPrefixValid(af: ProtoFamily, sPrefix: string) : boolean {
    if (sPrefix == null) {
      return false;
    }
    if (sPrefix.length == 0) {
      return true;
    }
    let prefix = parseInt(sPrefix, 10);
    if (isNaN(prefix)) {
      return false;
    }
    if (af == ProtoFamily.AF_INET && 0 <= prefix && prefix <= 32) {
      return true;
    }
    if (af == ProtoFamily.AF_INET6 && 0 <= prefix && prefix <= 128) {
      return true;
    }
    return false;
  }

  public static isValidWithoutPrefix(addr: string) : boolean {
    let slash = addr.split("/");
    if (slash.length != 1) {
      return false;
    }
    return IfAddrs.isValidWithPrefix(addr);
  }
  public static splitToIpv4(addr: string) : Number[] {
    let dots = addr.split(".");
    if (dots.length < 2) {
      return null;
    }
    let parts = Array<Number>();
    for (let part of dots) {
      if (!/^\d+$/.test(part)) {
        return null;
      }
      let num = parseInt(part, 10);
      if (isNaN(num)) {
        return null;
      }
      if (num < 0 || 256 <= num) {
        return null;
      }
      parts.push(num);
    }
    return parts;
  }
  public static split_on_colon(addr: string) : Number[] {
    let ret = Array<Number>();
    let parts = addr.split(":");
    if (parts.length == 0) {
      ret.push(0);
      return ret;
    }
    for (let part of parts) {
      if (!/^[0-9A-Fa-f]+$/.test(part)) {
        return null;
      }
      let num = parseInt(part, 16);
      if (isNaN(num)) {
        return null;
      }
      if (num < 0 || 65536 <= num) {
        return null;
      }
      ret.push(num);
    }
    return ret;
  }
  public static splitToIpv6(addr: string) : Number[] {
        let pre_post = addr.split("::");
        if (pre_post.length > 2) {
          return null;
        }
        if (pre_post.length == 2) {
            let pre = IfAddrs.split_on_colon(pre_post[0]);
            if (!pre) {
              return null;
            }
            let post = IfAddrs.split_on_colon(pre_post[1]);
            if (!post) {
              return null;
            }
            let zeros = Array<Number>();
            for (let i = 0; i < 128/16 - pre.length - post.length; ++i) {
              zeros.push(0);
            }
            return pre.concat(zeros.concat(post));
        }
        return IfAddrs.split_on_colon(addr);
  }
  public static isValidWithPrefix(addr: string) : boolean {
    let sp = IfAddrs.splitPrefix(addr);
    if (sp[0].length == 0) {
      return false;
    }
    if (IfAddrs.splitToIpv4(sp[0])) {
      return IfAddrs.isPrefixValid(ProtoFamily.AF_INET, sp[1]);
    }
    if (IfAddrs.splitToIpv6(sp[0])) {
     return IfAddrs.isPrefixValid(ProtoFamily.AF_INET6, sp[1]);
    }
    return false;
  }



}
