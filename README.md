# Arduino-TVout

This is a library for generating composite video on an ATmega microcontroller.

The library can create NTSC or PAL at a resolution of 128x96. The library currently works on ATmega168, ATmega328, ATmega1280, ATmega2560, ATmega644P, ATmega1284P, ATmega90USB1286 and more can be added by editing one file.



##Connections

SYNC is on OCR1A and AUDIO is on OCR2A

There are some timing issues with the m1284p, may be related to sanguino core.

On NG, Decimila, UNO and Nano the sync is pin 9, video on 7 and audio on 11. On Mega2560	sync is pin 11, video is on A7(D29)	and audio is on pin 10.




##Examples

https://youtu.be/MEg_V4YZDh0

https://youtu.be/bHpFv_x_8Kk
