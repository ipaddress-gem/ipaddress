
  /*
  # =NAME
  #
  # IPAddress::Prefix
  #
  # =SYNOPSIS
  #
  # Parent class for Prefix32 and Prefix128
  #
  # =DESCRIPTION
  #
  # IPAddress::Prefix is the parent class for IPAddress::Prefix32
  # and IPAddress::Prefix128, defining some modules in common for
  # both the subclasses.
  #
  # IPAddress::Prefix shouldn't be accesses directly, unless
  # for particular needs.
  */
  class Prefix  {
    prefix: number;
    /*
    # Creates a new general prefix
    */
    constructor(num: number) {
      this.prefix = num;
    }

    /*
    # Returns a string with the prefix
    */
    public to_s(): string {
      return "#"+prefix;
    }

    /*
    # Returns the prefix
    */
    public to_i(): number {
      return prefix;
    }

    /*
    # Compare the prefix
i   */
    public compare(oth: Prefix) : number {
      return this.prefix - oth.prefix;
    }

    /*
    # Sums two prefixes or a prefix to a
    # number, returns a Fixnum
    */
    public add(oth: Prefix) : number {
      return self.prefix + oth.prefix;
    }

    /*
    # Returns the difference between two
    # prefixes, or a prefix and a number,
    # as a Fixnum
    */
    public sub(oth: Prefix) : number {
      return self.prefix - oth.prefix;
  }
  }


  
end # module IPAddress
