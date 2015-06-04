# Details #

Set the function that is called once per vertical blanking period. This function MUST take no arguments and have a void return type.  This function also should only take about 62us to complete in its worst case.

# Use #

set\_vbi\_hook(&func)
  * Arguments:
    * func:
      * The function to call.