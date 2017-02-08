/*! \file glcd.h \brief Graphic LCD API functions. */
//*****************************************************************************
//
// File Name	: 'glcd.h'
// Title		: Graphic LCD API functions
// Author		: Pascal Stang - Copyright (C) 2002
// Date			: 5/30/2002
// Revised		: 5/30/2002
// Version		: 0.5
// Target MCU	: Atmel AVR
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
///	\ingroup driver_hw
/// \defgroup glcd Graphic LCD API (application programmer's interface) (glcd.c)
/// \code #include "glcd.h" \endcode
/// \par Overview
///		This library (or API) allows you to draw dots, lines, boxes, circles,
///	and text on most monochrome graphic LCDs.� An easily expandable font file
/// (5x7-pixel characters) is provided for all basic ASCII characters
/// (0x20-0x7F hex, 32-127 decimal).� An expandable graphic font file is
/// provided for defining specialty characters or custom icons.� Because this
/// library is designed to work with many different kinds of LCDs, it needs a
/// graphic LCD driver such as ks0108.c to enable it to talk to the LCD.
///
/// \note For full text-output functionality, you may wish to use the rprintf
/// functions along with this driver.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef GLCD_H
#define GLCD_H

#ifndef WIN32
// AVR specific includes
	#include <avr/io.h>
#endif

#include "global.h"

#define LINE1		0
#define LINE2		1
#define LINE3		2
#define LINE4		3
#define LINE5		4
#define LINE6		5
#define LINE7		6
#define LINE8		7

#define ON			1
#define OFF			0

// API-level interface commands
// ***** Public Functions *****

//! set a dot on the display (x is horiz 0:127, y is vert 0:63)
void glcdSetDot(u08 x, u08 y);

//! clear a dot on the display (x is horiz 0:127, y is vert 0:63)
void glcdClearDot(u08 x, u08 y);

//! draw line
void glcdLine(u08 x1, u08 y1, u08 x2, u08 y2);

//! draw rectangle (coords????)
void glcdRectangle(u08 x, u08 y, u08 a, u08 b);

//! draw circle of <radius> at <xcenter,ycenter>
void glcdCircle(u08 xcenter, u08 ycenter, u08 radius);

//! write a standard ascii charater (values 20-127)
// to the display at current position
void glcdWriteChar(unsigned char c);

//! write a special graphic character/icon
// to the display at current position
void glcdWriteCharGr(u08 grCharIndex);

// ***** Private Functions ***** (or depricated)
void glcdPutStr(u08 *data);

#endif
