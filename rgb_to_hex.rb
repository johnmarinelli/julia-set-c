class Color
  attr_accessor :r, :g, :b

  def initialize(r, g, b)
    @r = r
    @g = g
    @b = b
  end

  def rgb
    [@r, @g, @b]
  end
end

class HexColor < Color
  attr_accessor hex_string

  def initialize(hex_string)
    hex_string.slice! 2, hex_string.length if hex_string.start_with? '0x'
    @hex_string = hex_string
  end

  alias_method :to_s, :hex_string
end

module Convert
  def hex_to_rgb(hex_color)
    hex_string = hex_color.to_s
    [
      Integer hex_string[0..1], 16
      Integer hex_string[2..3], 16
      Integer hex_string[4..5], 16
    ]
  end

  def rgb_to_hex(r, g, b)
    [r, g, b].map { |c| "%.2s" % c.to_s(16) }.join.prepend '0x'
  end
end

module Format
  HEX = 0
  RGB = 1
end

class Gradient
  attr_accessor :color_begin, :color_end, :output_format 
  
  def initialize(b, e)
    @color_begin = b
    @color_end = e
    @output_format = Format::HEX
  end

  def lerp(pct)
    num_slices = 768
    if Format::HEX == @output_format
      br, bg, bb = Convert::hex_to_rgb @color_begin.to_s
      er, eg, eb = Convert::hex_to_rgb @color_end.to_s

      get_inc = lambda { |c1, c2| (c1 - c2).abs / Float(num_slices) }

      rinc = get_inc br, er
      ginc = get_inc bg, eg
      binc = get_inc bb, eb

    end
  end
end


