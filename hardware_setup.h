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

#ifndef HARDWARE_SETUP_H
#define HARDWARE_SETUP_H

// device specific settings.
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__)
#define _VID_PORT	PORTB
#define	_VID_DDR	DDRB
#define _VID_PIN	6
#define _SYNC_PORT	PORTB
#define _SYNC_DDR	DDRB
#define	_SYNC_PIN	5
#define _ANDI_HWS	"andi	r16,0xBF\n"
#define _BLD_HWS	"bld	r16,6\n\t"

#elif defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
#define _VID_PORT	PORTD
#define	_VID_DDR	DDRD
#define _VID_PIN	4
#define _SYNC_PORT	PORTD
#define _SYNC_DDR	DDRD
#define _SYNC_PIN	5
#define _ANDI_HWS	"andi	r16,0xDF\n"
#define _BLD_HWS	"bld	r16,4\n\t"

#elif defined(__AVR_ATmega8__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__)
#define _VID_PORT	PORTB
#define	_VID_DDR	DDRB
#define	_VID_PIN	0
#define _SYNC_PORT	PORTB
#define _SYNC_DDR	DDRB
#define _SYNC_PIN	1
#define _ANDI_HWS	"andi	r16,0xFD\n"
#define _BLD_HWS	"bld	r16,0\n\t"

#endif
#endif