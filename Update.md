# Introduction #

While working on getting the new version of TVout some poor initial design desicions became apparent. The most obvious being the video pin choice; to facilitate higher output resolutions this has changed to the highest pin on a port.  There have also been several other changes to make the library easier to use and consume less flash memory.

# Hardware Changes #

The video pin has changed to pin7 for atmega328 based devices and a7 for the arduino mega/sanguino.  For further details see the [hardware](Hardware.md) page of this wiki.  If you would like to revert this change comment out the following line in spec/hardware\_setup.h:
```
#define ENABLE_FAST_OUTPUT
```


# Fixing the Function Calls #

  * start\_render(mode) and start\_render(mode,x,y) are now [begin(mode)](FDbegind.md) and [begin(mode,x,y)](FDbegins.md)
    * This change was to keep the system in-line with other arduino libraries.
  * [select\_font()](FDselectfont.md) MUST be called prior to using any text as well as including the appropriate font.
    * This change was made to cut down the flash use by only including used fonts.
    * The new fonts included in fontAll.h are:
      * font4x6
      * font6x8
      * font8x8
      * font8x8ext
    * See [select\_font()](FDselectfont.md) for complete use description.
  * print\_str() is no longer avaliable use [print(x,y,string)](FDprint.md) instead, for terminal like printing use [print(string)](FDprint.md)).
  * delay() now takes the delay as ms not frames, use delay\_frame() frame delay
  * draw\_box() has been replaced:
    * [draw\_rect(x,y,w,h,color)](FDdrawrect.md)) for an empty rectangle.
    * [draw\_rect(x,y,w,h,color,fillcolor)](FDdrawrect.md) for a filled rectangle.
  * draw\_circle() has been modified the arguments have changed:
    * [draw\_circle(x,y,radius,color)](FDdrawcircle.md) for a circle with no fill.
    * [draw\_circle(x,y,radius,color,fillcolor)](FDdrawcircle.md) for a filled circle.
  * fs\_bitmap() was removed in favor of [bitmap(x,y,bmp)](Bitmap.md)


# Setup Comparison #
Here is an example of the the equivalent setup processes in the beta1.0+ libraries and the older releases:

Pre Beta1.0:
```
#include <TVout.h>

TVout TV;

void setup()  {
  TV.start_render(_NTSC);
}
```
Beta1.0:
```
#include <TVout.h>
#include <fontALLL.h>

TVout TV;

void setup()  {
  TV.begin(_NTSC);
  TV.select_font(font6x8);
}
```