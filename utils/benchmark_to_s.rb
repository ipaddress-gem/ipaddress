$:.unshift(File.dirname(__FILE__) + '/../lib')

require 'benchmark'
require 'ipaddress'
require 'ipaddr' 

n = 200_000

 Benchmark.bm do|b|
   b.report("ipaddr ") do
    n.times { IPAddr.new("192.168.10.100").to_s }
   end
 
   b.report("ipaddress parse ") do
    n.times { IPAddress.parse("192.168.10.100").to_s }
   end

   b.report("ipaddress ipv4 ") do
    n.times { IPAddress::IPv4.new("192.168.10.100").to_s }
   end
 end
