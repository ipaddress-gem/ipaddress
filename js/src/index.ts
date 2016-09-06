

export * from './crunchy';
export * from './prefix';
export * from './ip_bits';
export * from './ip_version';
export * from './ipaddress';
export * from './ipv4';
export * from './ipv6';
export * from './ipv6_mapped';
export * from './ipv6_loopback';
export * from './ipv6_unspec';

import Crunchy from './crunchy';
import Ipv4 from './ipv4';
import Ipv6 from './ipv6';
import Prefix from './prefix';
import Ipv6Mapped from './ipv6_mapped';
import Ipv6Loopback from './ipv6_loopback';
import Ipv6Unspec from './ipv6_unspec';
import IPAddress from './ipaddress';
//export = Crunchy;
export default IPAddress;
//declare let Ip: IPAddress;
