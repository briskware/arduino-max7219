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

// For MAX7219 datasheet see: https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf

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
 * @param csb_pin CS pin
 * @param clk CLK pin
 * @param dat_pin DATA pin
 * @param intensity 0x00 - 0x0f (bits 3-7 are ignored)
 */
void MAX7219_init(uint8_t csb_pin, uint8_t clk, uint8_t dat_pin, uint8_t intensity = 0xFF);

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
