# Arduino-TVout

# Hosted here to keep it available from Arduino IDE (and for simple download as the original Google Code has been archived).

This is a library for generating composite video on an ATmega microcontroller. 

This branch of the TVout library has been patched to allow use with the Arduino Leonardo. The goal of this project is to create a simple interupt driven library for generating composite video on a single AVR chip.

Currently the output is NTSC or PAL at a resolution of 128x96 by default. The library currently works on ATmega168,328,1280,2560,644p,1284p,32U4,AT90USB1286 and more can be added by editing spec/hardware_setup.h.

There are some timing issues with the m1284p, may be related to sanguino core.

MCU | SYNC | VIDEO | AUDIO | Arduino | SYNC | VIDEO | AUDIO
---|---|---|---|---|---|---|---
m168,m328 | B 1 | D 7 | B 3 | NG,Decimila,UNO | 9 | 7 | 11
m1280,m2560 | B 5 | A 7 | B 4 | Mega | 11 | A7(D29) | 10
m644,m1284p | D 5 | A 7 | D 7 | sanguino | 13 | A7(D24) | 8
m32u4 | B 5 | B 4 | B 7 | Leonardo | 9 | 8 | 11
AT90USB1286 | B 5 | F 7 | B 4 | -- | -- | -- | --

## Connections

SYNC is on OCR1A and AUDIO is on OCR2A (except on the Arduino Leonardo, where AUDIO is on OCR0A)

There are some timing issues with the m1284p, may be related to sanguino core.

On NG, Decimila, UNO and Nano the sync is pin 9, video on 7 and audio on 11. On Mega2560	sync is pin 11, video is on A7(D29)	and audio is on pin 10.


## Examples

https://youtu.be/MEg_V4YZDh0

https://youtu.be/bHpFv_x_8Kk


## Throubleshooting

Also always remember that this library could give problems to your arduino bootloader, expecially if it's not an original one and it uses a modified bootloader.

If the bootloader is damaged by the usage of the library(it happens some times) you need another arduino or eventually an RPi in order to reflash the bootloader on it and also you need of the bootloader.hex file that, if your arduino is original, you should be able to find in some folders of the Arduino IDE, otherwise you have to search the internet for it and hope it doesn't burn up your arduino.

Here I link a bunch of detailed tutorials I've used lately to save my arduinos:

- RPi - Arduino solution : https://learn.adafruit.com/program-an-avr-or-arduino-using-raspberry-pi-gpio-pins/programming

- Arduino - Arduino and other possible solutions : https://learn.sparkfun.com/tutorials/installing-an-arduino-bootloader/all
