# Details #

This Function will start generating a video signal using the specified standard(NTSC or PAL). This function uses the default resolution of 128x96.

# Use #

begin(mode)
  * Arguments:
    * _uint8\_t_ mode:
      * The video standard to follow:
        * `PAL	=1	=_PAL`
        * `NTSC	=0	=_NTSC`
  * Returns:
    * _char_ Error Code:
      * 0 if no error.
      * 4 if there is not enough memory for the frame buffer.