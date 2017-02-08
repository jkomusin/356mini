avrdude -c USBasp -P usbasp -p atmega32 -U hfuse:w:0xC7:m -U lfuse:w:0x0E:m
avrdude -c USBasp -P usbasp -p atmega32 -U flash:w:356_mini_keymain_20100906.hex:i
pause;
