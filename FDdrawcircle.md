# Details #

Draw a circle with the center at the given point and a the given radius. The circle can also be filled with a color if one is specified.

# Use #

draw\_circle(x,y,radius,color,fillcolor) **or** draw\_circle(x,y,radius,color,fillcolor)
  * Arguments:
    * _uint8\_t_ x0:
      * The x coordinate of upper left corner of the rectangle.
    * _uint8\_t_ y0:
      * The y coordinate of upper left corner of the rectangle.
    * _uint8\_t_ radius:
      * The radius of the circle.
    * _char_ color:
      * The color of the rectangle.
        * WHITE = 1
        * BLACK = 0
        * INVERT = 2
    * _char_ fillcolor:
      * The fill color of the rectangle.
        * WHITE = 1
        * BLACK = 0
        * INVERT = 2
        * NOFILL = -1 (default)