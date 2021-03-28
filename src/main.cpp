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

    pinMode(CS, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(DATA, OUTPUT);

    MAX7219_init(CS, CLK, DATA, 0x0a);
    MAX7219_test();
    MAX7219_clear();
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delay_ms);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delay_ms);
//    for(uint8_t i = 0; i < 8; ++i) {
//        MAX7219_write(i, (count + i) & 0x0f);
//    }
    write_address(count);
    write_data( (uint8_t) (count / 97));
    ++count;
}

void write_address(uint16_t address) {
    for ( uint8_t i = 4; i < 8; ++i) {
        MAX7219_write(i, address & 0x0f);
        address >>= 4;
    }
}

void write_data(uint8_t data) {
    for ( uint8_t i = 0; i < 2; ++i) {
        MAX7219_write(i, data & 0x0f);
        data >>= 4;
    }
}
