#!/bin/sh

QMK_PATH=./qmk_firmware
CRKBD_PATH=$QMK_PATH/keyboards/crkbd
KEYMAP_NAME=sergio

git submodule update --recursive
sed -i 's/B1/B2/' $CRKBD_PATH/info.json
mkdir $CRKBD_PATH/keymaps/sergio 2>/dev/null
cp keymap.c config.h rules.mk $CRKBD_PATH/keymaps/$KEYMAP_NAME
cd $QMK_PATH
qmk compile -kb crkbd/rev1 -km $KEYMAP_NAME -e CONVERT_TO=stemcell -e STMC_US=yes
cp $QMK_PATH/.build/crkbd_rev1_${KEYMAP_NAME}_stemcell.uf2 firmware.uf2
