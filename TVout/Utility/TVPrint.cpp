/*
TVPrint.cpp - Base class that provides print() and println()
Copyright (c) 2008 David A. Mellis.  All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Modified 23 November 2006 by David A. Mellis
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <avr/pgmspace.h>

#include "TVPrint.h"

void TVPrint::select_font(const unsigned char * f) {
	font = f;
}

void TVPrint::write(uint8_t c) {
	switch(c) {
		case '\0':			//null
			break;
		case '\n':			//line feed
			cursor_x = 0;
			inc_txtline();
			break;
		case 8:				//backspace
			cursor_x -= pgm_read_byte(font);
			print_char(cursor_x,cursor_y,' ');
			break;
		case 13:			//carriage return !?!?!?!VT!?!??!?!
			cursor_x = 0;
			break;
		case 14:			//form feed new page(clear screen)
			//clear_screen();
			break;
		default:
			if (cursor_x >= (display.hres*8 - pgm_read_byte(font))) {
				cursor_x = 0;
				inc_txtline();
				print_char(cursor_x,cursor_y,c);
			}
			else
				print_char(cursor_x,cursor_y,c);
			cursor_x += pgm_read_byte(font);
	}
}

void TVPrint::printPGM(const char str[]) {
	char c;
	while ((c = pgm_read_byte(str))) {
		str++;
		write(c);
	}
}

void TVPrint::printPGM(uint8_t x, uint8_t y, const char str[]) {
	char c;
	cursor_x = x;
	cursor_y = y;
	while ((c = pgm_read_byte(str))) {
		str++;
		write(c);
	}
}

void TVPrint::set_cursor(uint8_t x, uint8_t y) {
	cursor_x = x;
	cursor_y = y;
}

void TVPrint::print(uint8_t x, uint8_t y, const char str[]) {
	cursor_x = x;
	cursor_y = y;
	write(str);
	
}
void TVPrint::print(uint8_t x, uint8_t y, char c, int base) {
	cursor_x = x;
	cursor_y = y;
	print((long) c, base);
}
void TVPrint::print(uint8_t x, uint8_t y, unsigned char b, int base) {
	cursor_x = x;
	cursor_y = y;
	print((unsigned long) b, base);
}
void TVPrint::print(uint8_t x, uint8_t y, int n, int base) {
	cursor_x = x;
	cursor_y = y;
	print((long) n, base);
}
void TVPrint::print(uint8_t x, uint8_t y, unsigned int n, int base) {
	cursor_x = x;
	cursor_y = y;
	print((unsigned long) n, base);
}
void TVPrint::print(uint8_t x, uint8_t y, long n, int base) {
	cursor_x = x;
	cursor_y = y;
	print(n,base);
}
void TVPrint::print(uint8_t x, uint8_t y, unsigned long n, int base) {
	cursor_x = x;
	cursor_y = y;
	print(n,base);
}
void TVPrint::print(uint8_t x, uint8_t y, double n, int digits) {
	cursor_x = x;
	cursor_y = y;
	print(n,digits);
}

void TVPrint::print_str(uint8_t x,uint8_t y,const char str[]) {
	if (y >= display.vres)
		return;
	for (char i=0; str[i]!=0; i++) { 
		if (x > (display.hres*8+pgm_read_byte(font)))
			return;
		print_char(x,y,str[i]);
		x += pgm_read_byte(font);
	}
}

void TVPrint::print_str_P(uint8_t x,uint8_t y, const char str[]) {
	if (y >= display.vres)
		return;

	char c;
	while ((c=pgm_read_byte(str)))
	{
		str++;
		if (x > (display.hres*8+pgm_read_byte(font)))
			return;
		print_char(x,y,c);
		x += pgm_read_byte(font);
	}
}