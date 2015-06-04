# Required Setup #

To use any of the printing functions [select\_font(font)](FDselectfont.md) must be called with one of the following included fonts or a custom user font set.
  * font4x6
  * font6x8
  * font8x8
  * font8x8ext
To create a custom font see the [Fonts](Fonts.md) for instructions on how to create fonts.


# Details #

Print a character at given coordinate.

# Use #

print\_char(x,y,c)
  * Arguments:
    * _uint8\_t_ x:
      * The x coordinate that the character will be printed at.
    * _uint8\_t_ y:
      * The y coordinate that the character will be printed at.
    * _unsigned char_ c:
      * The character to print.