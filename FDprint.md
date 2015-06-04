# Required Setup #

To use any of the printing functions [select\_font(font)](FDselectfont.md) must be called with one of the following included fonts or a custom user font set.
  * font4x6
  * font6x8
  * font8x8
  * font8x8ext
To create a custom font see the [Fonts](Fonts.md) for instructions on how to create fonts.


# Details #

This function has several different forms all covered in the use section.  There is also a locationally aware version and a automatic version.

When using these functions the base and precision do not have to be specified.

# Use #
print(x,y,string)
  * Arguments:
    * _uint8\_t_ x:
      * The x location to print at.
    * _uint8\_t_ y:
      * The y location to print at.
    * _const char `*`_ string:
      * The string to be printed.
print(x,y,char,base)
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
print(x,y,int,base)
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
print(x,y,double,precision)
  * Arguments:
    * _uint8\_t_ x:
      * The x location to print at.
    * _uint8\_t_ y:
      * The y location to print at.
    * _double_ double:
      * The data to be printed.
    * _int_ precision:
      * The number of digits to print past the decimal place (default = 2)
print(string)
  * Arguments:
    * _const char `*`_ string:
      * The string to be printed.
print(char,base)
  * Arguments:
    * _char_ **or** _unsigned char_ char:
      * The data to print.
    * _int_ base:
      * The base to print in:
        * BYTE = 0 (default)
        * DEC = 10
        * HEX = 16
print(int,base)
  * Arguments:
    * _int_ **or** _unsigned int_ **or** _long_ **or** _unsigned long_ int:
      * The data to print.
    * _int_ base:
      * The base to print in:
        * BYTE = 0
        * DEC = 10 (default)
        * HEX = 16
print(double,precision)
  * Arguments:
    * _double_ double:
      * The data to be printed.
    * _int_ precision:
      * The number of digits to print past the decimal place (default = 2)