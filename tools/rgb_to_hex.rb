require 'awesome_print'
require 'rmagick'

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
  attr_accessor :hex_string

  def initialize(hex_string)
    hex_string.slice! 0, 2 if hex_string.start_with? '0x'
    @hex_string = hex_string
    @r = "0x#{@hex_string[0..1]}"
    @g = "0x#{@hex_string[2..3]}"
    @b = "0x#{@hex_string[4..5]}"
  end

  alias_method :to_s, :hex_string
end

module Convert
  def self.hex_to_rgb(hex_color)
    hex_string = hex_color.hex_string

    [
      Integer(hex_string[0..1], 16),
      Integer(hex_string[2..3], 16),
      Integer(hex_string[4..5], 16)
    ]
  end

  def self.rgb_to_hex(r, g, b)
    HexColor.new("%02x%02x%02x" % [r, g, b])
  end
end

module Format
  HEX = 0
  RGB = 1
end

class Gradient
  attr_accessor :color_begin, :color_end, :output_format, :colors
  
  def initialize(b, e)
    @color_begin = b
    @color_end = e
    @output_format = Format::HEX

    make_gradient 
  end

private

  def make_gradient
    num_slices = 256
    br, bg, bb = Convert::hex_to_rgb @color_begin
    er, eg, eb = Convert::hex_to_rgb @color_end

    get_inc = lambda { |c1, c2| ((c1 - c2).abs / Float(num_slices)) }

    rinc = get_inc.call br, er
    ginc = get_inc.call bg, eg
    binc = get_inc.call bb, eb

    colors = []

    (0..num_slices).each { |n|
      br += rinc
      bg += ginc
      bb += binc

      color = Color.new *([br, bg, bb].map(&:floor))
      colors << color
    }

    if Format::HEX == @output_format
      colors.map! { |c| 
        Convert::rgb_to_hex *c.rgb 
      }
    end

    @colors = colors
  end
end

grad = Gradient.new(HexColor.new('0x000000'), HexColor.new('0xFFFFFF'))
ap(grad.colors)

len = grad.colors.length
height = 25 

img = Magick::Image.new len, height

grad.colors.each.with_index { |col, n|
  r, g, b = Convert::hex_to_rgb col
  pix = Magick::Pixel.new *([r, g, b].map { |n| n << 8 })

  (0..height).each { |h| 
    img.pixel_color n, h, pix
  }
}

img.write('test.jpg')
File.open('colors.txt', 'wb') { |f| 
  grad.colors.each { |c|
    hr, hg, hb = c.rgb
    f.puts "#{hr}, #{hg}, #{hb},"
  }
}

