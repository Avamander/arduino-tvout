# Required Setup #

To use any of the printing functions [select\_font(font)](FDselectfont.md) must be called with one of the following included fonts or a custom user font set.
  * font4x6
  * font6x8
  * font8x8
  * font8x8ext
To create a custom font see the [Fonts](Fonts.md) for instructions on how to create fonts.


# Details #

These functions will print a string directly from the flash memory. This can be done and should be done when printing a constant string and memory is low. A string may be placed into the flash space by using this function PSTR(string) for instance: printPGM(PSTR("Hello World"))

This function has two separate forms, one that is locationally aware and one that is not both of which are covered in the Use section.

# Use #

printPGM(x,y,string)
  * Arguments:
    * _uint8\_t_ x:
      * The x location to print at.
    * _uint8\_t_ y:
      * The y location to print at.
    * _const char `*`_ string:
      * The string to be printed.
printPGM(x,y,string)
  * Arguments:
    * _const char `*`_ string:
      * The string to be printed.