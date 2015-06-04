# Required Setup #

To use any of the printing functions [select\_font(font)](FDselectfont.md) must be called with one of the following included fonts or a custom user font set.
  * font4x6
  * font6x8
  * font8x8
  * font8x8ext
To create a custom font see the [Fonts](Fonts.md) for instructions on how to create fonts.


# Details #

These functions will all print out the specified data or string then go to the next viable text line with an x position of 0.

This function has several different forms all covered in the use section.  There is also a locationally aware version and a automatic version.

When using these functions the base and precision do not have to be specified.

# Use #
println(x,y,string)
  * Arguments:
    * _uint8\_t_ x:
      * The x location to print at.
    * _uint8\_t_ y:
      * The y location to print at.
    * _const char `*`_ string:
      * The string to be printed.
println(x,y,char,base)
  * Arguments:
    * _uint8\_t_ x:
      * The x location to print at.
    * _uint8\_t_ y:
      * The y location to print at.
    * _char_ **or** _unsigned char_ char:
      * The data to print.
    * _int_ base:
      * The base to print in:
        * BYTE = 0 (default)
        * DEC = 10
        * HEX = 16
println(x,y,int,base)
  * Arguments:
    * _uint8\_t_ x:
      * The x location to print at.
    * _uint8\_t_ y:
      * The y location to print at.
    * _int_ **or** _unsigned int_ **or** _long_ **or** _unsigned long_ int:
      * The data to print.
    * _int_ base:
      * The base to print in:
        * BYTE = 0
        * DEC = 10 (default)
        * HEX = 16
println(x,y,double,precision)
  * Arguments:
    * _uint8\_t_ x:
      * The x location to print at.
    * _uint8\_t_ y:
      * The y location to print at.
    * _double_ double:
      * The data to be printed.
    * _int_ precision:
      * The number of digits to print past the decimal place (default = 2)
println(string)
  * Arguments:
    * _const char `*`_ string:
      * The string to be printed.
println(char,base)
  * Arguments:
    * _char_ **or** _unsigned char_ char:
      * The data to print.
    * _int_ base:
      * The base to print in:
        * BYTE = 0 (default)
        * DEC = 10
        * HEX = 16
println(int,base)
  * Arguments:
    * _int_ **or** _unsigned int_ **or** _long_ **or** _unsigned long_ int:
      * The data to print.
    * _int_ base:
      * The base to print in:
        * BYTE = 0
        * DEC = 10 (default)
        * HEX = 16
println(double,precision)
  * Arguments:
    * _double_ double:
      * The data to be printed.
    * _int_ precision:
      * The number of digits to print past the decimal place (default = 2)