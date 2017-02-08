/* Copyright Jamie Honan, 2001.  Distributed under the GPL.
   This program comes with ABSOLUTELY NO WARRANTY.
   See the file COPYING for license details.
   */
#define KEYBD_EXTERN
#include "timer.h"
#include "keysta.h"
#include "keymap.h"
#include "print.h"


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>



/* This controls what to do when a character has been recieved from the
   host, and when we want to send a character to the host. Note that
   replies are driven by a state machine.
   */

#define STA_NORMAL		0
#define STA_RXCHAR		1
#define STA_WAIT_SCAN_SET	2
#define STA_WAIT_SCAN_REPLY	3
#define STA_WAIT_ID		4
#define STA_WAIT_ID1		5
#define STA_WAIT_LEDS		6
#define STA_WAIT_AUTOREP	7
#define STA_WAIT_RESET		8
#define STA_DISABLED		9
#define STA_DELAY           11
#define STA_REPEAT        12

#define SC_EXTENDED_CODE     0xE0

#define SC_0 0x45
#define SC_1 0x16
#define SC_2 0x1E
#define SC_3 0x26
#define SC_4 0x25
#define SC_5 0x2E
#define SC_6 0x36
#define SC_7 0x3D
#define SC_8 0x3E
#define SC_9 0x46
#define SC_ENTER 0x5A

#define START_MAKE 0xFF
#define END_MAKE   0xFE
#define NO_REPEAT  0xFD
#define SPLIT      0xFC

#define M_LALT  	0x01
#define M_LSHIFT	0x02
#define M_LCTRL		0x04
#define M_LGUI		0x08
#define M_RALT		0x10
#define M_RSHIFT	0x20
#define M_RCTRL		0x40
#define M_RGUI		0x80

// Output buffer - circular queue
#define QUEUE_SIZE 200
static uint8_t QUEUE[QUEUE_SIZE];
static int rear=0, front=0;

static uint8_t lastMAKE_keyidx;
static uint8_t lastMAKE[10];
static uint8_t lastMAKE_SIZE=0;
static uint8_t lastMAKE_IDX=0;
static long loopCnt;

//static uint8_t modifier_mask=0;

// 17*8 bit matrix
static uint8_t MATRIX[17];

static uint8_t TYPEMATIC_DELAY=2;
static long TYPEMATIC_REPEAT=5;

unsigned char txScanCode = 0; // scancode being urrently transmitted
unsigned char m_state;
unsigned char lastSent;
unsigned char lastState;

//static uint16_t MACRO_IDX=0;

// Queue operation -> push, pop
void push(uint8_t item) {
	static uint8_t record=0;

	if(item==START_MAKE) {
		lastMAKE_SIZE=0;
		record=1;
		return;
	}
	if(item==END_MAKE) {
		record=0;
		return;
	}
	if(item==NO_REPEAT) {
		lastMAKE_SIZE=0;
		record=0;
		return;
	}

	if(record)
		lastMAKE[lastMAKE_SIZE++] = item;

	rear = (rear+1)%QUEUE_SIZE;
	if(front==rear) {
		rear = (rear!=0) ? (rear-1):(QUEUE_SIZE-1);
		return;
	}
	QUEUE[rear] = item;
}

uint8_t pop(void) {
	if(front==rear) {
		return 0;
	}
	front = (front+1)%QUEUE_SIZE;

	return QUEUE[front];
}

	uint8_t isEmpty(void) {
		if(front==rear)
			return 1;
		else
			return 0;
	}

void clear(void) {
	int i;
	rear = front = 0;
	lastMAKE_SIZE=0;
	lastMAKE_IDX=0;
	loopCnt=0;

	for(i=0;i<17;i++) 
		MATRIX[i] = 0x00;
}

// this is for debugging
void printState(uint8_t state_number) {
	switch(state_number) {
		case STA_NORMAL:
			DEBUG_PRINT(("NORMAL"));
			break;
		case STA_RXCHAR:
			DEBUG_PRINT(("RXCHAR"));
			break;
		case STA_WAIT_SCAN_SET:
			DEBUG_PRINT(("WAIT_SCAN_SET"));
			break;
		case STA_WAIT_SCAN_REPLY:
			DEBUG_PRINT(("WAIT_SCAN_REPLY"));
			break;
		case STA_WAIT_ID:
			DEBUG_PRINT(("WAIT_ID"));
			break;
		case STA_WAIT_ID1:
			DEBUG_PRINT(("WAIT_ID1"));
			break;
		case STA_WAIT_LEDS:
			DEBUG_PRINT(("WAIT_LEDS"));
			break;
		case STA_WAIT_AUTOREP:
			DEBUG_PRINT(("WAIT_AUTOREP"));
			break;
		case STA_WAIT_RESET:
			DEBUG_PRINT(("WAIT_RESET"));
			break;
		case STA_DISABLED:
			DEBUG_PRINT(("DISABLED"));
			break;
		case STA_DELAY:
			DEBUG_PRINT(("DELAY"));
			break;
		case STA_REPEAT:
			DEBUG_PRINT(("REPEAT"));
			break;
		default:
			DEBUG_PRINT(("UNDEFINED - %d", state_number));
	}
}

void tx_state(unsigned char x, unsigned char newstate)
{
	//DEBUG_PRINT(("TX : %02x\n", x));
	if(x != 0xFE)
		lastSent=x;
	kbd_set_tx(x);
	m_state = newstate;
}

void keymap_init(void) 
{
	int i, keyidx;

	// set zero for every flags
	for(i=0;i<NUM_KEY;i++)
		KFLA[i]=0;

	// set flags
	for(i=0;(keyidx=pgm_read_byte(&keycode_set2_special[i]))!=KEY_NONE;i++)
		KFLA[keyidx] |= KFLA_SPECIAL;
	for(i=0;(keyidx=pgm_read_byte(&keycode_set2_makeonly[i]))!=KEY_NONE;i++)
		KFLA[keyidx] |= KFLA_MAKEONLY;
	for(i=0;(keyidx=pgm_read_byte(&keycode_set2_make_break[i]))!=KEY_NONE;i++)
		KFLA[keyidx] |= KFLA_MAKE_BREAK;
	for(i=0;(keyidx=pgm_read_byte(&keycode_set2_extend[i]))!=KEY_NONE;i++)
		KFLA[keyidx] |= KFLA_EXTEND;
	for(i=0;(keyidx=pgm_read_byte(&keycode_set2_proc_shift[i]))!=KEY_NONE;i++)
		KFLA[keyidx] |= KFLA_PROC_SHIFT;


	for(i=0;i<17;i++) 
		MATRIX[i]=0;
}


// push the keycodes into the queue by its key index, and isDown
void putKey(uint8_t keyidx, uint8_t isDown) {
	if(keyidx == KEY_FN)
		return;
	// if prev and current state are different,
	uint8_t keyVal = pgm_read_byte(&keycode_set2[keyidx]);

	if(isDown)
		DEBUG_PRINT(("[%02x] PUSHED\n", keyVal));
	else
		DEBUG_PRINT(("[%02x] RELEASED\n", keyVal));

	if(isDown) {		// make code
		/* this is for macro
		   if(keyidx == KEY_LALT)		modifier_mask |= M_LALT;
		   if(keyidx == KEY_LSHIFT)	modifier_mask |= M_LSHIFT;
		   if(keyidx == KEY_LCTRL)		modifier_mask |= M_LCTRL;
		   if(keyidx == KEY_LGUI)		modifier_mask |= M_LGUI;
		   if(keyidx == KEY_RALT)		modifier_mask |= M_RALT;
		   if(keyidx == KEY_RSHIFT)	modifier_mask |= M_RSHIFT;
		   if(keyidx == KEY_RCTRL)		modifier_mask |= M_RCTRL;
		   if(keyidx == KEY_RGUI)		modifier_mask |= M_RGUI;
		   */


		lastMAKE_keyidx = keyidx;
		loopCnt=0;
		m_state = STA_NORMAL;

		if(KFLA[keyidx]&KFLA_SPECIAL) {
			switch(keyidx) {
				case KEY_PRNSCR:
					push(START_MAKE);
					push(0xE0);
					push(0x12);
					push(0xE0);
					push(0x7C);
					push(END_MAKE);
					push(SPLIT); // SPLIT is for make sure all key codes are transmitted before disturbed by RX
					break;
				case KEY_PAUSE:
					push(NO_REPEAT);
					push(0xE1);
					push(0x14);
					push(0x77);
					push(0xE1);
					push(0xF0);
					push(0x14);
					push(0xF0);
					push(0x77);
					push(SPLIT);
					break;
			}
		}
		else 
		{
			push(START_MAKE);
			if(KFLA[keyidx]&KFLA_EXTEND) 
				push(0xE0);
			push(keyVal);

			push(END_MAKE);
			push(SPLIT);
		}
	}
	else			// break code - key realeased
	{
		/* this is for macro
		   if(keyidx == KEY_LALT)		modifier_mask &=~M_LALT;
		   if(keyidx == KEY_LSHIFT)	modifier_mask &=~M_LSHIFT;
		   if(keyidx == KEY_LCTRL)		modifier_mask &=~M_LCTRL;
		   if(keyidx == KEY_LGUI)		modifier_mask &=~M_LGUI;					
		   if(keyidx == KEY_RALT)		modifier_mask &=~M_RALT;
		   if(keyidx == KEY_RSHIFT)	modifier_mask &=~M_RSHIFT;
		   if(keyidx == KEY_RCTRL)		modifier_mask &=~M_RCTRL;
		   if(keyidx == KEY_RGUI)		modifier_mask &=~M_RGUI;
		   */
		if(lastMAKE_keyidx == keyidx)		// repeat is resetted only if last make key is released
			lastMAKE_SIZE=0;

		if(KFLA[keyidx]&KFLA_SPECIAL) {
			switch(keyidx) {
				case KEY_PRNSCR:
					push(0xE0);
					push(0xF0);
					push(0x7C);
					push(0xE0);
					push(0xF0);
					push(0x12);
					push(SPLIT);
					break;
			}
		}
		else 
		{
			if(KFLA[keyidx]&KFLA_EXTEND)
				push(0xE0);

			push(0xF0);
			push(keyVal);
			push(SPLIT);
		}
	}
}

// function that determine keymap
uint8_t get_keymap(void) {
	uint8_t col, row, keyidx, cur;
	for(col=0;col<8;col++)
		for(row=0;row<17;row++) {
			keyidx = pgm_read_byte(&keymap_code[0][row][col]);
			if(keyidx == KEY_FN) {
				cur = 0;
				DDRB  = BV(col);
				PORTB = ~BV(col);

				delay_us(1);

				if(row<8)	{				// for 0..7, PORTA 0 -> 7
					cur = (~PINA)&BV(row);
				}
				else if(row>=8 && row<16) {	// for 8..15, PORTC 7 -> 0
					cur = (~PINC)&BV(15-row);
				}
				else {						// for 16..18, PORTD 7 -> 5
					cur = (~PIND)&BV(23-row);
				}

				if(cur)
					return 1;
			}
		}
	uint8_t keymap = ((~PIND)&0b01000000)>>6;

	if(keymap)
		return 1;
	else
		return 0;
}

// return : key modified
int scankey(void) {
	uint8_t col, row;
	uint8_t prev, cur;
	uint8_t keyidx;
	static int debounce=0;
	static uint8_t curMATRIX[17];
	//static uint16_t macro_count=0;
	static uint8_t svkeyidx[17][8];

	int isModified = 0;

	// scan matrix 
	int numKey=0;

	for(col=0;col<8;col++)
	{
		// Col -> set only one port as input and all others as output low
		DDRB  = BV(col);
		PORTB = ~BV(col);

		delay_us(1);

		// scan each rows
		for(row=0;row<17;row++)
		{
			if(row<8)	{				// for 0..7, PORTA 0 -> 7
				cur = (~PINA)&BV(row);
			}
			else if(row>=8 && row<16) {	// for 8..15, PORTC 7 -> 0
				cur = (~PINC)&BV(15-row);
			}
			else {						// for 16..18, PORTD 7 -> 5
				cur = (~PIND)&BV(23-row);
			}

			if(cur)
				numKey++;

			prev = curMATRIX[row]&BV(col);

			if(!(prev&&cur) && !(!prev&&!cur)) {
				if(cur)
					curMATRIX[row] |= BV(col);
				else
					curMATRIX[row] &=~ BV(col);

				isModified = 1;
			}
		}
	}

	static int pwmCounter=0;
	static int pwmDir = 0;
	const int speed=5;

	/* LED Fader */
	//if(keymap==1 || keymap==3) {
	if(1) {
		cbi(LED_SCR_PORT, LED_SCR_PIN);

		timer1PWMBOn();

		if(pwmDir==0)
			timer1PWMBSet((uint16_t)(pwmCounter/speed));
		else if(pwmDir==2)
			timer1PWMBSet((uint16_t)(255-pwmCounter/speed));
		else if(pwmDir==1 || pwmDir==3)
			pwmCounter++;

		// pwmDir 0~3 : idle
		if(pwmDir < 4 && pwmCounter>=255*speed) {
			pwmCounter=0;
			pwmDir = (pwmDir+1)%4;
		}
		// pwmDir 4 : key pressed : turn off for a while
		else if(pwmDir==4) {
			// after short period, turn on back.
			if(pwmCounter==10) {
				timer1PWMBSet((uint16_t)255);
				pwmDir=5;
			}
		}
		else if(pwmCounter>=255*speed*2) {
			if(numKey==0) {
				pwmDir=2;
			}

			pwmCounter=0;
		}
		pwmCounter++;

		if (kbd_flags & FLA_RX_BAD) {		// pokud je nastaveny flag spatneho prijmu, zrus ho 
			// pokud flag is set back income withdrawn
			cli();
			kbd_flags &= ~FLA_RX_BAD;
			sei();
		}
	}
	else
		timer1PWMBOff();

	if(isModified)
		debounce=0;
	else if(debounce<100)	// to prevent going over limit of int
		debounce++;

	static uint8_t prev_keymap=0;
	uint8_t keymap = get_keymap();

	// keymap changes detect
	if(prev_keymap != keymap) {
		DEBUG_PRINT(("KEYMAP CHANGED FROM %d - %d, lastMAKE_SIZE = %d, lastMAKE_keyidx = %d\n", prev_keymap, keymap, lastMAKE_SIZE, lastMAKE_keyidx));
	}

	if(debounce!=5)
		return 0;

	// LED Fader
	//if(keymap==1 || keymap==3) {
	if(1) {
		timer1PWMBSet((uint16_t)0);
		pwmCounter=0;
		pwmDir=4;
	}

	// debounce cleared => compare last matrix and current matrix
	for(col=0;col<8;col++)
	{
		for(row=0;row<17;row++)
		{
			prev = MATRIX[row]&BV(col);
			cur  = curMATRIX[row]&BV(col);
			keyidx = pgm_read_byte(&keymap_code[keymap][row][col]);

			if(!(prev&&cur) && !(!prev&&!cur) && keyidx!=KEY_NONE) {
				if(cur) {
					putKey(keyidx, 1);
					svkeyidx[row][col] = keyidx;
				}
				else {
					putKey(svkeyidx[row][col], 0);
				}
			}	
		}
	}

	for(row=0;row<17;row++) 
		MATRIX[row] = curMATRIX[row];
	prev_keymap = keymap;

	return 1;
}

int main(void)
{

	// initialize matrix ports - cols, rows
	// PB0-PB7 : col1 .. col8
	// PA0-PA7 : row1 .. row8
	// PC7-PC0 : row9 .. row16
	// PD7 : row17

	// PD6, PD5 : Dip switch

	// signal direction : col -> row

	DDRB 	= 0xFF;	// all outputs for cols
	PORTB	= 0xFF;	// pull-up

	DDRA	= 0x00;	// all inputs for rows
	DDRC	= 0x00;
	DDRD    = 0x00;

	PORTA	= 0xFF;	// all rows pull-up.
	PORTC	= 0xFF;
	PORTD	= 0xFF;


	// initial LED pins and turn off all
	sbi(LED_CAP_DDR, LED_CAP_PIN);
	sbi(LED_NUM_DDR, LED_NUM_PIN);
	sbi(LED_SCR_DDR, LED_SCR_PIN);

	sbi(LED_CAP_PORT, LED_CAP_PIN);
	sbi(LED_NUM_PORT, LED_NUM_PIN);
	sbi(LED_SCR_PORT, LED_SCR_PIN);

	DDRD&=~0b11100000;
	PORTD	|= 0b11100000;

	timerInit();
	timer1PWMInit(8);

	enable_printf();
	DEBUG_PRINT(("STARTING KEYBOARD\n"));

	// initialize speed port
	unsigned char rxed;

	int temp_a, temp_b;
	int i, j;

	m_state = STA_WAIT_RESET;
	kbd_init();

	keymap_init();

	sei();

	while(1) {
		static int keyval=0;

#ifdef DEBUG
		static int prev_state = -1;
		if(prev_state != m_state) {
			DEBUG_PRINT(("STATE CHANGE from "));
			printState(prev_state);
			DEBUG_PRINT((" -> "));
			printState(m_state);
			DEBUG_PRINT(("\n"));
			prev_state = m_state;
		}
#endif

		// check that every key code for single keys are transmitted
		if ((kbd_flags & FLA_RX_BYTE) && (keyval==SPLIT || isEmpty())) {     // pokud nastaveny flag prijmu bytu, vezmi ho a zanalyzuj
			// pokud law, the flag setting apart, take it and zanalyzuj
			rxed = kbd_get_rx_char();		
			DEBUG_PRINT(("RX : %02x\n", rxed));

			switch(m_state) {
				default:
					switch(rxed) {
						case 0xEE: /* echo */
							tx_state(0xEE, m_state);
							continue;
						case 0xF2: /* read id */
							tx_state(0xFA, STA_WAIT_ID);
							continue;
						case 0xFF: /* reset */
							tx_state(0xFA, STA_WAIT_RESET);
							continue;
						case 0xFE: /* resend */
							tx_state(lastSent, m_state);
							continue;
						case 0xF0: /* scan code set */
							tx_state(0xFA, STA_WAIT_SCAN_SET);
							continue;
						case 0xED: /* led indicators */
							tx_state(0xFA, STA_WAIT_LEDS);
							continue;
						case 0xF3:
							tx_state(0xFA, STA_WAIT_AUTOREP);
							continue;
						case 0xF4:		// enable
							tx_state(0xFA, STA_NORMAL);//STA_RXCHAR);
							continue;
						case 0xF5:		// disable
							//clear();	// clear buffers
							//tx_state(0xFA, STA_DISABLED);
							tx_state(0xFA, STA_NORMAL);
							continue;
						case 0xF6:		// Set Default
							TYPEMATIC_DELAY=1;
							TYPEMATIC_REPEAT=5;
							clear();
						default:
							break;
					}
					tx_state(0xFA, STA_NORMAL);
					continue;
				case STA_RXCHAR:
					if (rxed == 0xF5)
						tx_state(0xFA, STA_NORMAL);
					else {
						tx_state(0xFA, STA_RXCHAR);
					}
					continue;

				case STA_WAIT_SCAN_SET:
					clear();
					tx_state(0xFA, rxed == 0 ? STA_WAIT_SCAN_REPLY : STA_NORMAL);
					continue;
				case STA_WAIT_AUTOREP:
					TYPEMATIC_DELAY = (rxed&0b01100000)/0b00100000;

					temp_a = (rxed&0b00000111);
					temp_b = (rxed&0b00011000)/(0b000001000);

					j=1;
					for(i=0;i<temp_b;i++) {
						j = j*2;
					}

					TYPEMATIC_REPEAT = temp_a*j;

					tx_state(0xFA, STA_NORMAL);
					continue;
				case STA_WAIT_LEDS:
					// Reflect LED states to PD0~2

					// scroll lock
					if(rxed&0x01)
						sbi(LED_SCR_PORT, LED_SCR_PIN);
					else
						cbi(LED_SCR_PORT, LED_SCR_PIN);

					// num lock
					if(rxed&0x02)
						sbi(LED_NUM_PORT, LED_NUM_PIN);
					else
						cbi(LED_NUM_PORT, LED_NUM_PIN);

					// capslock
					if(rxed&0x04)
						sbi(LED_CAP_PORT, LED_CAP_PIN);
					else
						cbi(LED_CAP_PORT, LED_CAP_PIN);

					tx_state(0xFA, STA_NORMAL);
					continue;
			}
		}
		if (kbd_flags & FLA_TX_OK) {   // pokud flag odesilani ok -> if the flag sent ok
			switch(m_state) {
				case STA_NORMAL:
					// if error during send
					if(isEmpty())
						scankey();

					keyval=pop();
					if(keyval==SPLIT)
						continue;

					if(keyval) {
						tx_state(keyval, STA_NORMAL);

						loopCnt=0;
					}
					else if(lastMAKE_SIZE>0) {		// means key is still pressed
						loopCnt++;

						// if key is pressed until typmatic_delay, goes to repeat the last key
						if(loopCnt >= TYPEMATIC_DELAY*150+230) {
							loopCnt=0;
							lastMAKE_IDX=0;
							m_state = STA_REPEAT;
						}
					}

					break;
					// typematic : repeat last key
				case STA_REPEAT:

					if(lastMAKE_IDX==0)	{	// key state can be escaped only if whole key scancode is transmitted
						scankey();
					}

					if(lastMAKE_SIZE==0 || !isEmpty()) {	// key is released. go to normal
						m_state=STA_NORMAL;
						loopCnt=0;
						break;
					}

					// if release key is pushed, send them.
					if(loopCnt==1 || lastMAKE_IDX!=0) {
						tx_state(lastMAKE[lastMAKE_IDX++], STA_REPEAT);
						lastMAKE_IDX %= lastMAKE_SIZE;
					}

					loopCnt++;
					loopCnt %= (3+TYPEMATIC_REPEAT*10);

					break;
				case STA_WAIT_SCAN_REPLY:
					tx_state(0x02, STA_NORMAL);
					break;
				case STA_WAIT_ID:
					tx_state(0xAB, STA_WAIT_ID1);
					break;
				case STA_WAIT_ID1:
					tx_state(0x83, STA_NORMAL);
					break;

					delay_ms(300);
				case STA_WAIT_RESET:
					sbi(LED_CAP_PORT, LED_CAP_PIN);
					sbi(LED_NUM_PORT, LED_NUM_PIN);
					sbi(LED_SCR_PORT, LED_SCR_PIN);

					clear();
					DEBUG_PRINT(("############## RESET!!!\n"));

					delay_ms(1000);

					cbi(LED_CAP_PORT, LED_CAP_PIN);
					cbi(LED_NUM_PORT, LED_NUM_PIN);
					cbi(LED_SCR_PORT, LED_SCR_PIN);

					tx_state(0xAA, STA_NORMAL);
					break;
			}
		}
	}
}
