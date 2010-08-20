$:.unshift(File.dirname(__FILE__) + '/../lib')

require 'rubygems'
require 'ruby-prof'
require 'ipaddress'

n = 50_000
# Profile the code
result = RubyProf.profile do
  n.times { IPAddress.parse("192.168.10.100").to_s }
end

# Print a graph profile to text
printer = RubyProf::FlatPrinter.new(result)
printer.print(STDOUT, 0)
