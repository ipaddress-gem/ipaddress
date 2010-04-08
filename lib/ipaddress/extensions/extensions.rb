class << Math
  def log2(n); log(n) / log(2); end
end

class Integer
  def power_of_2?
    Math::log2(self).to_i == Math::log2(self)
  end

  def closest_power_of_2
    self.upto(32) do |i|
      return i if i.power_of_2?
    end
  end
end

