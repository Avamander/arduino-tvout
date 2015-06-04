# Details #

Draw a rectangle at with its upper left corner at the given point with the given width and height.  The rectangle may be optionally filled if a fill color is passed given.

# Use #
draw\_rect(x0,y0,width,height,color,fillcolor) **or**
draw\_rect(x0,y0,width,height,color)
  * Arguments:
    * _uint8\_t_ x0:
      * The x coordinate of upper left corner of the rectangle.
    * _uint8\_t_ y0:
      * The y coordinate of upper left corner of the rectangle.
    * _uint8\_t_ width:
      * The widht of the rectangle.
    * _uint8\_t_ height:
      * The height of the rectangle.
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