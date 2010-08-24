$:.unshift(File.dirname(__FILE__) + '/../lib')

require 'benchmark'
require 'ipaddress'
require 'ipaddr' 

n = 200_000

ipaddr    = IPAddr.new("192.168.10.100")
ipaddress = IPAddress::IPv4.new("192.168.10.100")

Benchmark.bm do|b|
  b.report("ipaddr ") do
    n.times { IPAddr.new("192.168.10.0/24").include?(ipaddr) }
  end
  
  b.report("ipaddress ") do
    n.times { IPAddress::IPv4.new("192.168.10.0/24").include?(ipaddress) }
  end
end
