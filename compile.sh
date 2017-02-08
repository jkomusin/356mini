cd hex
find ./ -type f -mtime +1 | xargs rm
cd ..
cp keymap.h ./firm/keymap.h
cd firm
make clean
make
cp keymain.hex ../hex/keymain_$1.hex
cd ..
