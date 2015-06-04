# Details #

This Function will start generating a video signal using the specified standard(NTSC or PAL).

# Use #

begin(mode,x,y)
  * Arguments:
    * _uint8\_t_ mode:
      * The video standard to follow:
        * `PAL	=1	=_PAL`
        * `NTSC	=0	=_NTSC`
    * _uint8\_t_ x:
      * Horizontal resolution, this must be a multiple of 8.
    * _uint8\_t_ y:
      * Vertical resolution.
  * Returns:
    * _char_ Error Code:
      * 0 if no error.
      * 1 if x is not a multiple of 8.
      * 2 if y is to large (NTSC only, 8bit vertical resolutions cannot be to large for PAL)
      * 4 if there is not enough memory for the frame buffer.