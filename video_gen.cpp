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

#include <avr/interrupt.h>
#include <avr/io.h>

#include "video_gen.h"
#include "spec/video_properties.h"
#include "spec/asm_macros.h"
#include "spec/hardware_setup.h"

int renderLine;
TVout_vid display;
void (*render_line)();
void (*line_handler)();
void (*fastpoll)() = &empty;

// render a line
ISR(TIMER1_OVF_vect) {
	fastpoll();
	line_handler();
}

void blank_line() {
	if ( display.scanLine == display.start_render) {
		renderLine = 0;
		display.vscale = display.vscale_const;
		line_handler = &active_line;
	}
	else if (display.scanLine == display.lines_frame)
		line_handler = &vsync_line;
	
	display.scanLine++;
}

void active_line() {
	wait_until(display.output_delay);
	render_line();
	if (!display.vscale) {
		display.vscale = display.vscale_const;
		renderLine += display.hres;
	}
	else
		display.vscale--;
		
	if ((display.scanLine + 1) == display.stop_render)
		line_handler = &blank_line;
		
	display.scanLine++;
}

void vsync_line() {
	if (display.scanLine >= display.lines_frame) {
		OCR1A = _CYCLES_VIRT_SYNC;
		display.scanLine = 0;
	}
	else if (display.scanLine == display.vsync_end) {
		OCR1A = _CYCLES_HORZ_SYNC;
		line_handler = &blank_line;
	}
	display.scanLine++;
}

void empty() {
}

static void inline wait_until(uint8_t time) {
	__asm__ __volatile__ (
			"subi	%[time], 10\n"
			"sub	%[time], %[tcnt1l]\n\t"
		"100:\n\t"
			"subi	%[time], 3\n\t"
			"brcc	100b\n\t"
			"subi	%[time], 0-3\n\t"
			"breq	101f\n\t"
			"dec	%[time]\n\t"
			"breq	102f\n\t"
			"rjmp	102f\n"
		"101:\n\t"
			"nop\n" 
		"102:\n"
		:
		: [time] "a" (time),
		[tcnt1l] "a" (TCNT1L)
	);
}

void render_line6c() {
	__asm__ __volatile__ (
		"ADD	r26,r28\n\t"
		"ADC	r27,r29\n\t"
		//save PORTB
		"svprt	%[port]\n\t"
		
		"rjmp	enter6\n"
	"loop6:\n\t"
		"bst	__tmp_reg__,0\n\t"			//8
		"o1bs	%[port]\n"
	"enter6:\n\t"
		"LD		__tmp_reg__,X+\n\t"			//1
		"delay1\n\t"
		"bst	__tmp_reg__,7\n\t"
		"o1bs	%[port]\n\t"
		"delay3\n\t"						//2
		"bst	__tmp_reg__,6\n\t"
		"o1bs	%[port]\n\t"
		"delay3\n\t"						//3
		"bst	__tmp_reg__,5\n\t"
		"o1bs	%[port]\n\t"
		"delay3\n\t"						//4
		"bst	__tmp_reg__,4\n\t"
		"o1bs	%[port]\n\t"
		"delay3\n\t"						//5
		"bst	__tmp_reg__,3\n\t"
		"o1bs	%[port]\n\t"
		"delay3\n\t"						//6
		"bst	__tmp_reg__,2\n\t"
		"o1bs	%[port]\n\t"
		"delay3\n\t"						//7
		"bst	__tmp_reg__,1\n\t"
		"o1bs	%[port]\n\t"
		"dec	%[hres]\n\t"
		"brne	loop6\n\t"					//go too loopsix
		"delay2\n\t"
		"bst	__tmp_reg__,0\n\t"			//8
		"o1bs	%[port]\n"
		
		"svprt	%[port]\n\t"
		"bst	r16,1\n\t"
		"o1bs	%[port]\n\t"
		:
		: [port] "i" (_SFR_IO_ADDR(_VID_PORT)),
		"x" (display.screen),
		"y" (renderLine),
		[hres] "d" (display.hres)
		: "r16" // try to remove this clobber later...
	);
}

void render_line5c() {

	__asm__ __volatile__ (
		"ADD	r26,r28\n\t"
		"ADC	r27,r29\n\t"
		//save PORTB
		"svprt	%[port]\n\t"
		
		"rjmp	enter5\n"
	"loop5:\n\t"
		"bst	__tmp_reg__,0\n\t"			//8
		"o1bs	%[port]\n"
	"enter5:\n\t"
		"LD		__tmp_reg__,X+\n\t"			//1
		"bst	__tmp_reg__,7\n\t"
		"o1bs	%[port]\n\t"
		"delay2\n\t"						//2
		"bst	__tmp_reg__,6\n\t"
		"o1bs	%[port]\n\t"
		"delay2\n\t"						//3
		"bst	__tmp_reg__,5\n\t"
		"o1bs	%[port]\n\t"
		"delay2\n\t"						//4
		"bst	__tmp_reg__,4\n\t"
		"o1bs	%[port]\n\t"
		"delay2\n\t"						//5
		"bst	__tmp_reg__,3\n\t"
		"o1bs	%[port]\n\t"
		"delay2\n\t"						//6
		"bst	__tmp_reg__,2\n\t"
		"o1bs	%[port]\n\t"
		"delay1\n\t"						//7
		"dec	%[hres]\n\t"
		"bst	__tmp_reg__,1\n\t"
		"o1bs	%[port]\n\t"
		"brne	loop5\n\t"					//go too loop5
		"delay1\n\t"
		"bst	__tmp_reg__,0\n\t"			//8
		"o1bs	%[port]\n"
		
		"svprt	%[port]\n\t"
		"bst	r16,1\n\t"
		"o1bs	%[port]\n\t"
		:
		: [port] "i" (_SFR_IO_ADDR(_VID_PORT)),
		"x" (display.screen),
		"y" (renderLine),
		[hres] "d" (display.hres)
		: "r16" // try to remove this clobber later...
	);
}

void render_line4c() {
	__asm__ __volatile__ (
		"ADD	r26,r28\n\t"
		"ADC	r27,r29\n\t"
		
		"rjmp	enter4\n"
	"loop4:\n\t"
		"lsl	__tmp_reg__\n\t"			//8
		"out	%[port],__tmp_reg__\n\t"
	"enter4:\n\t"
		"LD		__tmp_reg__,X+\n\t"			//1
		"delay1\n\t"
		"out	%[port],__tmp_reg__\n\t"
		"delay2\n\t"						//2
		"lsl	__tmp_reg__\n\t"
		"out	%[port],__tmp_reg__\n\t"
		"delay2\n\t"						//3
		"lsl	__tmp_reg__\n\t"
		"out	%[port],__tmp_reg__\n\t"
		"delay2\n\t"						//4
		"lsl	__tmp_reg__\n\t"
		"out	%[port],__tmp_reg__\n\t"
		"delay2\n\t"						//5
		"lsl	__tmp_reg__\n\t"
		"out	%[port],__tmp_reg__\n\t"
		"delay2\n\t"						//6
		"lsl	__tmp_reg__\n\t"
		"out	%[port],__tmp_reg__\n\t"
		"delay1\n\t"						//7
		"lsl	__tmp_reg__\n\t"
		"dec	%[hres]\n\t"
		"out	%[port],__tmp_reg__\n\t"
		"brne	loop4\n\t"					//go too loop4
		"delay1\n\t"						//8
		"lsl	__tmp_reg__\n\t"
		"out	%[port],__tmp_reg__\n\t"
		"delay3\n\t"
		"cbi	%[port],7\n\t"
		:
		: [port] "i" (_SFR_IO_ADDR(_VID_PORT)),
		"x" (display.screen),
		"y" (renderLine),
		[hres] "d" (display.hres)
		: "r16" // try to remove this clobber later...
	);
}

// only 16mhz right now!!!
void render_line3c() {

	__asm__ __volatile__ (
	".macro byteshift\n\t"
		"LD		__tmp_reg__,X+\n\t"
		"out	%[port],__tmp_reg__\n\t"	//0
		"nop\n\t"
		"lsl	__tmp_reg__\n\t"
		"out	%[port],__tmp_reg__\n\t"	//1
		"nop\n\t"
		"lsl	__tmp_reg__\n\t"
		"out	%[port],__tmp_reg__\n\t"	//2
		"nop\n\t"
		"lsl	__tmp_reg__\n\t"
		"out	%[port],__tmp_reg__\n\t"	//3
		"nop\n\t"
		"lsl	__tmp_reg__\n\t"
		"out	%[port],__tmp_reg__\n\t"	//4
		"nop\n\t"
		"lsl	__tmp_reg__\n\t"
		"out	%[port],__tmp_reg__\n\t"	//5
		"nop\n\t"
		"lsl	__tmp_reg__\n\t"
		"out	%[port],__tmp_reg__\n\t"	//6
		"nop\n\t"
		"lsl	__tmp_reg__\n\t"
		"out	%[port],__tmp_reg__\n\t"	//7
	".endm\n\t"
	
		"ADD	r26,r28\n\t"
		"ADC	r27,r29\n\t"
		
		"cpi	%[hres],30\n\t"		//615
		"breq	skip0\n\t"
		"cpi	%[hres],29\n\t"
		"breq	jumpto1\n\t"
		"cpi	%[hres],28\n\t"
		"breq	jumpto2\n\t"
		"cpi	%[hres],27\n\t"
		"breq	jumpto3\n\t"
		"cpi	%[hres],26\n\t"
		"breq	jumpto4\n\t"
		"cpi	%[hres],25\n\t"
		"breq	jumpto5\n\t"
		"cpi	%[hres],24\n\t"
		"breq	jumpto6\n\t"
	"jumpto1:\n\t"
		"rjmp	skip1\n\t"
	"jumpto2:\n\t"
		"rjmp	skip2\n\t"
	"jumpto3:\n\t"
		"rjmp	skip3\n\t"
	"jumpto4:\n\t"
		"rjmp	skip4\n\t"
	"jumpto5:\n\t"
		"rjmp	skip5\n\t"
	"jumpto6:\n\t"
		"rjmp	skip6\n\t"
	"skip0:\n\t"
		"byteshift\n\t"	//1		\\643
	"skip1:\n\t"
		"byteshift\n\t"	//2
	"skip2:\n\t"
		"byteshift\n\t"	//3
	"skip3:\n\t"
		"byteshift\n\t"	//4
	"skip4:\n\t"
		"byteshift\n\t"	//5
	"skip5:\n\t"
		"byteshift\n\t"	//6
	"skip6:\n\t"
		"byteshift\n\t"	//7
		"byteshift\n\t"	//8
		"byteshift\n\t"	//9
		"byteshift\n\t"	//10
		"byteshift\n\t"	//11
		"byteshift\n\t"	//12
		"byteshift\n\t"	//13
		"byteshift\n\t"	//14
		"byteshift\n\t"	//15
		"byteshift\n\t"	//16
		"byteshift\n\t"	//17
		"byteshift\n\t"	//18
		"byteshift\n\t"	//19
		"byteshift\n\t"	//20
		"byteshift\n\t"	//21
		"byteshift\n\t"	//22
		"byteshift\n\t"	//23
		"byteshift\n\t"	//24
		"byteshift\n\t"	//25
		"byteshift\n\t"	//26
		"byteshift\n\t"	//27
		"byteshift\n\t"	//28
		"byteshift\n\t"	//29
		"byteshift\n\t"	//30
		
		"delay2\n\t"
		"cbi	%[port],7\n\t"
		:
		: [port] "i" (_SFR_IO_ADDR(_VID_PORT)),
		"x" (display.screen),
		"y" (renderLine),
		[hres] "d" (display.hres)
		: "r16" // try to remove this clobber later...
	);
}