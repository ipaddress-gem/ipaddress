require 'rubygems'
require 'rake'
require 'rake/clean'

begin
  require 'jeweler'
  Jeweler::Tasks.new do |gem|
    gem.name = "ipaddress"
    gem.summary = %Q{IPv4/IPv6 addresses manipulation library}
    gem.email = "ceresa@gmail.com"
    gem.homepage = "http://github.com/bluemonk/ipaddress"
    gem.authors = ["Marco Ceresa"]
    gem.description   = <<-EOD
      IPAddress is a Ruby library designed to make manipulation 
      of IPv4 and IPv6 addresses both powerful and simple. It mantains
      a layer of compatibility with Ruby's own IPAddr, while 
      addressing many of its issues.
    EOD
  end
rescue LoadError
  puts "Jeweler (or a dependency) not available. Install it with: sudo gem install jeweler"
end

require 'rake/testtask'
Rake::TestTask.new(:test) do |test|
  test.libs << 'lib' << 'test'
  test.pattern = 'test/**/*_test.rb'
  test.verbose = true
end

begin
  require 'rcov/rcovtask'
  Rcov::RcovTask.new do |test|
    test.libs << 'test'
    test.pattern = 'test/**/*_test.rb'
    test.verbose = true
  end
rescue LoadError
  task :rcov do
    abort "RCov is not available. In order to run rcov, you must: sudo gem install spicycode-rcov"
  end
end


task :default => :test

require 'rake/rdoctask'
Rake::RDocTask.new do |rdoc|
  if File.exist?('VERSION.yml')
    config = YAML.load(File.read('VERSION.yml'))
    version = "#{config[:major]}.#{config[:minor]}.#{config[:patch]}"
  else
    version = ""
  end

  rdoc.rdoc_dir = 'rdoc'
  rdoc.title = "ipaddress #{version}"
  rdoc.rdoc_files.include('README*')
  rdoc.rdoc_files.include('lib/**/*.rb')
end

desc "Open an irb session preloaded with this library"
task :console do
  sh "irb -rubygems -I lib -r ipaddress.rb"
end

desc "Look for TODO and FIXME tags in the code"
task :todo do
  def egrep(pattern)
    Dir['**/*.rb'].each do |fn|
      count = 0
      open(fn) do |f|
        while line = f.gets
          count += 1
          if line =~ pattern
            puts "#{fn}:#{count}:#{line}"
          end
        end
      end
    end
  end
  egrep /(FIXME|TODO|TBD)/
end
