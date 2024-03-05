require "bundler/gem_tasks"
require "rake/clean"
require "rake/testtask"
require "rdoc/task"

require_relative "lib/ipaddress/version"

Rake::RDocTask.new do |rdoc|
  rdoc.rdoc_dir = 'rdoc'
  rdoc.title = "ipaddress #{IPAddress::VERSION}"
  rdoc.rdoc_files.include('README*')
  rdoc.rdoc_files.include('lib/**/*.rb')
end

Rake::TestTask.new(:test) do |test|
  test.libs << 'lib' << 'test'
  test.pattern = 'test/**/*_test.rb'
  test.verbose = true
  test.warning = true
end

task :default => :test
