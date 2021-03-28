//
// Created by Stefan Szaniszlo on 28/03/2021.
//

#include <Arduino.h>

#include "MAX7219.h"

#define CLOCK_DELAY_MS 0

uint8_t _MAX7219_CS = 2;
uint8_t _MAX7219_CLK = 3;
uint8_t _MAX7219_DATA = 4;

// if this is set we are directly driving segments
#define MAX7219_DIRECT_DRIVE

#ifdef MAX7219_DIRECT_DRIVE
#define MAX7219_BLANK 0x00
#else
#define MAX7219_BLANK 0x0f
#endif

const uint8_t _segments[16] = {
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
        0b0001101, // CHARLIE 'c'
//        0b1001110, // CHARLIE 'C'
        0b0111101, // DELTA
        0b1001111, // ECHO
        0b1000111  // FOXTROT
};

void _shift_out_msb(uint8_t val) // rising edge of clock grabs the value
{
    uint8_t i;
    for (i = 0; i < 8; i++)  {
        digitalWrite(_MAX7219_CLK, LOW);
        delay(CLOCK_DELAY_MS);

        digitalWrite(_MAX7219_DATA, (val & 0x80) != 0);
        val <<= 1;

        digitalWrite(_MAX7219_CLK, HIGH); //shift happens
        delay(CLOCK_DELAY_MS);
    }
}

void MAX7219_send_cmd(uint16_t cmd) {
    digitalWrite(_MAX7219_CS, LOW);
    delay(CLOCK_DELAY_MS);
    _shift_out_msb((cmd >> 8) & 0xFF);
    _shift_out_msb(cmd & 0xFF);
    digitalWrite(_MAX7219_CS, HIGH);
    delay(CLOCK_DELAY_MS);
}

void MAX7219_init(uint8_t cs_pin, uint8_t clk_pin, uint8_t data_pin, uint8_t intensity) {
    _MAX7219_CS = cs_pin;
    _MAX7219_CLK = clk_pin;
    _MAX7219_DATA = data_pin;

    // disable chip
    digitalWrite(_MAX7219_CS, HIGH);
    digitalWrite(_MAX7219_DATA, LOW);
    digitalWrite(_MAX7219_CLK, LOW);

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

    MAX7219_send_cmd(0x0c01);
}

void MAX7219_test() {
    MAX7219_send_cmd(0x0f01); // switch test bit on
    delay(500);
    MAX7219_send_cmd(0x0f00); // switch test bit off
}

void MAX7219_clear() {
    for (uint16_t pos = 0; pos < 8; pos++) {
        MAX7219_send_cmd( ( (pos+1) << 8) + MAX7219_BLANK); //digit 0 is at address 1
    }
}

void MAX7219_write(uint8_t position, uint8_t value) {
    position++;
    uint16_t cmd = position > 8 ? position - 8 : position; // wrap around position
#ifdef MAX7219_DIRECT_DRIVE
    // use segment lookup table and show dot if but 7 is set
    value = _segments[(int)(value & 0xff)] + (value & 0x80);
#endif
    MAX7219_send_cmd((cmd <<  8) + value);
}
