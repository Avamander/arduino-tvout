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
#include "video_properties.h"
#include "hardware_setup.h"

PROGMEM const unsigned char ascii3x5[] ={
#include "ascii3x5.h"
};

PROGMEM const unsigned char ascii5x7[] = {
#include "ascii5x7.h"
};

PROGMEM const unsigned char ascii8x8[] = {
#include "ascii8x8.h"
};

PROGMEM const unsigned char bitmap[] = {
#include "bitmap.h"
};

/* call this to start video output with the default resolution.
 * returns 4 if not enough memory.
 */
char TVout::begin(uint8_t mode) {
		
	display.vscale = 1;
	display.vscale_const = 1;
	display.vres = 96;
	display.hres = 128/8;
	render_line = &render_line6c;
	
	display.screen = (unsigned char*)malloc(display.hres * display.vres * sizeof(unsigned char));
	if (display.screen == NULL)
		return 4;
	
	render_setup(mode);
	
	return 0;
}

/* call this to start video output with a specified resolution.
 * mode selects NTSC or PAL
 * x is the horizontal resolution MUST BE DIVISABLE BY 8 (max 248)
 * y is the vertical resolution (max 255)
 */
char TVout::begin(uint8_t mode, uint8_t x, uint8_t y) {

	display.hres = x/8;
	display.vres = y;
	if (mode)
		display.vscale_const = _PAL_LINE_DISPLAY/display.vres - 1;
	else
		display.vscale_const = _NTSC_LINE_DISPLAY/display.vres - 1;
	display.vscale = display.vscale_const;
	

	if ( !(display.hres & 0xF8))
		return 1;
	if ( display.hres > 248 )
		return 2;
	if (!mode && y > _NTSC_LINE_DISPLAY)
		return 3;
		
	display.screen = (unsigned char*)malloc(display.hres * display.vres * sizeof(unsigned char));
	if (display.screen == NULL)
		return 4;
		
	if ( display.hres >= 13 && display.hres <= 16 )
		render_line = &render_line6c;
	else if ( display.hres >= 17 && display.hres <= 19)
		render_line = &render_line5c;
	else
		return 5;
	
	render_setup(mode);
	return 0;
}

/* pauses rendering but keeps outputting a HSYNC this gives all CPU time to
 * user code
*/
void TVout::pause() {
	TIMSK1 &= ~_BV(TOIE1);
	OCR1A = _CYCLES_HORZ_SYNC;
}

/* Resume rendering the screen
*/
void TVout::resume() {
	display.scanLine = display.lines_frame+1;
	line_handler = &vsync_line;
	TIMSK1 = _BV(TOIE1);
}

/* Clears the screen
*/
void TVout::fill(uint8_t color) {
	switch(color) {
		case 0:
			for (int i = 0; i < (display.hres)*display.vres; i++)
				display.screen[i] = 0;
			break;
		case 1:
			for (int i = 0; i < (display.hres)*display.vres; i++)
				display.screen[i] = 0xFF;
			break;
		case 2:
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
	return ((display.hres*8)/font);
}

/* Delay for x frames
 * for NTSC 1 second = 60 frames
 * for PAL 1 second = 50 frames
 */
void TVout::delay(unsigned int x) {
	if (TIMSK1 & _BV(TOIE1)) {
		while (x) {
			while (display.scanLine != display.stop_render+1);
			while (display.scanLine == display.stop_render+1);
			x--;
		}
	}
	else {
		int lines;
		while (x) {
			lines = display.lines_frame;
			while (lines) {
				while (TCNT1 < _CYCLES_HORZ_SYNC);
				while (TCNT1 > _CYCLES_HORZ_SYNC);
				lines--;
			}
			x--;
		}
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
	return ((display.screen[(x/8) +(y*display.hres)] >> ((~x) & 7)) &0x01);
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

/* draw a box
 * x1,y1 to x2,y2
 * with color 1 = white, 0=black, 2=invert
 * with fill 0 = black fill, 1 = white fill, 2 = invert fill, 3 = no fill
 * with radius for rounded box
 * safe draw or not 1 = safe
 */
void TVout::draw_box(unsigned char x0, unsigned char y0,
			   unsigned char x1, unsigned char y1, char c, char d, char e, char f)
{
    /* x0,y0 = start position from top left    x1,y1 = width and height
    *  c is box line colour, d is fill colour, e is radius for rounded rectangles (0 radius = straight box)
    *  if the radius is bigger than x0 or y0 then dont render, if the position + length + radius is bigger than the resolution then dont render
    */
    if (f == 0 ) { // if safe sp is off
		if (x0 >= display.hres*8 || y0 >= display.vres || x0 + x1 + e >= display.hres*8 || y0 + y1 + e >= display.vres)
			return;
    }

    unsigned char x,y;

    if (e == 0) { // ordinary box
		if (d < 3) { // fill the box with the appropriate colour if the fill is 0, 1 or 2. if 3 or higher, no fill
			for (y = y0+1; y < y0+y1-1; y++) {
				for (x = x0+1; x < x0+x1; x++)
					sp_safe(x,y,d,f);
			}
		}
		// Now draw the box
		for (y = y0; y < y0+y1; y++) {
			sp_safe(x0,y,c,f);   // left hand line
			sp_safe(x0+x1,y,c,f); // right hand line
		}
		for (x = x0+1; x < x0 + x1; x++) {
			sp_safe(x,y0,c,f); // top line
			sp_safe(x,y0+y1-1,c,f); // bottom line
		}
    }
    if (e>0) {   // rounded rectangle! Split a circle into the 4 corners and draw a box
	// subtract the radius of the corners from the straight box we want to draw

		if (e+e > y1) // we cant draw a rounded box with a total radius bigger than the dimensions
			return;
		if (e+e > x1)
			return;

		x0=x0+e;
		y0=y0+e;
		y1=y1-e-e;
		x1=x1-e-e;
		// draw the box


		if (d < 3) {// fill the box with the appropriate colour if the fill is 0, 1 or 2. if 3 or higher, no fill
			for (y = y0-e; y < y0+y1+e; y++) {
				for (x = x0; x < x0+x1+1; x++)
					sp_safe(x,y,d,f);
			}
			for (y = y0; y < y0+y1+1; y++) {
				for (x = x0-e; x < x0+1; x++)
					sp_safe(x,y,d,f);
				for (x = x0+x1; x < x0+x1+e+1; x++)
					sp_safe(x,y,d,f);
			}
		}


		for (y = y0; y < y0+y1; y++) {
			sp_safe(x0-e,y+1,c,f);		// First do the left hand line
			sp_safe(x0+x1+e,y+1,c,f);	// Now do the right hand line
		}

		for (x = x0+1; x < x0 + x1+1; x++) {
			sp_safe(x,y0-e,c,f);		// First do the top line
			sp_safe(x,y0+y1+e,c,f);		// Now do the bottom line
		}

		int radius = e;
		int ff = 1 - radius;
		int dx = 1;
		int dy = -2 * radius;
		int x = 0;
		int y = radius;
		int sty1 = 0;

		// draw  quarter circles at each corner of the box

		ff = 1 - radius;
		dx = 1;
		dy = -2 * radius;
		x = 0;
		y = radius;

		if (d<3) {
			// if filled, use endpoints to draw lines to center x
			for (sty1=0; sty1<radius; sty1++) {
				sp_safe(x0,y0+sty1+y1,d,f);
				sp_safe(x0,y0-sty1,d,f);
				sp_safe(x0+sty1+x1,y0,d,f);
				sp_safe(x0-sty1,y0,d,f);
			}
		}
		sp_safe(x0, y0 + radius+y1,c,f);
		sp_safe(x0, y0 - radius,c,f);
		sp_safe(x0 + radius+x1, y0,c,f);
		sp_safe(x0 - radius, y0,c,f);
		while(x < y) {
			if(ff >= 0) {
				y--;
				dy += 2;
				ff += dy;
			}
			x++;
			dx += 2;
			ff += dx;
			if (d<3) {
				// if filled, use endpoints to draw lines to center x
				for (sty1=0; sty1<y; sty1++) {
					sp_safe(x0+x+x1,y0+sty1+y1,d,f);
					sp_safe(x0-x,y0+sty1+y1,d,f);
					sp_safe(x0+x+x1,y0-sty1,d,f);
					sp_safe(x0-x,y0-sty1,d,f);
					sp_safe(x0+sty1+x1,y0+x+y1,d,f);
					sp_safe(x0-sty1,y0+x+y1,d,f);
					sp_safe(x0+sty1+x1,y0-x,d,f);
					sp_safe(x0-sty1,y0-x,d,f);
				}
			}
			sp_safe(x0 + x+x1, y0 + y+y1,c,f);
			sp_safe(x0 - x, y0 + y+y1,c,f);
			sp_safe(x0 + x+x1, y0 - y,c,f);
			sp_safe(x0 - x, y0 - y,c,f);
			sp_safe(x0 + y+x1, y0 + x+y1,c,f);
			sp_safe(x0 - y, y0 + x+y1,c,f);
			sp_safe(x0 + y+x1, y0 - x,c,f);
			sp_safe(x0 - y, y0 - x,c,f);
		}
		if (d<3) { // redraw circle
			// draw circle
			ff = 1 - radius;
			dx = 1;
			dy = -2 * radius;
			x = 0;
			y = radius;

			sp_safe(x0, y0 + radius+y1,c,f);
			sp_safe(x0, y0 - radius,c,f);
			sp_safe(x0 + radius+x1, y0,c,f);
			sp_safe(x0 - radius, y0,c,f);

			while(x < y) {

				if(ff >= 0) {
					y--;
					dy += 2;
					ff += dy;
				}
				x++;
				dx += 2;
				ff += dx;
				sp_safe(x0 + x+x1, y0 + y+y1,c,f);
				sp_safe(x0 - x, y0 + y+y1,c,f);
				sp_safe(x0 + x+x1, y0 - y,c,f);
				sp_safe(x0 - x, y0 - y,c,f);
				sp_safe(x0 + y+x1, y0 + x+y1,c,f);
				sp_safe(x0 - y, y0 + x+y1,c,f);
				sp_safe(x0 + y+x1, y0 - x,c,f);
				sp_safe(x0 - y, y0 - x,c,f);
			}
		}
    }
}

/* draw a circle
 * x0,y0 around radius
 * with color 1 = white, 0=black, 2=invert
 * with fill 0 = black fill, 1 = white fill, 2 = invert fill, 3 = no fill
 * safe draw or not 1 = safe
 * Added by Andy Cook 2010
 */
void TVout::draw_circle(unsigned char x0, unsigned char y0,
				unsigned char radius, char c, char d, char h) {


	if (h == 0) {
		if (x0+radius >= display.hres*8 || y0+radius >= display.vres || radius >= x0 || radius >= y0)
			return;
	}
	int f = 1 - radius;
	int dx = 1;
	int dy = -2 * radius;
	int x = 0;
	int y = radius;
	int sty1 = 0;

	// draw circle
	f = 1 - radius;
	dx = 1;
	dy = -2 * radius;
	x = 0;
	y = radius;

	if (d<3) { // if filled, use endpoints to draw lines to center x
		for (sty1=0; sty1<radius; sty1++) {
			sp_safe(x0,y0+sty1,d,h);
			sp_safe(x0,y0-sty1,d,h);
			sp_safe(x0+sty1,y0,d,h);
			sp_safe(x0-sty1,y0,d,h);
		}
	}
	sp_safe(x0, y0 + radius,c,h);
	sp_safe(x0, y0 - radius,c,h);
	sp_safe(x0 + radius, y0,c,h);
	sp_safe(x0 - radius, y0,c,h);
	while(x < y) {
		if(f >= 0) {
			y--;
			dy += 2;
			f += dy;
		}
		x++;
		dx += 2;
		f += dx;
		if (d<3) {
			// if filled, use endpoints to draw lines to center x
			for (sty1=0; sty1<y; sty1++) {
				sp_safe(x0+x,y0+sty1,d,h);
				sp_safe(x0-x,y0+sty1,d,h);
				sp_safe(x0+x,y0-sty1,d,h);
				sp_safe(x0-x,y0-sty1,d,h);
				sp_safe(x0+sty1,y0+x,d,h);
				sp_safe(x0-sty1,y0+x,d,h);
				sp_safe(x0+sty1,y0-x,d,h);
				sp_safe(x0-sty1,y0-x,d,h);
			}
		}
		sp_safe(x0 + x, y0 + y,c,h);
		sp_safe(x0 - x, y0 + y,c,h);
		sp_safe(x0 + x, y0 - y,c,h);
		sp_safe(x0 - x, y0 - y,c,h);
		sp_safe(x0 + y, y0 + x,c,h);
		sp_safe(x0 - y, y0 + x,c,h);
		sp_safe(x0 + y, y0 - x,c,h);
		sp_safe(x0 - y, y0 - x,c,h);
	}
	if (d<3) {  // redraw circle

	// draw circle
		f = 1 - radius;
		dx = 1;
		dy = -2 * radius;
		x = 0;
		y = radius;

		sp_safe(x0, y0 + radius,c,h);
		sp_safe(x0, y0 - radius,c,h);
		sp_safe(x0 + radius, y0,c,h);
		sp_safe(x0 - radius, y0,c,h);

		while(x < y) {
			if(f >= 0) {
				y--;
				dy += 2;
				f += dy;
			}
			x++;
			dx += 2;
			f += dx;
			sp_safe(x0 + x, y0 + y,c,h);
			sp_safe(x0 - x, y0 + y,c,h);
			sp_safe(x0 + x, y0 - y,c,h);
			sp_safe(x0 - x, y0 - y,c,h);
			sp_safe(x0 + y, y0 + x,c,h);
			sp_safe(x0 - y, y0 + x,c,h);
			sp_safe(x0 + y, y0 - x,c,h);
			sp_safe(x0 - y, y0 - x,c,h);
		}
	}
}


/* very simple bitmap placeing function, bitmap must be full screen and
 * called bitmap, this will be expanded.
 */
void TVout::fs_bitmap() {
	uint8_t byte;
	for (int i = 0; i < display.hres*display.vres; i++) {
		byte = pgm_read_byte((uint32_t)(bitmap) + i);
		display.screen[i] = byte;
		if (byte != display.screen[i]) {
			print_str(0,80,"Copy Err");
			while(1);
		}
	}
}

void TVout::select_font(uint8_t f) {
	font = f;
}
/*
 * print an 8x8 char c at x,y
 * x must be a multiple of 8
 */
void TVout::print_char(uint8_t x, uint8_t y, char c) {
	uint8_t y_pos;
	uint8_t j;
	
	if (font == _3X5) {
	
		// since this is not a complete font set fix the character
		if (c < 39)
			return;
		else if (c > 'z')
			c-=(26+39);
		else if (c > '`')
			c-=(97-65+39);
		else
			c-=39;
			
		uint8_t mask;
		if (x & 0x03) {
			if (x == (x & 0xF8))
				mask = 0x0F;
			else
				mask = 0xF0;
			for (char i = 0; i < 5; i++) {
				j = pgm_read_byte(((uint32_t)(ascii3x5)) + c*5 +i);
				display.screen[x+(y*display.hres)] = (display.screen[x+(y*display.hres)] & mask) | (j & ~mask);
				y++;
			}
		}
		else {

			for (char i=0;i<5;i++) {
				y_pos = y + i;

				j = pgm_read_byte(((uint32_t)(ascii3x5)) + c*5 + i);

				sp(x,   y_pos, (j & 0x80)==0x80);  
				sp(x+1, y_pos, (j & 0x40)==0x40); 
				sp(x+2, y_pos, (j & 0x20)==0x20);
				sp(x+3, y_pos, (j & 0x10)==0x10);
			}
		}
	}
	else if (font == _5X7) {
		
		// since there is nothing in the first 32 characters of the 5x7 font fix the offset c
		if (c < ' ')
			return;
		else
			c -= 32;
		for (char i=0;i<7;i++) {
			y_pos = y + i;

			j = pgm_read_byte(((uint32_t)(ascii5x7)) + c*7 + i);

			sp(x,   y_pos, (j & 0x80)==0x80);  
			sp(x+1, y_pos, (j & 0x40)==0x40); 
			sp(x+2, y_pos, (j & 0x20)==0x20);
			sp(x+3, y_pos, (j & 0x10)==0x10);
			sp(x+4, y_pos, (j & 0x08)==0x08);
		}
	}
	else if (font == _8X8) {
		if (x & 0x07) {
			for (char i=0;i<8;i++) {
				y_pos = y + i;

				j = pgm_read_byte(((uint32_t)(ascii8x8)) + c*8 + i);

				sp(x,   y_pos, (j & 0x80)==0x80);  
				sp(x+1, y_pos, (j & 0x40)==0x40); 
				sp(x+2, y_pos, (j & 0x20)==0x20);
				sp(x+3, y_pos, (j & 0x10)==0x10);
				sp(x+4, y_pos, (j & 0x08)==0x08);
				sp(x+5, y_pos, (j & 0x04)==0x04);
				sp(x+6, y_pos, (j & 0x02)==0x02);
				sp(x+7, y_pos, (j & 0x01)==0x01);
			}
		}
		else {
			x = x/8;
			for (char i = 0; i < 8; i++) {
				j = pgm_read_byte(((uint32_t)(ascii8x8)) + c*8 + i);
				display.screen[x+y*display.hres] = j;
				y++;
			}
		}
	}
}

/* print a null terminated string at x,y
*/
void TVout::print_str(uint8_t x, uint8_t y, char *str) {
	if (y >= display.vres)
		return;
	for (char i=0; str[i]!=0; i++) { 
		if (x > (display.hres*8+font))
			return;
		print_char(x,y,str[i]);
		x += font;
	}
}

/* Simple tone generation
 * Takes the frequency and duration in ms
 * courtesy of nootropic on arduino.cc forums
 */
void TVout::tone(unsigned int frequency, unsigned int duration_ms) {
	toneHalfWavelengthISRCount = ((1000000 / frequency) / 2) / 64;
	remainingISRs = duration_ms * (1000.0 / 64.0);
	remainingHalfWavelengthISRCount = toneHalfWavelengthISRCount;
}

void TVout::render_setup(uint8_t mode) {
	clear_screen();
	font = _8X8;
	// device specific settings changes must also be made in asm_macros.h
	
	_VID_DDR |= _BV(_VID_PIN);
	_SYNC_DDR |= _BV(_SYNC_PIN);
	_VID_PORT &= ~_BV(_VID_PIN);
	_SYNC_PORT |= _BV(_SYNC_PIN);
	DDRB |= 0x04;	// for tone generation.
	
	// inverted fast pwm mode on timer 1
	TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(WGM11);
	TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
	
	if (mode) {
		display.start_render = _PAL_LINE_MID - ((display.vres * (display.vscale+1))/2);
		display.stop_render = display.start_render + (display.vres * (display.vscale+1));
		display.output_delay = _PAL_CYCLES_OUTPUT_START;
		display.vsync_end = _PAL_LINE_STOP_VSYNC;
		display.lines_frame = _PAL_LINE_FRAME;
		ICR1 = _PAL_CYCLES_SCANLINE;
		OCR1A = _CYCLES_HORZ_SYNC;
		}
	else {
		display.start_render = _NTSC_LINE_MID - ((display.vres * (display.vscale+1))/2) + 8;
		display.stop_render = display.start_render + (display.vres * (display.vscale+1));
		display.output_delay = _NTSC_CYCLES_OUTPUT_START;
		display.vsync_end = _NTSC_LINE_STOP_VSYNC;
		display.lines_frame = _NTSC_LINE_FRAME;
		ICR1 = _NTSC_CYCLES_SCANLINE;
		OCR1A = _CYCLES_HORZ_SYNC;
	}
	TIMSK1 = _BV(TOIE1);
	display.scanLine = display.lines_frame+1;
	line_handler = &vsync_line;
	sei();
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

// Inline version of set_pixel that does perform a bounds check
// Added by Andy Cook 2010
static void inline sp_safe(unsigned char x, unsigned char y, char c, char d) {
	if (d==0) { // if d is zero then just do without any bounds check
		if (c==1)
			display.screen[(x/8) + (y*display.hres)] |= 0x80 >> (x&7);
		else if (c==0)
			display.screen[(x/8) + (y*display.hres)] &= ~0x80 >> (x&7);
		else
			display.screen[(x/8) + (y*display.hres)] ^= 0x80 >> (x&7);
	}
	else {  // d is 1, so check every pixel for boundary
		if (x < display.hres*8 && y < display.vres) {  // if x is in bounds AND if y is in bounds then proceed with pixel write
			if (c==1)
				display.screen[(x/8) + (y*display.hres)] |= 0x80 >> (x&7);
			else if (c==0)
				display.screen[(x/8) + (y*display.hres)] &= ~0x80 >> (x&7);
			else
				display.screen[(x/8) + (y*display.hres)] ^= 0x80 >> (x&7);
		}
	}
}