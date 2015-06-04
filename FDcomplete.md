# Details #

The complete list of functions not broken down into sections.

  * [begin(mode)](FDbegind.md) Start Outputting at the default resolution of 128x96.
  * [begin(mode,x,y)](FDbegins.md) Start Outputting at a specified resolution.
  * [end()](FDend.md) Clean up the video data and disable interrupts.
  * [force\_vscale(sfactor)](FDFvs.md) Force the number of times to display each line.
  * [force\_outstart(time)](FDFos.md) Force the time to start outputting on an active line.
  * [force\_linestart(line)](FDFls.md) Force line to start outputting on.
  * [set\_vbi\_hook(func)](FDsetvbi.md) Set the function to be called once per vertical blanking period.
  * [set\_hbi\_hook(func)](FDsethbi.md) Set the function to be called once per horizontal blanking period.
  * [hres()](FDhres.md) Get the Horizontal Resolution.
  * [vres()](FDvres.md) Get the Vertical Resolution.
  * [char\_line()](FDcharl.md) Get the number of characters that will fit on a text line.
  * [set\_pixel(x,y,color)](FDsetp.md) Set a pixel.
  * [get\_pixel(x,y)](FDgetp.md) Get a pixel.
  * [fill(color)](FDfill.md) Fill the screen with a color.
  * [clear\_screen()](FDfill.md) Clear the screen.
  * [invert()](FDfill.md) Invert the data on the screen.
  * [shift(distance,direction)](FDshift.md) Shift the screen a specified distance in any direction.
  * [draw\_line(x0,y0,x1,y1,color)](FDdrawline.md) Draw a line from (x0,y0) to (x1,y1).
  * [draw\_row(row,x0,x1,color)](FDfillline.md) Fill the given row from x0 to x1 with the given color.
  * [draw\_column(column,y0,y1,color)](FDdrawcol.md) Fill the given column from y0 to y1 with the given color.
  * [draw\_rect(x,y,w,h,color,fillcolor)](FDdrawrect.md) Draw a rectangle at (x,y) by dimensions (h,w), and fill with the given color.
  * [draw\_rect(x,y,w,h,color)](FDdrawrect.md) Draw a rectangle at (x,y) by dimensions (h,w).
  * [draw\_circle(x,y,radius,color,fillcolor)](FDdrawcircle.md) Draw a Circle at (x,y) with the given radius, and fill with the given.
  * [draw\_circle(x,y,radius,color)](FDdrawcircle.md) Draw a Circle at (x,y) with the given radius.
  * [bitmap(x,y,bmp,i,width,height)](Bitmap.md) Place a bitmap at (x,y).
  * [print\_char(x,y,c)](FDprintchar.md) Print a character at (x,y).
  * [set\_cursor(x,y)](FDsetcursor.md) Set the position that the next character will be printed at.
  * [select\_font(font)](FDselectfont.md) Select the font to print.
  * [print()](FDprint.md) Print a string.
  * [println()](FDprintln.md) Print a line.
  * [printPGM()](FDprintPGM.md) Print a string from program memory.
  * [tone(frequency)](FDtone.md) Start outputting a tone.
  * [tone(frequency,duration)](FDtone.md) Start outputting a tone for a duration in ms.
  * [noTone()](FDnotone.md) Stop all tones.