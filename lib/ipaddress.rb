$LOAD_PATH.unshift(File.dirname(__FILE__))
$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))

require 'ipaddress/ipbase'
require 'ipaddress/ipv4'


def IPAddress(str)
  if str.include? "-"
    IPAddress::Range.new(str)
  else
    begin
      IPAddress::IPv4.new(str)
    rescue ArgumentError
      IPAddress::IPv6.new(str)
    end
  end
end
  



