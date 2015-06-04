# Details #

**Note:** fill\_screen() and invert() both direct here as they are macros that make the calls fill(0) and fill(2) respectively.

Fill the screen with the specified color. The macros clear\_screen() and invert() call this function with the correct color.

# Use #
fill(color)
  * Arguments:
    * _uint\_t_ color:
      * The color to fill the screen with.
        * WHITE = 1
        * BLACK = 0
        * INVERT = 2