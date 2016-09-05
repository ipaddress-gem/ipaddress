

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

import IPAddress from './ipaddress';
import Crunchy from './crunchy';
//export = Crunchy;
export default IPAddress;
//declare let Ip: IPAddress;
