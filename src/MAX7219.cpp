/*
    MIT License

    Copyright (c) 2021 Briskware Ltd

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#include <Arduino.h>

#include "MAX7219.h"

/**
 * slow down the serial communications (the module only accepts serial bits at speeds <10MHz)
 */
#define MAX7219_CLOCK_DELAY_US 0

/**
 * if this is set we are directly driving segments
 */
#define MAX7219_DIRECT_DRIVE

#ifdef MAX7219_DIRECT_DRIVE
#define _MAX7219_BLANK 0x00
#else
#define _MAX7219_BLANK 0x0f
#endif

// private config (state), use _max7219_init() to initialize
uint8_t _MAX7219_CS = 2;
uint8_t _MAX7219_CLK = 3;
uint8_t _MAX7219_DATA = 4;


// 7 seg display driver characters, common cathode configuration
const uint8_t _max7219_segments[16] = {
        //abcdefg
        0b1111110, // ZERO
        0b0110000, // ONE
        0b1101101, // TWO
        0b1111001, // THREE
        0b0110011, // FOUR
        0b1011011, // FIVE
        0b1011111, // SIX
        0b1110000, // SEVEN
        0b1111111, // EIGHT
        0b1111011, // NINE
        0b1110111, // ALPHA
        0b0011111, // BRAVO
        0b0001101, // CHARLIE 'c'  or use 0b1001110 for CHARLIE 'C'
        0b0111101, // DELTA
        0b1001111, // ECHO
        0b1000111  // FOXTROT
};

void _max7219_clock_delay() {
    delayMicroseconds(MAX7219_CLOCK_DELAY_US);
}

void _max7219_shift_out_msb(uint8_t val) // rising edge of clock grabs the value
{
    uint8_t i;
    for (i = 0; i < 8; i++)  {
        digitalWrite(_MAX7219_CLK, LOW);
        _max7219_clock_delay();

        digitalWrite(_MAX7219_DATA, (val & 0x80) != 0);
        val <<= 1;

        digitalWrite(_MAX7219_CLK, HIGH); //shift happens
        _max7219_clock_delay();
    }
}

void MAX7219_send_cmd(uint16_t cmd) {
    digitalWrite(_MAX7219_CS, LOW);
    _max7219_clock_delay();
    _max7219_shift_out_msb((cmd >> 8) & 0xFF);
    _max7219_shift_out_msb(cmd & 0xFF);
    digitalWrite(_MAX7219_CS, HIGH); // on CS rising edge the command is latched
    _max7219_clock_delay();
}

void MAX7219_init(uint8_t cs_pin, uint8_t clk_pin, uint8_t data_pin, uint8_t intensity) {
    _MAX7219_CS = cs_pin;
    _MAX7219_CLK = clk_pin;
    _MAX7219_DATA = data_pin;

    // initialize pins
    pinMode(_MAX7219_CS, OUTPUT);
    pinMode(_MAX7219_CLK, OUTPUT);
    pinMode(_MAX7219_DATA, OUTPUT);

    // disable chip
    digitalWrite(_MAX7219_CS, HIGH);
    digitalWrite(_MAX7219_DATA, LOW);
    digitalWrite(_MAX7219_CLK, LOW);

    // set shutdown mode
    MAX7219_shutdown();

    // clear the digits
    MAX7219_clear();

#ifdef MAX7219_DIRECT_DRIVE
    // set decode mode to explicit, we will drive the segments directly
    MAX7219_send_cmd(0x0900);
#else
    // set decode mode to "code B", i.e. [0-9EHLP\-]
    MAX7219_send_cmd(0x09FF);
#endif

    // set_intensity
    MAX7219_send_cmd(0x0a00 + (intensity % 8));

    // set scan limit to show all 8 digits
    MAX7219_send_cmd(0x0b07);

    // set normal operation flag in the shutdown register
    MAX7219_send_cmd(0x0c01);
}

void MAX7219_shutdown() {
    // set shutdown flag in the shutdown register
    MAX7219_send_cmd(0x0c00);
}

void MAX7219_test() {
    MAX7219_send_cmd(0x0f01); // switch test bit on
    delay(500);
    MAX7219_send_cmd(0x0f00); // switch test bit off
    delay(500);
}

void MAX7219_clear() {
    for (uint16_t pos = 0; pos < 8; pos++) {
        MAX7219_clear(pos);
    }
}

void MAX7219_clear(uint8_t position) {
    MAX7219_send_cmd(( ((position & 0x7) + 1) << 8) + _MAX7219_BLANK); //digit 0 is at address 1
}

void MAX7219_write(uint8_t position, uint8_t value) {
    position++;
    uint16_t cmd = position > 8 ? position - 8 : position; // wrap around position
#ifdef MAX7219_DIRECT_DRIVE
    // use segment lookup table and show dot if but 7 is set
    value = _max7219_segments[(int)(value & 0x7f)] + (value & 0x80);
#endif
    MAX7219_send_cmd((cmd <<  8) + value);
}
