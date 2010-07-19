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

#include "hardware_setup.h"

#ifndef ASM_MACROS_H
#define ASM_MACROS_H

// delay macros
__asm__ __volatile__ (
	// delay 1 clock cycle.
	".macro delay1\n\t"
		"nop\n"
	".endm\n"
	
	// delay 2 clock cycles
	".macro delay2\n\t"
		"nop\n\t"
		"nop\n"
	".endm\n"
	
	// delay 3 clock cyles
	".macro delay3\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n"
	".endm\n"
	
	// delay 4 clock cylces
	".macro delay4\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n"
	".endm\n"
	
	// delay 5 clock cylces
	".macro delay5\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n"
	".endm\n"
	
	// delay 6 clock cylces
	".macro delay6\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n"
	".endm\n"
	
	// delay 7 clock cylces
	".macro delay7\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n"
	".endm\n"
	
	// delay 8 clock cylces
	".macro delay8\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n"
	".endm\n"
	
	// delay 9 clock cylces
	".macro delay9\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n"
	".endm\n"
	
	// delay 10 clock cylces
	".macro delay10\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n"
	".endm\n"
); // end of delay macros

// common output macros, specific output macros at top of file
__asm__ __volatile__ (
	// output 1 bit port unsafe, highest order bit on port.
	// lowest would require modifications to how the frame buffer
	// is mapped to memory
	".macro o1bu p\n\t"
		"lsl	__tmp_reg__\n\t"
		"out	\\p,__tmp_reg__\n"
	".endm\n"
	
	// save port 16 and clear the video bit
	".macro svprt p\n\t"
		"in		r16,\\p\n\t"
		_ANDI_HWS
	".endm\n"
	
	// ouput 1 bit port safe
	".macro o1bs p\n\t"
		_BLD_HWS
		"out	\\p,r16\n"
	".endm\n"
); // end of output macros

//byte shift macros
__asm__ __volatile__ (

	// shift a byte out the highest order pin of the given port
	// at a rate of 3cycles per bit.
	".macro bs3c port\n\t"
		"LD		__tmp_reg__,X+\n\t"			//1
		"out	\\port\n\t"
		"delay1\n\t"						//2
		"o1bu	\\port\n\t"
		"delay1\n\t"						//3
		"o1bu	\\port\n\t"
		"delay1\n\t"						//4
		"o1bu	\\port\n\t"
		"delay1\n\t"						//5
		"o1bu	\\port\n\t"
		"delay1\n\t"						//6
		"o1bu	\\port\n\t"
		"delay1\n\t"						//7
		"o1bu	\\port\n\t"
		"delay1\n\t"						//8
		"o1bu	\\port\n"
	".endm\n"
);
#endif