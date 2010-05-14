require 'test_helper'

class ExtensionsTest < Test::Unit::TestCase

  def test_method_power_of_2?
    assert_equal true, 16.power_of_2?
    assert_equal false, 20.power_of_2?
  end

  def test_method_closest_power_of_2
    assert_equal 8, 6.closest_power_of_2
    assert_equal 16, 13.closest_power_of_2    
    assert_equal 32, 24.closest_power_of_2
  end
  
end


