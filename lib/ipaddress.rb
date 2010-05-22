$LOAD_PATH.unshift(File.dirname(__FILE__))
$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))

require 'ipaddress/ipbase'
require 'ipaddress/ipv4'
require 'ipaddress/ipv6'

#
# IPAddress is a wrapper method built around 
# IPAddress's library classes. Its purpouse is to 
# make you indipendent from the type of IP address 
# you're going to use.
#
# For example, instead of creating the three types 
# of IP addresses using their own contructors
#
#   ip  = IPAddress::IPv4.new "172.16.10.1/24"
#   ip6 = IPAddress::IPv6.new "2001:db8::8:800:200c:417a/64"
#   ip_mapped = IPAddress::IPv6::Mapped "::ffff:172.16.10.1/128" 
#
# you can just use the IPAddress wrapper:
#
#   ip  = IPAddress "172.16.10.1/24"
#   ip6 = IPAddress "2001:db8::8:800:200c:417a/64"
#   ip_mapped = IPAddress "::ffff:172.16.10.1/128"
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
#
def IPAddress(str)
  case str
  when /:.+\./
    IPAddress::IPv6::Mapped.new(str)
  else
    begin
      IPAddress::IPv4.new(str)
    rescue ArgumentError
      IPAddress::IPv6.new(str)
    end
  end
end




