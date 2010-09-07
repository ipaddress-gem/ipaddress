class << Math # :nodoc:
  def log2(n); log(n) / log(2); end
end

if RUBY_VERSION =~ /1\.8/
    class Hash # :nodoc:
        alias :key :index
    end
end

class Integer # :nodoc:
  def power_of_2?
    Math::log2(self).to_i == Math::log2(self)
  end

  def closest_power_of_2(limit=32)
    self.upto(limit) do |i|
      return i if i.power_of_2?
    end
  end
end

