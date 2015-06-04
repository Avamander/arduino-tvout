# Details #

Set the function to be called once per scanline. This function should take no parameters and have a void return type. The function should be very fast taking at most a few us.

# Use #
set\_hbi\_hook(&func)
  * Arguments:
    * func:
      * The function to call.