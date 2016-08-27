
import Crunchy from './crunchy';
import Prefix32 from './prefix32';
import IPAddress from './ipaddress';
import IpBits from './ip_bits';

class Ipv4 {
    public static from_number(addr: number, prefix_num: number): IPAddress {
        let prefix = Prefix32.create(prefix_num);
        if (!prefix) {
            return null;
        }
        return new IPAddress({
            ip_bits: IpBits.v4(),
            host_address: Crunchy.from_number(addr),
            prefix: prefix,
            mapped: null,
            vt_is_private: Ipv4.ipv4_is_private,
            vt_is_loopback: Ipv4.ipv4_is_loopback,
            vt_to_ipv6: Ipv4.to_ipv6,
        });
    }

    public static create(str: string): IPAddress {
        let(ip, netmask) = IPAddress::split_at_slash(&str);
        if !IPAddress::is_valid_ipv4(ip.clone()) {
            return Err(format!("Invalid IP {}", str));
        }
        let mut ip_prefix_num = Ok(32);
        if netmask.is_some() {
            //  netmask is defined
            ip_prefix_num = IPAddress::parse_netmask_to_prefix(netmask);
            if ip_prefix_num.is_err() {
                return Err(ip_prefix_num.unwrap_err());
            }
            //if ip_prefix.ip_bits.version
        }
        let ip_prefix = prefix32::new(ip_prefix_num);
        if ip_prefix.is_err() {
            return Err(ip_prefix.unwrap_err());
        }
        let split_number = IPAddress::split_to_number(&ip);
        if split_number.is_err() {
            return Err(split_number.unwrap_err());
        }
        return Ok(IPAddress {
            ip_bits: ::ip_bits::v4(),
            host_address: BigUint::from_number(split_number),
            prefix: ip_prefix,
            mapped: None,
            vt_is_private: ipv4_is_private,
            vt_is_loopback: ipv4_is_loopback,
            vt_to_ipv6: to_ipv6,
        });
    }

    public static ipv4_is_private(my: IPAddress): bool {
        return [IPAddress::parse("10.0.0.0/8"),
            IPAddress::parse("172.16.0.0/12"),
            IPAddress::parse("192.168.0.0/16")]
            .iter().find(|i | i.includes(my)).is_some();
    }

    public static ipv4_is_loopback(my: IPAddress): bool {
        return IPAddress::parse("127.0.0.0/8")
            .includes(my);
    }

    public static to_ipv6(ia: IPAddress): IPAddress {
        return IPAddress {
            ip_bits: ::ip_bits::v6(),
                host_address: ia.host_address.clone(),
                    prefix: ::prefix128::new (ia.prefix.num),
                        mapped: None,
                            vt_is_private: ::ipv6::ipv6_is_private,
                                vt_is_loopback: ::ipv6::ipv6_is_loopback,
                                    vt_to_ipv6: ::ipv6::to_ipv6
        }
    }

    //  Checks whether the ip address belongs to a
    //  RFC 791 CLASS A network, no matter
    //  what the subnet mask is.
    //
    //  Example:
    //
    //    ip = IPAddress("10.0.0.1/24")
    //
    //    ip.a?
    //      // => true
    //
    public static is_class_a(my: IPAddress): bool {
        return my.is_ipv4() && my.host_address < BigUint::from_number(0x80000000);
    }

    //  Checks whether the ip address belongs to a
    //  RFC 791 CLASS B network, no matter
    //  what the subnet mask is.
    //
    //  Example:
    //
    //    ip = IPAddress("172.16.10.1/24")
    //
    //    ip.b?
    //      // => true
    //
    public static is_class_b(my: IPAddress): bool {
        return my.is_ipv4() &&
            BigUint::from_number(0x80000000) <= my.host_address &&
                my.host_address < BigUint::from_number(0xc0000000);
    }

    //  Checks whether the ip address belongs to a
    //  RFC 791 CLASS C network, no matter
    //  what the subnet mask is.
    //
    //  Example:
    //
    //    ip = IPAddress("192.168.1.1/30")
    //
    //    ip.c?
    //      // => true
    //
    public static is_class_c(my: IPAddress): boolean {
        return my.is_ipv4() &&
            Crunchy.from_number(0xc0000000).lte(my.host_address) &&
                my.host_address.lt(Crunchy.from_number(0xe0000000));
    }


    //  Creates a new IPv4 address object by parsing the
    //  address in a classful way.
    //
    //  Classful addresses have a fixed netmask based on the
    //  class they belong to:
    //
    //  * Class A, from 0.0.0.0 to 127.255.255.255
    //  * Class B, from 128.0.0.0 to 191.255.255.255
    //  * Class C, D and E, from 192.0.0.0 to 255.255.255.254
    //
    //  Example:
    //
    //    ip = IPAddress::IPv4.parse_classful "10.0.0.1"
    //
    //    ip.netmask
    //      // => "255.0.0.0"
    //    ip.a?
    //      // => true
    //
    //  Note that classes C, D and E will all have a default
    //  prefix of /24 or 255.255.255.0
    //
    public parse_classful(ip_si: string): IPAddress {
        if !IPAddress.is_valid_ipv4(ip_si) {
            return null;
        }
        let o_ip = IPAddress.parse(ip_si);
        if (o_ip) {
            return o_ip;
        }
        let ip = o_ip;
        if (Ipv4.is_class_a(ip)) {
            ip.prefix = Prefix32.create(8);
        } else if (Ipv4.is_class_b(ip)) {
            ip.prefix = Prefix32.create(16);
        } else if (Ipv4.is_class_c(ip)) {
            ip.prefix = Prefix32.create(24);
        }
        return ip;
    }
}


export default Ipv4;