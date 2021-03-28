//
// Created by Stefan Szaniszlo on 28/03/2021.
//
// For datasheet see: https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf

#ifndef MAX7219_H
#define MAX7219_H

void MAX7219_send_cmd(uint16_t cmd);
void MAX7219_init(uint8_t cs, uint8_t clk, uint8_t data, uint8_t intensity = 0xFF);
void MAX7219_test();
void MAX7219_clear();
void MAX7219_write(uint8_t position, uint8_t value);

#endif MAX7219_H
