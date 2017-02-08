#ifndef KEYMAP_H
#define KEYMAP_H

#include <avr/pgmspace.h>

// Total 132 keys + one none
#define NUM_KEY 133

#define KEY_FN 255

enum {
	KEY_NONE=0,

	KEY_HASH,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9, 
	KEY_0,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_BKSP,

	KEY_TAB,
	KEY_Q,
	KEY_W,
	KEY_E,
	KEY_R,
	KEY_T,
	KEY_Y,
	KEY_U,
	KEY_I,
	KEY_O,
	KEY_P,
	KEY_LBR,
	KEY_RBR,
	KEY_BKSLASH,

	KEY_CAPS,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_COLON,
	KEY_QUOTE,
	KEY_ENTER,
	
	KEY_LSHIFT,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	KEY_N,
	KEY_M,
	KEY_COMMA,
	KEY_DOT,
	KEY_SLASH,
	KEY_RSHIFT,
	
	KEY_LCTRL,
	KEY_LGUI,
	KEY_LALT,
	KEY_SPACE,
	KEY_RALT,
	KEY_RGUI,
	KEY_APPS,
	KEY_RCTRL,
	KEY_ESC,

	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,

	KEY_NUMLOCK,
	KEY_KP_MINUS,
	KEY_KP_SLASH,
	KEY_KP_DOT,
	KEY_KP_AST,
	KEY_KP_PLUS,
	KEY_KP_ENTER,

	KEY_KP_0,
	KEY_KP_1,
	KEY_KP_2,
	KEY_KP_3,
	KEY_KP_4,
	KEY_KP_5,
	KEY_KP_6,
	KEY_KP_7,
	KEY_KP_8,
	KEY_KP_9,
	
	KEY_INSERT,
	KEY_HOME,
	KEY_PGUP,
	KEY_DEL,
	KEY_END,
	KEY_PGDN,
	
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	
	KEY_PRNSCR,
	KEY_SCRLCK,
	KEY_PAUSE,

	KEY_HANJA,	KEY_HANGLE,	KEY_POWER,	KEY_SLEEP,	KEY_WAKE,	KEY_EMAIL,
 	KEY_WWW_SEARCH, KEY_WWW_HOME,	KEY_WWW_BACK,	KEY_WWW_FORWARD,	KEY_WWW_STOP,	KEY_WWW_REFRESH,	KEY_WWW_FAVORITE,	KEY_NEXT_TRK,	KEY_PREV_TRK,	KEY_STOP,
 	KEY_PLAY,	KEY_MUTE,	KEY_VOL_UP,	KEY_VOL_DOWN,	KEY_MEDIA,	KEY_CALC,	KEY_MYCOM,	KEY_SCREENSAVE,	KEY_REC,	KEY_REWIND,
 	KEY_MINIMIZE,	KEY_EJECT
};

const uint8_t PROGMEM keycode_set2[NUM_KEY] =	{ 0x00,
 0x0E, 0x16, 0x1E, 0x26, 0x25, 0x2E, 0x36, 0x3D, 0x3E, 0x46, 0x45, 0x4E, 0x55, 0x66,// Hash1234...
 0x0D, 0x15, 0x1D, 0x24, 0x2D, 0x2C, 0x35, 0x3C, 0x43, 0x44, 0x4D, 0x54, 0x5B, 0x5D,// TabQWERT..
 0x58, 0x1C, 0x1B, 0x23, 0x2B, 0x34, 0x33, 0x3B, 0x42, 0x4B, 0x4C, 0x52, 0x5A,		// CapASDF..
 0x12, 0x1A, 0x22, 0x21, 0x2A, 0x32, 0x31, 0x3A, 0x41, 0x49, 0x4A, 0x59,			// LshiftZXCV..
 0x14, 0x1F, 0x11, 0x29, 0x11, 0x27, 0x2F, 0x14, 0x76, 								// Lctrl...

 0x05, 0x06, 0x04, 0x0C, 0x03, 0x0B, 0x83, 0x0A, 0x01, 0x09, 0x78, 0x07,			// F1-F12
 0x77, 0x7B, 0x4A, 0x71, 0x7C, 0x79, 0x5A,											// Num-/.*..
 0x70, 0x69, 0x72, 0x7A, 0x6B, 0x73, 0x74, 0x6C, 0x75, 0x7D,						// KP01234..

 0x70, 0x6C, 0x7D, 0x71, 0x69, 0x7A,												// INS HOME ...

 0x75, 0x6B, 0x72, 0x74,															// U L D R

 0x00, 0x7E, 0x00,																	// Prtscr,..

 0xF1, 0xF2, 0x37, 0x3F, 0x5E, 0x48,
 0x10, 0x3A, 0x38, 0x30, 0x28, 0x20, 0x18, 0x4D, 0x15, 0x3B,
 0x34, 0x23, 0x32, 0x21, 0x50, 0x2B, 0x40, 0x4B, 0x1C, 0x43,
 0x22, 0x1D 
};

#define KFLA_EXTEND 		0x01
#define KFLA_SPECIAL		0x02
#define KFLA_MAKEONLY		0x04
#define KFLA_MAKE_BREAK		0x08
#define KFLA_PROC_SHIFT		0x10

// key information for each keys
static uint8_t KFLA[NUM_KEY];

const uint8_t PROGMEM keycode_set2_special[] = 
{ 	KEY_PRNSCR, KEY_PAUSE,
	KEY_NONE };

const uint8_t PROGMEM keycode_set2_makeonly[] = 
{ 	KEY_PAUSE, KEY_HANGLE, KEY_HANJA,
	KEY_NONE };

const uint8_t PROGMEM keycode_set2_make_break[] =
{ 	KEY_POWER, KEY_SLEEP, KEY_WAKE,
	KEY_NONE };

const uint8_t PROGMEM keycode_set2_extend[] =
{	KEY_LGUI, KEY_RCTRL, KEY_RGUI, KEY_RALT, KEY_APPS, KEY_PRNSCR,
	KEY_INSERT, KEY_HOME, KEY_PGUP, KEY_DEL, KEY_END, KEY_PGDN, 
	KEY_UP, KEY_LEFT, KEY_RIGHT, KEY_DOWN, KEY_KP_SLASH, KEY_KP_ENTER,
	KEY_POWER, KEY_SLEEP, KEY_WAKE, KEY_EMAIL, KEY_WWW_SEARCH, KEY_WWW_HOME,
	KEY_WWW_BACK, KEY_WWW_FORWARD, KEY_WWW_STOP, KEY_WWW_REFRESH, KEY_WWW_FAVORITE,
	KEY_NEXT_TRK, KEY_PREV_TRK, KEY_STOP, KEY_PLAY, KEY_MUTE, KEY_VOL_UP, 
	KEY_VOL_DOWN, KEY_MEDIA, KEY_CALC, KEY_MYCOM, KEY_SCREENSAVE, KEY_REC,
	KEY_REWIND, KEY_MINIMIZE, KEY_EJECT, 
	KEY_NONE };

const uint8_t PROGMEM keycode_set2_proc_shift[] = 
{
	KEY_INSERT, KEY_DEL, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN, KEY_LGUI, KEY_RGUI, KEY_APPS,
 	KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_KP_SLASH,
	KEY_NONE };


const uint8_t PROGMEM keymap_code[2][17][8] =  {
	%%%KEYMAP%%%
};

#endif
