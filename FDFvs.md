# Details #

This function will override the automatically chosen vertical scale. Choosing a lower vertical scale will allow the program more time to execute at the expense of filling the screen.  Be warned though choosing a value to large will break the output.

# Use #

force\_vscale(sfactor)
  * Arguments:
    * _char_ sfactor:
      * The scale number of times to repeate each line. This must be greater than or equal to 1. A value to large will break the library and there are no checks with this.