//
// Created by Stefan Szaniszlo on 28/03/2021.
//
// For datasheet see: https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf

#ifndef MAX7219_H
#define MAX7219_H

/**
 * Send a 16 bit command to the MAX7219
 * @param cmd
 */
void MAX7219_send_cmd(uint16_t cmd);

/**
 * Initialize the MAX7219 library.
 * * initialize control pins
 * * disable chip and send shutdown command
 * * clear all values (makes all positions show empty)
 * * set decode mode (see directive MAX7219_DIRECT_DRIVE)
 * * set intensity
 * * set scan limit to all 8 positions
 * * switch on normal operation
 *
 * @param cs CS pin
 * @param clk CLK pin
 * @param data DATA pin
 * @param intensity 0x00 - 0x0f (bits 3-7 are ignored)
 */
void MAX7219_init(uint8_t cs, uint8_t clk, uint8_t data, uint8_t intensity = 0xFF);

/**
 * Sends the shutdown command to the MAX7219 controller (switches on power safe more and disables display).
 * Call MAX7219_init() or MAX7219_test() to resume operation
 */
void MAX7219_shutdown();

/**
 * Set test mode for 500 millis, then turn the display off for 500 mills, total delay ~1 second.
 */
void MAX7219_test();

/**
 * switch off all segments in all positions
 */
void MAX7219_clear();

/**
 * switch off all segments in at the given position
 * @param position 0 - 7, wraps around by ignoring bits 3-7, position 0 is the right-most (least-significant)
 */
void MAX7219_clear(uint8_t position);

/**
 * Send a single character to the display
 * @param position 0 - 7, wraps around by ignoring bits 3-7, position 0 is the right-most (least-significant)
 * @param value the value to show, if bit 7 is set the decimal point is illuminated
 */
void MAX7219_write(uint8_t position, uint8_t value);

#endif MAX7219_H
