/*
 Copyright (c) 2010 Myles Metzer

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 This File contains the timing definitions for the TVout AVR composite video
 generation Library
*/
#ifndef VIDEO_TIMING_H
#define	VIDEO_TIMING_H

#define _CYCLES_PER_US			(F_CPU / 1000000)



/*
 Analog Video Vertical Sync (NTSC): https://www.youtube.com/watch?v=NY2rIjkH1Xw
 PAL video timing specification: http://martin.hinner.info/vga/pal.html
 Field Synchronization of PAL System: http://martin.hinner.info/vga/pal.gif
 Field Synchronization of NTSC System: http://martin.hinner.info/vga/ntsc.gif
*/

// Timing settings for NTSC
#define _NTSC_LINE_FULL_FRAME		525		// lines
#define _NTSC_LINE_FRAME			(_NTSC_LINE_FULL_FRAME / 2)		// 262 lines

#define _NTSC_TIME_RENDERING_LINE	46		// µs
#define _NTSC_LINE_DISPLAY			216		// lines
#define _NTSC_LINE_MID				((_NTSC_LINE_FRAME - _NTSC_LINE_DISPLAY) / 2 + _NTSC_LINE_DISPLAY / 2) // lines

#define _NTSC_TIME_SCANLINE			63.563	// 63.5 µs
#define _NTSC_TIME_HSYNC_PULSE		4.7		// 4.7 µs
#define _NTSC_TIME_OUTPUT_START		12.5	// 10.9 + 0.5 µs + ...

#define _NTSC_CYCLES_SCANLINE		((_NTSC_TIME_SCANLINE * _CYCLES_PER_US) - 1)
#define _NTSC_CYCLES_HSYNC_PULSE	((_NTSC_TIME_HSYNC_PULSE * _CYCLES_PER_US) - 1)
#define _NTSC_CYCLES_OUTPUT_START	((_NTSC_TIME_OUTPUT_START * _CYCLES_PER_US) - 1)

#define _NTSC_TIME_VSYNC_SCANLINE				(_NTSC_TIME_SCANLINE / 2)	// µs
#define _NTSC_TIME_VSYNC_PRE_EQUALIZING_PULSE	2.3		// 2.3 µs
#define _NTSC_TIME_VSYNC_PULSE					27.1	// 27.1 µs

#define _NTSC_CYCLES_VSYNC_SCANLINE				((_NTSC_TIME_VSYNC_SCANLINE * _CYCLES_PER_US) - 1)
#define _NTSC_CYCLES_VSYNC_EQUALIZING_PULSE		((_NTSC_TIME_VSYNC_PRE_EQUALIZING_PULSE * _CYCLES_PER_US) - 1)
#define _NTSC_CYCLES_VSYNC_PULSE				((_NTSC_TIME_VSYNC_PULSE * _CYCLES_PER_US) - 1)

#define _NTSC_LINE_FIRSTFRAME_END		263		// 262 line#
#define _NTSC_LINE_SECONDFRAME_START	272		// 271 line#

#define _NTSC_LINE_SECONDFRAME_END		525		// 525 line#
#define _NTSC_LINE_FIRSTFRAME_START		10		// 9 line#



// Timing settings for PAL
#define _PAL_LINE_FULL_FRAME		625		// lines
#define _PAL_LINE_FRAME				312		// 312 lines

#define _PAL_TIME_RENDERING_LINE	46		// µs
#define _PAL_LINE_DISPLAY			260		// lines
#define _PAL_LINE_MID				((_PAL_LINE_FRAME - _PAL_LINE_DISPLAY) / 2 + _PAL_LINE_DISPLAY / 2) // lines

#define _PAL_TIME_SCANLINE			64		// 64 µs
#define _PAL_TIME_HSYNC_PULSE		4.7		// 4.7 µs
#define _PAL_TIME_OUTPUT_START		12.5	// 12.05 + 0.45 µs

#define _PAL_CYCLES_SCANLINE		((_PAL_TIME_SCANLINE * _CYCLES_PER_US) - 1)
#define _PAL_CYCLES_HSYNC_PULSE		((_PAL_TIME_HSYNC_PULSE * _CYCLES_PER_US) - 1)
#define _PAL_CYCLES_OUTPUT_START	((_PAL_TIME_OUTPUT_START * _CYCLES_PER_US) - 1)

#define _PAL_TIME_VSYNC_SCANLINE				(_PAL_TIME_SCANLINE / 2)	// µs
#define _PAL_TIME_VSYNC_PRE_EQUALIZING_PULSE	2.38		// 2.35 µs
#define _PAL_TIME_VSYNC_PULSE					27.32	// 27.3 µs

#define _PAL_CYCLES_VSYNC_SCANLINE				((_PAL_TIME_VSYNC_SCANLINE * _CYCLES_PER_US) - 1)
#define _PAL_CYCLES_VSYNC_EQUALIZING_PULSE		((_PAL_TIME_VSYNC_PRE_EQUALIZING_PULSE * _CYCLES_PER_US) - 1)
#define _PAL_CYCLES_VSYNC_PULSE					((_PAL_TIME_VSYNC_PULSE * _CYCLES_PER_US) - 1)

#define _PAL_LINE_FIRSTFRAME_END				311		// line# 310
#define _PAL_LINE_SECONDFRAME_START				318		// line# 317

#define _PAL_LINE_SECONDFRAME_END				624		// line# 622
#define _PAL_LINE_FIRSTFRAME_START				6		// line# 5

#endif