# Introduction #

This page describes using the library as it is with its currently supported devices and how to add support for another device.

# Connections #

![http://farm5.static.flickr.com/4087/5225072558_5f5f760037.jpg](http://farm5.static.flickr.com/4087/5225072558_5f5f760037.jpg)

SYNC is on _OCR1A_ and AUDIO is on _OC2A_
| **MCU** | **SYNC** | **VIDEO** | **AUDIO** | **Arduino** | **SYNC** | **VIDEO** | **AUDIO** |
|:--------|:---------|:----------|:----------|:------------|:---------|:----------|:----------|
|m168,m328|B 1       |D 7        |B 3        |NG,Decimila,UNO|9         |7          |11         |
|m1280,m2560|B 5       |A 7        |B 4        |Mega         |11        |A7(D29)    |10         |
|m644,m1284p|D 5       |A 7        |D 7        |sanguino     |13        |A7(D24)    |8          |
|AT90USB1286|B 5       |F 7        |B 4        |--           |--        |--         |--         |
**There are some timing issues with the m1284p, may be related to sanguino core.**

# Adding Additional AVR Devices #