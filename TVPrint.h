/*
Print.h - Base class that provides print() and println()
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
*/

#ifndef TVPrint_h
#define TVPrint_h

#include <inttypes.h>
#include <stdio.h> // for size_t
#include <Print.h>
#include "video_gen.h"

class TVPrint : public Print
{
protected:
	uint8_t cursor_x,cursor_y,font;
	virtual void inc_txtline() = 0;
public:
	virtual void print_char(uint8_t, uint8_t, unsigned char) = 0;
	virtual void write(uint8_t);
	using Print::write;
	
	void set_cursor(uint8_t, uint8_t);
	
	using Print::print;
	void print(uint8_t, uint8_t, const char[]);
	void print(uint8_t, uint8_t, char, int = BYTE);
	void print(uint8_t, uint8_t, unsigned char, int = BYTE);
	void print(uint8_t, uint8_t, int, int = DEC);
	void print(uint8_t, uint8_t, unsigned int, int = DEC);
	void print(uint8_t, uint8_t, long, int = DEC);
	void print(uint8_t, uint8_t, unsigned long, int = DEC);
	void print(uint8_t, uint8_t, double, int = 2);
	
	void printPGM(const char[]);
	void printPGM(uint8_t, uint8_t, const char[]);

	//basic "dumb" print functions usefull for special character definitions
	//outside of the range of "normal" characters.
	void print_str(uint8_t,uint8_t,const char[]);
	void print_str_P(uint8_t,uint8_t,const char[]);
};

#endif
