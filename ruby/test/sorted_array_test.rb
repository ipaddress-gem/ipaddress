require_relative 'test_helper'

require 'pry'

require_relative '../lib/sorted_array'

class SortedArrayTest < Test::Unit::TestCase
  def setup
    @unsorted = ['x', 'd', 'e', 'o', 'c', 'q', 'j', 'm', 'o']
    @unsorted = ['x', 'd', 'e', 'o']
    @sorted = @unsorted.sort
  end

  def test_file
    f = IO.read(__FILE__).split(/./)
    sa = SortedArray.new(*f)
    assert_equal sa, f.sort.uniq
  end

  def test_constructor
    sa = SortedArray.new(*@unsorted)
    assert_equal @sorted, sa
    sb = SortedArray.new(@unsorted)
    assert_equal @sorted, sb
    sc = SortedArray.new(@sorted)
    assert_equal @sorted, sc
  end

  def test_add
      sa = SortedArray.new
    @unsorted.each do |c|
      sa.add(c)
    end
    assert_equal @sorted, sa
    @sorted.sort.each do |c|
      sa.add(c)
    end
    assert_equal @sorted, sa
  end

  def test_del
    sa = SortedArray.new(@unsorted)
    sa.delete_at(2)
    sorted.delete_at(2)
    assert_equal @sorted, sa
  end

  def test_lt
      sa = SortedArray.new(@unsorted)
    assert_equal sa.lt('e'), 'd'
    assert_equal sa.lt('a'), nil
  end
  def test_le
    se = SortedArray.new
    assert_equal sa.le('a'), nil
    sa = SortedArray.new(@unsorted)
    assert_equal sa.le('a'), nil
    assert_equal sa.le('e'), 'e'
    assert_equal sa.le('i'), 'e'
    assert_equal sa.le('z'), 'x'
  end
  def test_gt
    sa = SortedArray.new(@unsorted)
    assert_equal sa.gt('e'), 'o'
    assert_equal sa.gt('z'), nil
  end
  def test_ge
    sa = SortedArray.new(@unsorted)
    assert_equal sa.ge('e'), 'e'
    assert_equal sa.ge('z'), nil
  end
  def test_eq
    sa = SortedArray.new(@unsorted)
    assert_equal sa.eq('e'), 'e'
    assert_equal sa.eq('z'), nil
  end


end
