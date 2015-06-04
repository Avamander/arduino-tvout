# Required Setup #

To use any of the printing functions [select\_font(font)](FDselectfont.md) must be called with one of the following included fonts or a custom user font set.
  * font4x6
  * font6x8
  * font8x8
  * font8x8ext
To create a custom font see the [Fonts](Fonts.md) for instructions on how to create fonts.

When using a locationally aware print function ie print(10,10,"Hello World) the cursor will be left at the end of the print call.  So to print the string "My double=" at (10,10) the following can be done:
```
tv.print(10,10,"My double=");
tv.print(somedouble);
```
Assuming that tv is the instantiated TVout object.


# Brief Overview #

  * [print\_char(x,y,c)](FDprintchar.md) Print a character at (x,y).
  * [set\_cursor(x,y)](FDsetcursor.md) Set the position that the next character will be printed at.
  * [select\_font(font)](FDselectfont.md) Select the font to print.
  * [print()](FDprint.md) Print a string.
  * [println()](FDprintln.md) Print a line.
  * [printPGM()](FDprintPGM.md) Print a string from program memory.