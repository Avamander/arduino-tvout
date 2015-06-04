# Details #

This will fill a line with the specified color from one x0 to x1. This function draws only horizontal lines and does it very fast, it is best for filling primitive shapes with any color.

# Use #

draw\_row(line,x0,x1,color)
  * Argument:
    * _uint8\_t line:
      * The row that fill will be performed on.
    *_uint8\_t x0:
      * Edge 0 of the fill.
    * _uint8\_t x1:
      * Edge 1 of the fill.
    *_uint8\_t c:
      * The color of the fill.
        * WHITE = 1
        * BLACK = 0
        * INVERT = 2