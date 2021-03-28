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

#define CS    PIND2
#define CLK   PIND3
#define DATA  PIND4

int delay_ms = 1; //ms

uint16_t count = 0;

void write_address(uint16_t address);
void write_data(uint8_t data);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    MAX7219_init(CS, CLK, DATA, 0x02);
    MAX7219_test();
    MAX7219_clear();
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delay_ms);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delay_ms);

    write_address(count);
    write_data( count / 97);
    ++count;
}

void write_address(uint16_t address) {
    for ( uint8_t i = 4; i < 8; ++i) {
        MAX7219_write(i, (address & 0x0f) + 0x00);
        address >>= 4;
    }
}

void write_data(uint8_t data) {
    for ( uint8_t i = 0; i < 2; ++i) {
        MAX7219_write(i, (data & 0x0f) + 0x00);
        data >>= 4;
    }
}
