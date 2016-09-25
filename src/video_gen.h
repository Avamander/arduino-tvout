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

#ifndef VIDEO_GEN_H
#define VIDEO_GEN_H

#define	NTSC					0
#define _NTSC					0
#define PAL						1
#define _PAL					1
#define OVERLAY					2
#define _OVERLAY				2

typedef struct {
	volatile int scanLine;
	volatile int vsyncScanLine;
	volatile unsigned long frames;
	int first_frame_start_render_line;
	int first_frame_end_render_line;
	int second_frame_start_render_line;
	int second_frame_end_render_line;
	int lines_frame; 		//remove me
	uint8_t vres;
	uint8_t hres;
	uint16_t size;
	uint8_t output_delay; 	//remove me
	char vscale_const;		//combine me with status switch
	char vscale;			//combine me too.
	uint8_t * screen;
} TVout_vid;

extern TVout_vid display;

extern void (*hbi_hook)();
extern void (*vbi_hook)();

void render_setup(uint8_t mode, uint8_t x, uint8_t y, uint8_t *scrnptr);

void first_frame_vsync_lines();
void first_frame_blank_line();
void first_frame_active_line();
void second_frame_vsync_lines();
void second_frame_blank_line();
void second_frame_active_line();
void overlay_blank_line();
void overlay_active_line();
void empty();

//tone generation properties
extern volatile long remainingToneVsyncs;

// 6cycles functions
void render_line6c();
void render_line5c();
void render_line4c();
void render_line3c();
#endif
