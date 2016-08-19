
class SortedArray < Array
  def initialize(*arr)
    arr.flatten.each do |a|
      self.add(a)
    end
  end
  class PosVal
    attr_reader :pos, :val
    def initialize(pos, val)
      @pos = pos
      @val = val
    end
  end
  class Between
    BEFORE = :before
    AFTER = :after
    BETWEEN = :between
    FOUND = :found
    EMPTY = :empty
    DONE = :done
    attr_reader :lower
    attr_reader :upper
    attr_reader :direction
    def initialize(direction, lower, upper)
      @direction = direction
      @lower = lower
      @upper = upper
    end
  end

  def _bsearch_le(start, _end, &block)
    #binding.pry
    return Between.new(Between::EMPTY, nil, nil) if _end < 0
    diff = _end - start
    return Between.new(Between::DONE, nil, nil) if diff == 0 && _end > 1
    pos = start + diff/2
    lower = PosVal.new(pos, self[pos])
    upper = PosVal.new(pos+1, self[pos+1])
    ret_lower = block.call(lower.val)
    ret_upper = block.call(upper.val)
    #binding.pry if ret_lower.nil?
    #binding.pry if ret_upper.nil?
    puts "start:#{start} _end:#{_end} pos:#{pos} val_lower:#{lower.val} val_upper:#{upper.val} ret_lower:#{ret_lower} ret_upper:#{ret_upper}"
    if ret_lower == 0
      return Between.new(Between::FOUND, lower, upper)
    elsif ret_upper == 0
      return Between.new(Between::FOUND, lower, upper)

    elsif ret_lower > 0 && upper.val.nil?
      return Between.new(Between::BEFORE, lower, upper)
    elsif ret_lower < 0 && ret_upper.nil?
      return Between.new(Between::AFTER, lower, upper)

    elsif ret_lower > 0 && ret_upper > 0
      return _bsearch_le(start, pos, &block)
    elsif ret_lower < 0 && ret_upper < 0
      return _bsearch_le(pos+1, _end, &block)

    elsif ret_lower > 0 && ret_upper < 0
      return Between.new(Between::BETWEEN, lower, upper)
    elsif ret_lower < 0 && ret_upper > 0
      return Between.new(Between::BETWEEN, lower, upper)

    elsif ret_lower > 0
      return _bsearch_le(start, pos, &block)
    elsif ret_upper > 0
      return _bsearch_le(pos+1, _end, &block)
    end
    throw "never go here!"
  end
  def bsearch_le(&block)
    self._bsearch_le(0, self.length-1, &block)
  end


  def add(elem)
    between = self.bsearch_le { |i| i <=> elem  }
    puts ">>add:#{between.inspect} #{elem}"
    if [Between::AFTER, Between::EMPTY].include?(between.direction)
      self.push(elem)
    elsif between.direction == Between::BEFORE
      self.insert(between.lower.pos, elem)
    elsif between.direction == Between::BETWEEN
      self.insert(between.upper.pos, elem)
    end
  end

  def le(item)
    binding.pry if item == 'i'
    ret = self.bsearch_le { |i| i <=> item  }
    ret.nil? ? nil : ret.val
  end
  def lt(item)
  end
  def ge(item)
  end
  def gt(item)
  end
  def eq(item)
  end
end
#
#
# def _bsearch_eq(start, _end, &block)
#   return nil if _end < 0
#   pos = (_end - start)/2
#   return nil if pos == 0
#   val = self[pos];
#   ret = block.call(val)
#   if ret == 0
#     return PosVal.new(pos, val)
#   elsif ret > 0
#     _bsearch_eq(pos, _end, &block)
#   else
#     _bsearch_eq(start, pos, &block)
#   end
# end
# def bsearch_eq(&block)
#   self._bsearch_eq(0, self.length-1, &block)
# end
