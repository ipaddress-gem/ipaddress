require 'jeweler'
Jeweler::Tasks.new do |gem|
  # gem is a Gem::Specification... see http://guides.rubygems.org/specification-reference/ for more options
  gem.name = "ipaddress"
  gem.summary = %q{IPv4/IPv6 address manipulation library}
  gem.description = %q{IPAddress is a Ruby library designed to make manipulation
      of IPv4 and IPv6 addresses both powerful and simple. It mantains
      a layer of compatibility with Ruby's own IPAddr, while
      addressing many of its issues.}
  gem.email = "ceresa@gmail.com"
  gem.homepage = "https://github.com/bluemonk/ipaddress"
  gem.authors = ["bluemonk", "mikemackintosh"]
  gem.license = "MIT"
end
Jeweler::RubygemsDotOrgTasks.new
