# Arduino-TVout

This is a library for generating composite video on an ATmega microcontroller. Hosted here to keep it available from Arduino IDE (and for simple download as the original Google Code has been archived).

This branch of the TVout library has been patched to allow use with the Arduino Leonardo. The goal of this project is to create a simple interupt driven library for generating composite video on a single AVR chip.

Currently the output is NTSC or PAL at a resolution of 128x96 by default. The library currently works on ATmega168,328,1280,2560,644p,1284p,32U4,AT90USB1286 and more can be added by editing spec/hardware_setup.h.

There are some timing issues with the m1284p, may be related to sanguino core.

##Connections

MCU | SYNCOUT | AUDIOOUT | VIDEO | VSYNCIN | CSYNCIN
---|---|---|---|---|---
m168,m328 | PB1 | PB3 | PD7 | INT0 | ICP1
m1280,m2560 | B 5 | B 4 | A7 | INT0 | ICP1
m644,m1284p | D 5 | D 7 | A7 | |
m32u4 | B 5 | B 7 | B4 | |
AT90USB1286 | B 5 | B 4 | F7 | |

Arduino | SYNCOUT | AUDIOOUT | VIDEO | VSYNCIN | CSYNCIN
---|---|---|---|---|---
NG,Decimila,UNO | 9 | 11 | 7 | 2 | 8
Mega | 11 | 10 | A7(D29) | 21
sanguino | 13 | 8 | A7(D24)
Leonardo | 9 | 11 | 8
Nano | D9 | D11 | D7 | D2 | D8

SYNCOUT is on OCR1A and AUDIOOUT is on OCR2A (except on the Arduino Leonardo, where AUDIOOUT is on OCR0A)

There are some timing issues with the m1284p, may be related to sanguino core.

On NG, Decimila, UNO and Nano the SYNCOUT is pin 9, video on 7 and AUDIOOUT on 11.
On Mega2560	SYNCOUT is pin 11, video is on A7(D29) and AUDIOOUT is on pin 10.


##Examples

https://youtu.be/MEg_V4YZDh0

https://youtu.be/bHpFv_x_8Kk
