# Brief Overview #

  * [begin(mode)](FDbegind.md) Start Outputting at the default resolution of 128x96.
  * [begin(mode,x,y)](FDbegins.md) Start Outputting at a specified resolution.
  * [end()](FDend.md) Clean up the video data and disable interrupts.
  * [force\_vscale(sfactor)](FDFvs.md) Force the number of times to display each line.
  * [force\_outstart(time)](FDFos.md) Force the time to start outputting on an active line.
  * [force\_linestart(line)](FDFls.md) Force line to start outputting on.
  * [set\_vbi\_hook(func)](FDsetvbi.md) Set the function to be called once per vertical blanking period.
  * [set\_hbi\_hook(func)](FDsethbi.md) Set the function to be called once per horizontal blanking period.