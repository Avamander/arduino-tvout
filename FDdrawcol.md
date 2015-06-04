# Details #

This will fill a column with the specified color from one y0 to y1. This function draws only vertical lines and does it very fast.

# Use #

draw\_column(column,y0,y1,color)
  * Argument:
    * _uint8\_t column:
      * The column that fill will be performed on.
    *_uint8\_t y0:
      * Edge 0 of the fill.
    * _uint8\_t y1:
      * Edge 1 of the fill.
    *_uint8\_t c:
      * The color of the fill.
        * WHITE = 1
        * BLACK = 0
        * INVERT = 2