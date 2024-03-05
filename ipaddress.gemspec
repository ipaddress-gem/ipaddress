require_relative "lib/ipaddress/version"

Gem::Specification.new do |spec|
  # Information
  spec.name        = "ipaddress"
  spec.version     = IPAddress::VERSION
  spec.summary     = "IPv4/IPv6 address manipulation library"
  spec.description = <<~DESC
    IPAddress is a Ruby library designed to make manipulation
    of IPv4 and IPv6 addresses both powerful and simple. It maintains
    a layer of compatibility with Ruby's own IPAddr, while
    addressing many of its issues.
  DESC
  spec.homepage = "https://github.com/ipaddress-gem/ipaddress"
  spec.license  = "MIT"

  # Ownership
  spec.authors = ["bluemonk", "mikemackintosh"]
  spec.email   = "ceresa@gmail.com"

  # Metadata
  spec.metadata["bug_tracker_uri"]  = "#{spec.homepage}/issues"
  spec.metadata["changelog_uri"]    = "#{spec.homepage}/blob/v#{spec.version}/CHANGELOG.md"
  spec.metadata["homepage_uri"]     = spec.homepage
  spec.metadata["mailing_list_uri"] = "#{spec.homepage}/discussions"
  spec.metadata["source_code_uri"]  = "#{spec.homepage}/tree/v#{spec.version}"

  # Gem
  spec.files = Dir["lib/**/*", "CHANGELOG.md", "LICENSE.txt", "README.md"]

  # Ruby
  spec.required_ruby_version = "~> 3.1"

  # Documentation
  spec.extra_rdoc_files = ["CHANGELOG.md", "LICENSE.txt", "README.md"]
end
