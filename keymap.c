 /*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>
Copyright 2024 Sergio Miguéns Iglesias <sergio@lony.xyz>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <keymap_spanish.h>

//                                                                   CUSTOM KEYS
////////////////////////////////////////////////////////////////////////////////

// List of custom keycodes.
enum custom_keycodes {
    LN_SPC = SAFE_RANGE, LN_BRC, LN_CBR,
    LN_ADD,
    LN_SLH,
    LN_EXCL,
    LN_QSTN,
    LN_QUOT,
    LN_2, LN_3, LN_4, LN_7, LN_8, LN_9,
    LN_DOT, LN_DDOT,
    LN_DLR,
    LN_TAB
};

// Press or release RALT or LSFT.
void press_mode(uint8_t mode, bool pressed) {
    switch (mode) {
        case 1:
            pressed ? register_code(KC_LSFT) : unregister_code(KC_LSFT);
            break;
        case 2:
            pressed ? register_code(KC_RALT) : unregister_code(KC_RALT);
            break;
        default:
            break;
    }
}

// Autoshift function for keys allowing to create custom keys.
void press_hold(keyrecord_t *record, uint16_t tap_keycode, uint16_t hold_keycode, uint16_t *timer, uint8_t mode_tap, uint8_t mode_hold) {
    if (record->event.pressed) {
        *timer = timer_read();
    } else {
        if (timer_elapsed(*timer) < TAPPING_TERM) {
            press_mode(mode_tap, true);
            tap_code(tap_keycode);
            press_mode(mode_tap, false);
        } else {
            press_mode(mode_hold, true);
            tap_code(hold_keycode);
            press_mode(mode_hold, false);
        }
    }
}

// Custom macro that sets up a timer for a key and calls the press_hold function
// for it.
#define key_case(name, key_tap, key_hold, mode_tap, mode_hold)\
    static uint16_t _##name##_timer;\
    if (keycode == name)\
        press_hold(record,\
            key_tap, key_hold,\
            &_##name##_timer,\
            mode_tap, mode_hold);

// Setting these custom keys is a really bad solution.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    key_case(LN_SPC,  ES_8,    ES_9,    1, 1); // ( )
    key_case(LN_BRC,  ES_GRV,  ES_PLUS, 2, 2); // [ ]
    key_case(LN_CBR,  ES_ACUT, ES_CCED, 2, 2); // { }
    key_case(LN_ADD,  ES_PLUS, ES_PLUS, 0, 1); // + *
    key_case(LN_SLH,  ES_7,    ES_MORD, 1, 2); // / /
    key_case(LN_EXCL, ES_1,    ES_IEXL, 1, 0); // ! ¡
    key_case(LN_QSTN, ES_QUOT, ES_IEXL, 1, 1); // ? ¿
    key_case(LN_QUOT, ES_QUOT, ES_2,    0, 1); // ' "
    key_case(LN_DLR,  ES_4,    ES_E,    1, 2); // : ;
    key_case(LN_TAB,  KC_TAB,  KC_TAB,  0, 1); // \t \t

    key_case(LN_2,    ES_2,    ES_4,    0, 2); // 2 ~
    key_case(LN_3,    ES_3,    ES_1,    0, 2); // 3 |
    key_case(LN_4,    ES_4,    ES_6,    0, 1); // 4 &
    key_case(LN_7,    ES_7,    ES_0,    0, 1); // 7 =
    key_case(LN_8,    ES_8,    ES_2,    0, 2); // 8 @
    key_case(LN_9,    ES_9,    ES_3,    0, 2); // 9 #
    return true;
};

//                                                                 CUSTOM LAYERS
////////////////////////////////////////////////////////////////////////////////

// Disable autoshift on gaming layer.
layer_state_t layer_state_set_user(layer_state_t state) {
    if (get_highest_layer(state) == 2)
        autoshift_disable();
    else
        autoshift_enable();
    return state;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // Letters layout: based on DVORAK.
    [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       LN_TAB, ES_COMM,  ES_DOT, ES_NTIL,    ES_P,    ES_Y,                         ES_F,    ES_G,    ES_C,    ES_H,    ES_L,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_BSPC,    ES_A,    ES_O,    ES_E,    ES_U,    ES_I,                         ES_D,    ES_R,    ES_T,    ES_N,    ES_S, ES_ACUT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, ES_MINS,    ES_Q,    ES_J,    ES_K,    ES_X,                         ES_B,    ES_M,    ES_W,    ES_V,    ES_Z, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX,  KC_SPC, KC_LWIN,      MO(1),  KC_ENT, S(KC_ENT)
                                      //`--------------------------'  `--------------------------'
  ),

    // Symbols layout: numbers, symbols and arrow keys.
    [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______,    KC_1,    LN_2,    LN_3,    LN_4,    KC_5,                         KC_6,    LN_7,    LN_8,    LN_9,    KC_0, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_DEL, XXXXXXX, LN_QUOT,  LN_SLH,  LN_ADD,  LN_DLR,                      XXXXXXX, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT,  ES_GRV,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, ES_LABK, LN_QSTN, LN_EXCL, XXXXXXX,                      XXXXXXX,  LN_SPC,  LN_BRC,  LN_CBR, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, _______, _______,    _______,   TG(2), XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),

    // Gaming layout: all keys are QWERTY and have no automatic shift. The right
    // hand side is used to switch back to the other layouts.
    [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
         ES_1,  KC_TAB,    ES_Q,    ES_W,    ES_E,    ES_R,                        TG(2),   TG(2),   TG(2),   TG(2),   TG(2),   TG(2),
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
         ES_2, KC_LSFT,    ES_A,    ES_S,    ES_D,    ES_F,                        TG(2),   TG(2),   TG(2),   TG(2),   TG(2),   TG(2),
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
         ES_3, KC_LCTL,    ES_Z,    ES_X,    ES_C,    ES_V,                        TG(2),   TG(2),   TG(2),   TG(2),   TG(2),   TG(2),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                           KC_SPC,  KC_SPC,  KC_SPC,      TG(2),  TG(2),   TG(2)
                                      //`--------------------------'  `--------------------------'
  ),
};
