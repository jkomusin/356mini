# 356 Mini Firmware GUI

GUI tool for generating keymappings and firmware for the 356 Mini keyboard

### Credits

This GUI tool was developed by Sunjun Kim and is rehosted here for use by the wider community

356 Mini keyboard designed and produced by Eungsam of OTD.kr


## Usage

*NOTE* The following steps were tested on Windows 11 with an Atmel-ICE Basic for an Atmega32 version of the 356 Mini.
They will almost certainly work on other operating systems with other AVR ISPs and for the Atmega16 version of the 356 Mini in a similar fashion.

This is a fairly old package and currently not all functionality of the GUI works. Adobe Flash is unsupported in all major browsers, but even prior
to its end-of-life the following functionality was broken:

* GUI generation of keymap.h ("Export" functionality in the GUI)
* GUI generation of firmware from exported keymapping ("Make Firm" functionality in the GUI)
* Compilation does not work on newest versions of arv-gcc, arv-libc like those installed via homebrew on OSX

**As a reminder this software is provided as-is and the authors are not responsible for damage done through its use.**

Much of this will likely seem familiar if your are comfortable with ps2avr -- newer versions of ps2avr than the one included may or may not be compatible.


### Prerequisites - Windows

1. Setup AVR-GCC toolchain
  * This is probably easiest via WinAVR
  * NOTE: It looks like new versions of arv-libc might not be compatible with the current code, but WinAVR-20100110 has been used successfully in the past
  * Install the latest AVRDUDE distribution if you need access to newer programmers' configuration
2. Setup make
  * Install [Chocolatey](https://chocolatey.org/)
  * From within an administrative Powrshell, do `choco install make`


### Generating keymap.h

keymap.h is the header file which defines the keymapping you'd like to bake into the firmware. It is pulled in by the compile scripts by name.

An example is included, as well as keymap_template.h which is the template the GUI uses to generate
a keymap and the template we will use to create our new version of keymap.h

0. Install [Ruffle](https://ruffle.rs/) or another Flash emulator
1. Open Ruffle and load `356miniMapper.swf`
2. Open index.html in a browser and using the legend at the footer of the page to indicate keys, create a mapping for the base and function layer by clicking and typing in each key within the Flash UI loaded in Ruffle.
3. Click "Save" above the base layer diagram in Ruffle and save the .3mn file
4. Copy keymap_template.h to keymap.h and replace the placeholder string %%%KEYMAP%%% with the entire contents of the .3mn file generated in the previous step.
  * NOTE: You may receive a syntax error later like `error: expected '}' before '{' token`. If this is the case, check that there is a trailing comma after each layer section in the keymap. This might be due to a bug with Flash emulation, it's unclear.

Alternatively, you can edit the keymap.h directly, the key codes are listed at the top of the file and are relatively self-explanatory. But the UI is nice to help generating the matrix.


### Generating keymain.hex

keymain.hex is the baked firmware that you will flash to the keyboard to implement your keymapping.

There is an example .hex file provided in the `hex` directory. Please note this example .hex file is for the Atmega32 version of the 356 Mini, NOT the Atmega16 version.

To generate keymain.hex first ensure that the keymapping you'd like to bake in is present in a keymain.h file in the base directory (where the provided example is).
Then run the appropriate compiling script:

* For the Atmega32 version of the 356 Mini, run `./compile.sh NAME` where 'NAME' is a unique name you'd like to give to the keymain.hex file
* For the Atmega16 version of the 356 Mini, run `./compile_16.sh NAME` where 'NAME' is a unique name you'd like to give to the keymain.hex file

On success, a new keymain_NAME.hex file will be placed in the `hex` directory.

Alternatively, both the `firm` and `firm_16` directories makefiles can be run manually via `make clean` followed by `make`. They will generate keymain.hex within the `firm` or `firm_16` directory.


### Backup existing firmware

It is *highly* recommended that you backup any firmware existing on your 356 Mini before attempting to flash a new version. Doing so varies by toolchain, but for avrdude on an ATMEGA 32 356mini using an Arduino ISP connected via USB:

`avrdude -p atmega32 -P usb -c arduino -b 19200 -U flash:r:backup.hex:i`

For the ATMEL ICE used to test a lot of this (requires a newer AVRDUDE distribution):

`.\avrdude.exe -p atmega32 -c atmelice_isp -b 19200 -U flash:r:backup.hex:i`

replacing `com3` with the appropriate interface and `arduino` with the appropriate ISP and `atmega32` with `atmega16` depending on the version of the 356 Mini. See the avrdude documentation for specifics on each flag.

Atmel Studio also provides utilities for backing up firmware which may be useful if the Windows avrdude toolchain isn't working for you, but be aware it can further complicate using avrdude.


### Flashing the 356 Mini with keymain.hex

Finally, flash your Atmega with the generated keymain.hex file. Steps to do so vary a bit between ISPs and the toolchain you've chosen. Roughly:

1. Connect your ISP to the 6 headers to the right of the spacebar switch on the PCB
2. Plug in your ISP to the host computer
3. Upload your generated keymain_NAME.hex file:
  * For avrdude, the following command should work (with the -p, -P, and -c flags changed as per the preceding backup instructions): `.\avrdude.exe -p atmega32 -P usb -c arduino -b 19200 -U flash:w:your_new_keymain.hex:i`
  * Atmel Studio also provides utilities for flashing firmware which may be useful if the Windows avrdude toolchain isn't working for you, but be aware it can further complicate using avrdude
4. Congratulations, your 356 Mini has been remapped!


## Contributing

We welcome any contributions to fix broken functionality, document code, or implement new functionality.

Feel free to fork and submit a pull request, or create an Issue if you'd like to discuss your intentions.

Thank you!

