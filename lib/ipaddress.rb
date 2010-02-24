$LOAD_PATH.unshift(File.dirname(__FILE__))
$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))

require 'ipaddress/ipbase'
require 'ipaddress/ipv4'
require 'ipaddress/ipv6'

def IPAddress(str)
  case str
  when /[:\.]/
    IPAddress::IPv6::Mapped.new(str)
  else
    begin
      IPAddress::IPv4.new(str)
    rescue ArgumentError
      IPAddress::IPv6.new(str)
    end
  end
end




