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
 This library provides a simple method for outputting data to a tv
 from a frame buffer stored in sram.  This implementation is done
 completly by interupt and will return give as much cpu time to the
 application as possible.
 
 Limitations:
  -	Currently only works with NTSC. PAl support would require modifying
	the timings in video_gen.h as well as the OCR1A ISR to allow for a 16bit
	line counter.
  -	each frame only consists of 256 scanlines vs 262 for the fake progressive
	signal this is supposed to generate (works fine on my tv).
  - virtical sync does not match the specs at all.
 

 current hardware setup:
 Pin9: Sync line:  -->|--/\/\/\--o
   Diode 1Kohm   |
 Pin8: Video line: -->|--/\/\/\--o---------------- RCA tip
   Diode 330ohm  |
 o--/--/\/\/\--o-- RCA GND
							 Switch  75ohm  |
									V
											   GND
											   
  Audio connected to arduino pin 10, hard coded for now
*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "TVout.h"
#include "video_gen.h"
#include "spec/hardware_setup.h"
#include "spec/video_properties.h"


/* call this to start video output with the default resolution.
 * returns 4 if not enough memory.
 */
char TVout::begin(uint8_t mode) {
		
	return begin(mode,128,96);
}


/* call this to start video output with a specified resolution.
 * mode selects NTSC or PAL
 * x is the horizontal resolution MUST BE DIVISABLE BY 8 (max 240)
 * y is the vertical resolution (max 255)
 */
char TVout::begin(uint8_t mode, uint8_t x, uint8_t y) {
	
	// check if x is divisable by 8
	if ( !(x & 0xF8))
		return 1;
	x = x/8;
		
	screen = (unsigned char*)malloc(x * y * sizeof(unsigned char));
	if (screen == NULL)
		return 4;
		
	cursor_x = 0;
	cursor_y = 0;
	
	render_setup(mode,x,y,screen);
	clear_screen();
	return 0;
}


/* Clears the screen
*/
void TVout::fill(uint8_t color) {
	switch(color) {
		case BLACK:
			cursor_x = 0;
			cursor_y = 0;
			for (int i = 0; i < (display.hres)*display.vres; i++)
				display.screen[i] = 0;
			break;
		case WHITE:
			cursor_x = 0;
			cursor_y = 0;
			for (int i = 0; i < (display.hres)*display.vres; i++)
				display.screen[i] = 0xFF;
			break;
		case INVERT:
			for (int i = 0; i < display.hres*display.vres; i++)
				display.screen[i] = ~display.screen[i];
			break;
	}
}


/* Gets the Horizontal resolution of the screen
*/
unsigned char TVout::hres() {
	return display.hres*8;
}


/* Gets the Vertical resolution of the screen
*/
unsigned char TVout::vres() {
	return display.vres;
}


/* Return the number of characters that will fit on a line
*/
char TVout::char_line() {
	return ((display.hres*8)/pgm_read_byte(font));
}


void TVout::delay(unsigned int x) {
	unsigned long time = millis() + x;
	while(millis() < time);
}


/* Delay for x frames
 * for NTSC 1 second = 60 frames
 * for PAL 1 second = 50 frames
 * exits at the end of the last display line always
 * delay_frame(1) is useful prior to drawing so there is little/no flicker.
 */
void TVout::delay_frame(unsigned int x) {
	while (x) {
		while (display.scanLine != display.stop_render+1);
		while (display.scanLine == display.stop_render+1);
		x--;
	}
}


unsigned long TVout::millis() {
	if (display.lines_frame == _NTSC_LINE_FRAME) {
		return display.frames * _NTSC_TIME_SCANLINE * _NTSC_LINE_FRAME / 1000;
	}
	else {
		return display.frames * _PAL_TIME_SCANLINE * _PAL_LINE_FRAME / 1000;
	}
}


/* plot one point 
 * at x,y with color 1=white 0=black 2=invert
 */
void TVout::set_pixel(uint8_t x, uint8_t y, char c) {
	if (x >= display.hres*8 || y >= display.vres)
		return;
	sp(x,y,c);
}


/* Returns the value of pixel (x,y)
 * 0 if pixel is black
 * !0 if pixel is white
 * Thank you gijs on the arduino.cc forum for the non obviouse fix.
*/
unsigned char TVout::get_pixel(uint8_t x, uint8_t y) {
	if (x >= display.hres*8 || y >= display.vres)
		return 0;
	if (display.screen[x/8+y*display.hres] & (0x80 >>(x&7)))
		return 1;
	return 0;
}


/* draw a line
 * x1,y1 to x2,y2
 * with color 1 = white, 0=black, 2=invert
 */
void TVout::draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, char c) {

	if (x0 > display.hres*8 || y0 > display.vres || x1 > display.hres*8 || y1 > display.vres)
		return;
	int e;
	signed int dx,dy,j, temp;
	signed char s1,s2, xchange;
	signed int x,y;

	x = x0;
	y = y0;
	
	//take absolute value
	if (x1 < x0) {
		dx = x0 - x1;
		s1 = -1;
	}
	else if (x1 == x0) {
		dx = 0;
		s1 = 0;
	}
	else {
		dx = x1 - x0;
		s1 = 1;
	}

	if (y1 < y0) {
		dy = y0 - y1;
		s2 = -1;
	}
	else if (y1 == y0) {
		dy = 0;
		s2 = 0;
	}
	else {
		dy = y1 - y0;
		s2 = 1;
	}

	xchange = 0;   

	if (dy>dx) {
		temp = dx;
		dx = dy;
		dy = temp;
		xchange = 1;
	} 

	e = ((int)dy<<1) - dx;  
	 
	for (j=0; j<=dx; j++) {
		sp(x,y,c);
		 
		if (e>=0) {
			if (xchange==1) x = x + s1;
			else y = y + s2;
			e = e - ((int)dx<<1);
		}
		if (xchange==1)
			y = y + s2;
		else
			x = x + s1;
		e = e + ((int)dy<<1);
	}
}


void TVout::fill_line(uint8_t line, uint16_t x0, uint16_t x1, uint8_t c) {
	uint8_t lbit, rbit;
	
	if (x0 == x1)
		set_pixel(x0,line,c);
	else {
		if (x0 > x1) {
			lbit = x0;
			x0 = x1;
			x1 = lbit;
		}
		lbit = 0xff >> (x0&7);
		x0 = x0/8 + display.hres*line;
		rbit = ~(0xff >> (x1&7));
		x1 = x1/8 + display.hres*line;
		if (x0 == x1) {
			lbit = lbit & rbit;
			rbit = 0;
		}
		if (c == WHITE) {
			screen[x0++] |= lbit;
			while (x0 < x1)
				screen[x0++] = 0xff;
			screen[x0] |= rbit;
		}
		else if (c == BLACK) {
			screen[x0++] &= ~lbit;
			while (x0 < x1)
				screen[x0++] = 0;
			screen[x0] &= ~rbit;
		}
		else if (c == INVERT) {
			screen[x0++] ^= lbit;
			while (x0 < x1)
				screen[x0++] ^= 0xff;
			screen[x0] ^= rbit;
		}
	}
}


void TVout::draw_rect(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, char c, char fc) {
	
	if (fc != -1) {
		for (unsigned char i = y0; i < y0+h; i++)
			//draw_line(x0,i,x0+w,i,fc);
			fill_line(i,x0,x0+w,fc);
	}
	draw_line(x0,y0,x0+w,y0,c);
	draw_line(x0,y0,x0,y0+h,c);
	draw_line(x0+w,y0,x0+w,y0+h,c);
	draw_line(x0,y0+h,x0+w,y0+h,c);
}


/* draw a circle
 * x0,y0 around radius
 * with color 1 = white, 0=black, 2=invert
 * with fill 0 = black fill, 1 = white fill, 2 = invert fill, -1/blank = nofill
 */
void TVout::draw_circle(uint8_t x0, uint8_t y0, uint8_t radius, char c, char fc) {

	int f = 1 - radius;
	int ddF_x = 1;
	int	ddF_y = -2 * radius;
	int x = 0;
	int y = radius;
	
	//there is a fill color
	if (fc != -1)
		//draw_line(x0-radius,y0,x0+radius,y0,fc);
		fill_line(y0,x0-radius,x0+radius,fc);
	
	sp(x0, y0 + radius,c);
	sp(x0, y0 - radius,c);
	sp(x0 + radius, y0,c);
	sp(x0 - radius, y0,c);
	
	while(x < y) {
		if(f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (fc != -1) {	//there is a fill color
			//draw_line(x0-x,y0+y,x0+x,y0+y,fc);
			fill_line(y0+y,x0-x,x0+x,fc);
			//draw_line(x0-x,y0-y,x0+x,y0-y,fc);
			fill_line(y0-y,x0-x,x0+x,fc);
			//draw_line(x0-y,y0+x,x0+y,y0+x,fc);
			fill_line(y0+x,x0-y,x0+y,fc);
			//draw_line(x0-y,y0-x,x0+y,y0-x,fc);
			fill_line(y0-x,x0-y,x0+y,fc);
		}
		sp(x0 + x, y0 + y,c);
		sp(x0 - x, y0 + y,c);
		sp(x0 + x, y0 - y,c);
		sp(x0 - x, y0 - y,c);
		sp(x0 + y, y0 + x,c);
		sp(x0 - y, y0 + x,c);
		sp(x0 + y, y0 - x,c);
		sp(x0 - y, y0 - x,c);
	}
}


void TVout::bitmap(uint8_t x, uint8_t y, const unsigned char * bmp,
				   uint16_t i, uint8_t width, uint8_t lines) {

	uint8_t temp, lshift, rshift, save, xtra;
	uint16_t si = 0;
	
	rshift = x&7;
	lshift = 8-rshift;
	if (width == 0) {
		width = pgm_read_byte((uint32_t)(bmp) + i);
		i++;
	}
	if (lines == 0) {
		lines = pgm_read_byte((uint32_t)(bmp) + i);
		i++;
	}
		
	if (width&7) {
		xtra = width&7;
		width = width/8;
		width++;
	}
	else {
		xtra = 8;
		width = width/8;
	}
	
	for (uint8_t l = 0; l < lines; l++) {
		si = (y + l)*display.hres + x/8;
		if (width == 1)
			temp = 0xff >> rshift + xtra;
		else
			temp = 0;
		save = screen[si];
		screen[si] &= ((0xff << lshift) | temp);
		temp = pgm_read_byte((uint32_t)(bmp) + i++);
		screen[si++] |= temp >> rshift;
		for ( uint16_t b = i + width-1; i < b; i++) {
			save = screen[si];
			screen[si] = temp << lshift;
			temp = pgm_read_byte((uint32_t)(bmp) + i);
			screen[si++] |= temp >> rshift;
		}
		if (rshift + xtra < 8)
			screen[si-1] |= (save & (0xff >> rshift + xtra));	//test me!!!
		if (rshift + xtra - 8 > 0)
			screen[si] &= (0xff >> rshift + xtra - 8);
		screen[si] |= temp << lshift;
	}
}


void TVout::shift(uint8_t distance, uint8_t direction) {
	uint8_t * src;
	uint8_t * dst;
	uint8_t * end;
	uint8_t shift;
	uint8_t tmp;
	switch(direction) {
		case UP:
			dst = display.screen;
			src = display.screen + distance*display.hres;
			end = display.screen + display.vres*display.hres;
				
			while (src <= end) {
				*dst = *src;
				*src = 0;
				dst++;
				src++;
			}
			break;
		case DOWN:
			dst = display.screen + display.vres*display.hres;
			src = dst - distance*display.hres;
			end = display.screen;
				
			while (src >= end) {
				*dst = *src;
				*src = 0;
				dst--;
				src--;
			}
			break;
		case LEFT:
			shift = distance & 7;
			
			for (uint8_t line = 0; line < display.vres; line++) {
				dst = display.screen + display.hres*line;
				src = dst + distance/8;
				end = dst + display.hres-2;
				while (src <= end) {
					tmp = 0;
					tmp = *src << shift;
					*src = 0;
					src++;
					tmp |= *src >> (8 - shift);
					*dst = tmp;
					dst++;
				}
				tmp = 0;
				tmp = *src << shift;
				*src = 0;
				*dst = tmp;
			}
			break;
		case RIGHT:
			shift = distance & 7;
			
			for (uint8_t line = 0; line < display.vres; line++) {
				dst = display.screen + display.hres-1 + display.hres*line;
				src = dst - distance/8;
				end = dst - display.hres+2;
				while (src >= end) {
					tmp = 0;
					tmp = *src >> shift;
					*src = 0;
					src--;
					tmp |= *src << (8 - shift);
					*dst = tmp;
					dst--;
				}
				tmp = 0;
				tmp = *src >> shift;
				*src = 0;
				*dst = tmp;
			}
			break;
	}
}


void TVout::inc_txtline() {
	if (cursor_y >= (display.vres - pgm_read_byte(font+1)))
		shift(pgm_read_byte(font+1),UP);
	else
		cursor_y += pgm_read_byte(font+1);
}


/*
 * print an 8x8 char c at x,y
 * x must be a multiple of 8
 */
void TVout::print_char(uint8_t x, uint8_t y, unsigned char c) {

	c -= pgm_read_byte(font+2);
	bitmap(x,y,font,(c*pgm_read_byte(font+1))+3,pgm_read_byte(font),pgm_read_byte(font+1));
}


/* Inline version of set_pixel that does not perform a bounds check
*/
static void inline sp(uint8_t x, uint8_t y, char c) {
	if (c==1)
		display.screen[(x/8) + (y*display.hres)] |= 0x80 >> (x&7);
	else if (c==0)
		display.screen[(x/8) + (y*display.hres)] &= ~0x80 >> (x&7);
	else
		display.screen[(x/8) + (y*display.hres)] ^= 0x80 >> (x&7);
}


void TVout::set_vbi_hook(void (*func)(), char n) {
	if (n == 0)
		vbi_hook0 = func;
	else if (n == 1)
		vbi_hook1 = func;
}


void TVout::set_hbi_hook(void (*func)()) {
	hbi_hook = func;
}


void TVout::tone(unsigned int frequency) {
	tone(frequency, 0);
}


/* Simple tone generation
 * Takes the frequency and duration in ms
 * courtesy of adamwwolf
 */
void TVout::tone(unsigned int frequency, unsigned long duration_ms) {

	if (frequency == 0)
		return;

#define TIMER 2
	//this is init code
	TCCR2A = 0;
	TCCR2B = 0;
	TCCR2A |= _BV(WGM21);
	TCCR2B |= _BV(CS20);
	//end init code

	//most of this is taken from Tone.cpp from Arduino
	uint8_t prescalarbits = 0b001;
	uint32_t ocr = 0;
  

    DDR_SND |= _BV(SND_PIN); //set pb3 (digital pin 11) to output

    //we are using an 8 bit timer, scan through prescalars to find the best fit
	ocr = F_CPU / frequency / 2 - 1;
    prescalarbits = 0b001;  // ck/1: same for both timers
    if (ocr > 255) {
        ocr = F_CPU / frequency / 2 / 8 - 1;
        prescalarbits = 0b010;  // ck/8: same for both timers

        if (ocr > 255) {
			ocr = F_CPU / frequency / 2 / 32 - 1;
			prescalarbits = 0b011;
        }

        if (ocr > 255) {
			ocr = F_CPU / frequency / 2 / 64 - 1;
			prescalarbits = TIMER == 0 ? 0b011 : 0b100;
			if (ocr > 255) {
				ocr = F_CPU / frequency / 2 / 128 - 1;
				prescalarbits = 0b101;
			}

			if (ocr > 255) {
				ocr = F_CPU / frequency / 2 / 256 - 1;
				prescalarbits = TIMER == 0 ? 0b100 : 0b110;
				if (ocr > 255) {
					// can't do any better than /1024
					ocr = F_CPU / frequency / 2 / 1024 - 1;
					prescalarbits = TIMER == 0 ? 0b101 : 0b111;
				}
			}
        }
    }
    TCCR2B = prescalarbits;

	if (duration_ms > 0)
		remainingToneVsyncs = duration_ms*60/1000; //60 here represents the framerate
	else
		remainingToneVsyncs = -1;
 
    // Set the OCR for the given timer,
    OCR2A = ocr;
    //set it to toggle the pin by itself
    TCCR2A &= ~(_BV(COM2A1)); //set COM2A1 to 0
    TCCR2A |= _BV(COM2A0);
}


void TVout::noTone() {
	TCCR2B = 0;
	PORT_SND &= ~(_BV(SND_PIN)); //set pin 11 to 0
}