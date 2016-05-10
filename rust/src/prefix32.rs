

// ![feature(associated_consts)]

impl Prefix32 for Prefix {
  const IN4MASK: u32 = 0xffffffff;

  pub fn new(num: u8) Result<Prefix32, &'static str> {
    if (0 <= num && num <= 32) {
        return Ok(Prefix32 { prefix:  num })
    }
    return Err(format!("Prefix must be in range 0..32, got: {}", num))
  }

  pub fn host_prefix(&self) {
    return 32 - self.prefix
  }

  pub fn bits(&self) {
    return !format!("{#b032}", self.prefix)
  }

  //
  //  Gives the prefix in IPv4 dotted decimal format,
  //  i.e. the canonical netmask we're all used to
  //
  //    prefix = IPAddress::Prefix32.new 24
  //
  //    prefix.to_ip
  //      // => "255.255.255.0"
  //
  pub fn to_ip(&self) {
    return to_ip_str(self.to_u32())
  }

  pub fn to_ip_str(value: u32) {
    format!("{}.{}.{}.{}",
        (value >> 24) & 0xff,
        (value >> 16) & 0xff,
        (value >> 8) & 0xff,
         value & 0xff)
  }
  //
  //  An array of octets of the IPv4 dotted decimal
  //  format
  //
  //    prefix = IPAddress::Prefix32.new 24
  //
  //    prefix.octets
  //      // => [255, 255, 255, 0]
  //
  pub fn octets(&self) {
    let value = self.to_u32()
    vec![
        (value >> 24) & 0xff,
        (value >> 16) & 0xff,
        (value >> 8) & 0xff,
         value & 0xff]
  }

  //
  //  Unsigned 32 bits decimal number representing
  //  the prefix
  //
  //    prefix = IPAddress::Prefix32.new 24
  //
  //    prefix.to_u32
  //      // => 4294967040
  //
  pub fn to_u32(&self) {
    return (IN4MASK >> self.host_prefix()) << self.host_prefix()
  }

  //
  //  Shortcut for the octecs in the dotted decimal
  //  representation
  //
  //    prefix = IPAddress::Prefix32.new 24
  //
  //    prefix[2]
  //      // => 255
  //
  pub fn get(index) {
    return self.octets.get(index)
  }

  //
  //  The hostmask is the contrary of the subnet mask,
  //  as it shows the bits that can change within the
  //  hosts
  //
  //    prefix = IPAddress::Prefix32.new 24
  //
  //    prefix.hostmask
  //      // => "0.0.0.255"
  //
  pub fn hostmask() {
    to_ip_str(~to_u32())
  }

  //
  //  Creates a new prefix by parsing a netmask in
  //  dotted decimal form
  //
  //    prefix = IPAddress::Prefix32::parse_netmask "255.255.255.0"
  //      // => 24
  //
  pub fn parse_netmask(netmask: String) {
    let mut shift = 24
    let mut ip: u32 = 0
    val comp = netmask.split(".").map( |i|
        ip = ip || (i.parse::<u8>() << shift)
        shift -= 8
    )
    let mut null_prefix = 0
    let mut nulls = 0
    while (nulls < 32) {
        if (0 == (ip & 0x1)) {
            null_prefix++;
        } else {
            break;
        }
        ip = ip >> 1;
        nulls++;
    }
    let mut one_prefix = 0
    while (nulls < 32) {
        if (1 == (ip & 0x1)) {
            one_prefix++;
        } else {
            return Err(format!("Prefix must be 111 and 000 {}", &netmask))
        }
        ip = ip >> 1;
        nulls++;
    }
    return Prefix32::new(one_prefix)
  }

} //  class Prefix32 < Prefix
