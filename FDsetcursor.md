# Required Setup #

To use any of the printing functions [select\_font(font)](FDselectfont.md) must be called with one of the following included fonts or a custom user font set.
  * font4x6
  * font6x8
  * font8x8
  * font8x8ext
To create a custom font see the [Fonts](Fonts.md) for instructions on how to create fonts.


# Details #

Select the new cursor position.

# Use #
set\_cursor(x,y)
  * Arguments:
    * _uint8\_t_ x:
      * The new x coordinate of the cursor.
    * _uint8\_t_ y:
      * The new y coordinate of the cursor.