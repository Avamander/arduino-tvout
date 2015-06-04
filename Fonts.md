# Introduction #

This wiki covers the creation and use of fonts.


# Defining a Font #

There are two kinds of fonts; fixed width and variable width.
  * **Fixed width fonts:** The first 3 bytes must contain the width, height, and first defined character followed by the font definitions.  For example the following is the header and first 2 characters from the 4x6 font definition.
```
4,6,32,
//space
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
//!
0b01000000,
0b01000000,
0b01000000,
0b00000000,
0b01000000,
0b00000000,
```

> There must be a full definition for each character; in this case the width of 4 fits into a single byte and the height of 6 means there needs to 6 bytes per character. Since 32 is the first printing character for this font set anything before character 32 will be ignored.

  * **Variable width fonts:** These fonts are defined by having a width of 0, height, and first defined character. An example of a variable width font with a height of 6 and the first printing character being a space.
```
0,6,32,
//space
2,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
//!
3,
0b01000000,
0b01000000,
0b01000000,
0b00000000,
0b01000000,
0b00000000,
```