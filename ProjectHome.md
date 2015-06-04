The goal of this project is to create a simple interupt driven library for generating composite video on a single AVR chip.

Currently the output is  NTSC or PAL at a resolution of 128x96 by default.
The library currently works on ATmega168,328,1280,2560,644p,1284p,AT90USB1286 and more can be added by editing one file.

# News #
Beta1 for version 1.0 is out!!! It has some it changes the hardware connections as well as some of the function calls see the wiki for full details.

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


# Products Based on TVout #
Wayne and Layn LLC now makes a shield that uses this library, the
[video game shield](http://wayneandlayne.com/projects/video-game-shield/games/).


nootropic design is selling a stand alone arduino compatible board the
[hackvision](http://nootropicdesign.com/hackvision/).


# Included Examples #
Very simple as they may be.

<a href='http://www.youtube.com/watch?feature=player_embedded&v=MEg_V4YZDh0' target='_blank'><img src='http://img.youtube.com/vi/MEg_V4YZDh0/0.jpg' width='425' height=344 /></a>
<a href='http://www.youtube.com/watch?feature=player_embedded&v=bHpFv_x_8Kk' target='_blank'><img src='http://img.youtube.com/vi/bHpFv_x_8Kk/0.jpg' width='425' height=344 /></a>
