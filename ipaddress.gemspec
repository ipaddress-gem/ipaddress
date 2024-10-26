require_relative 'lib/ipaddress/version'

Gem::Specification.new do |s|
  s.name = 'ipaddress'
  s.version = IPAddress::VERSION

  s.summary = 'A library for working with IPv4 and IPv6 addresses.'
  s.description = <<-EOF
    A Ruby library designed to make manipulation of IPv4 and IPv6 addresses both powerful and simple. 
    It maintains a layer of compatibility with Ruby's built-in IPAddr, while addressing many of its issues.
  EOF

  s.authors = ['bluemonk', 'mikemackintosh']
  s.email = 'ceresa@gmail.com'
  s.homepage = 'https://github.com/ipaddress-gem/ipaddress'
  s.license = 'MIT'

  s.extra_rdoc_files = %w[README.md CHANGELOG.md LICENSE.txt]
  s.require_paths = ['lib']
  
  # TODO enable this when bumping the major version of this gem
  # s.required_ruby_version = '>= 3.1.0'

  s.files = Dir[
    'lib/**/*.rb',
    '.document',
    'CHANGELOG.md',
    'CONTRIBUTING.md',
    'LICENSE.txt',
    'README.md',
    'Rakefile',
  ]

  s.metadata = {
    'homepage_uri'      => 'https://github.com/ipaddress-gem/ipaddress',
    'bug_tracker_uri'   => 'https://github.com/ipaddress-gem/ipaddress/issues',
    'changelog_uri'     => 'https://github.com/ipaddress-gem/ipaddress/blob/master/CHANGELOG.md',
    'documentation_uri' => 'https://github.com/ipaddress-gem/ipaddress/blob/master/README.md',
    'source_code_uri'   => 'https://github.com/ipaddress-gem/ipaddress',
  }
end
