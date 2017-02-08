cp keymap.h ./firm_16/keymap.h
cd firm_16
make clean
make
cp keymain.hex ../hex/keymain_$1.hex
cd ..
