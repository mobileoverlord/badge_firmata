/*
 * TickerOLED.cpp
 * SSD130x OLED Driver Library
 *
 * Copyright (c) 2011 seeed technology inc.
 * Author        :   Visweswara R
 * Create Time   :   Dec 2011
 * Change Log    :
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Wire.h"
#include "TickerOLED.h"

#include <avr/pgmspace.h>

#define MAX_DATA_CHUNK 31  // The Arduino has a 32 byte limit and the OLED wants a "Data" indicator byte

#if 0
// 8x8 Font ASCII 32 - 127 Implemented
// Users can modify this to support more characters(glyphs)
// BasicFont is placed in code memory.

// This font be freely used without any restriction(It is placed in public domain)
const unsigned char BasicFont[][8] PROGMEM=
{
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x5F,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x07,0x00,0x07,0x00,0x00,0x00},
  {0x00,0x14,0x7F,0x14,0x7F,0x14,0x00,0x00},
  {0x00,0x24,0x2A,0x7F,0x2A,0x12,0x00,0x00},
  {0x00,0x23,0x13,0x08,0x64,0x62,0x00,0x00},
  {0x00,0x36,0x49,0x55,0x22,0x50,0x00,0x00},
  {0x00,0x00,0x05,0x03,0x00,0x00,0x00,0x00},
  {0x00,0x1C,0x22,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x41,0x22,0x1C,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x2A,0x1C,0x2A,0x08,0x00,0x00},
  {0x00,0x08,0x08,0x3E,0x08,0x08,0x00,0x00},
  {0x00,0xA0,0x60,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
  {0x00,0x60,0x60,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x10,0x08,0x04,0x02,0x00,0x00},
  {0x00,0x3E,0x51,0x49,0x45,0x3E,0x00,0x00},
  {0x00,0x00,0x42,0x7F,0x40,0x00,0x00,0x00},
  {0x00,0x62,0x51,0x49,0x49,0x46,0x00,0x00},
  {0x00,0x22,0x41,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x18,0x14,0x12,0x7F,0x10,0x00,0x00},
  {0x00,0x27,0x45,0x45,0x45,0x39,0x00,0x00},
  {0x00,0x3C,0x4A,0x49,0x49,0x30,0x00,0x00},
  {0x00,0x01,0x71,0x09,0x05,0x03,0x00,0x00},
  {0x00,0x36,0x49,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x06,0x49,0x49,0x29,0x1E,0x00,0x00},
  {0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00},
  {0x00,0x00,0xAC,0x6C,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x14,0x22,0x41,0x00,0x00,0x00},
  {0x00,0x14,0x14,0x14,0x14,0x14,0x00,0x00},
  {0x00,0x41,0x22,0x14,0x08,0x00,0x00,0x00},
  {0x00,0x02,0x01,0x51,0x09,0x06,0x00,0x00},
  {0x00,0x32,0x49,0x79,0x41,0x3E,0x00,0x00},
  {0x00,0x7E,0x09,0x09,0x09,0x7E,0x00,0x00},
  {0x00,0x7F,0x49,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x41,0x22,0x00,0x00},
  {0x00,0x7F,0x41,0x41,0x22,0x1C,0x00,0x00},
  {0x00,0x7F,0x49,0x49,0x49,0x41,0x00,0x00},
  {0x00,0x7F,0x09,0x09,0x09,0x01,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x51,0x72,0x00,0x00},
  {0x00,0x7F,0x08,0x08,0x08,0x7F,0x00,0x00},
  {0x00,0x41,0x7F,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x40,0x41,0x3F,0x01,0x00,0x00},
  {0x00,0x7F,0x08,0x14,0x22,0x41,0x00,0x00},
  {0x00,0x7F,0x40,0x40,0x40,0x40,0x00,0x00},
  {0x00,0x7F,0x02,0x0C,0x02,0x7F,0x00,0x00},
  {0x00,0x7F,0x04,0x08,0x10,0x7F,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x41,0x3E,0x00,0x00},
  {0x00,0x7F,0x09,0x09,0x09,0x06,0x00,0x00},
  {0x00,0x3E,0x41,0x51,0x21,0x5E,0x00,0x00},
  {0x00,0x7F,0x09,0x19,0x29,0x46,0x00,0x00},
  {0x00,0x26,0x49,0x49,0x49,0x32,0x00,0x00},
  {0x00,0x01,0x01,0x7F,0x01,0x01,0x00,0x00},
  {0x00,0x3F,0x40,0x40,0x40,0x3F,0x00,0x00},
  {0x00,0x1F,0x20,0x40,0x20,0x1F,0x00,0x00},
  {0x00,0x3F,0x40,0x38,0x40,0x3F,0x00,0x00},
  {0x00,0x63,0x14,0x08,0x14,0x63,0x00,0x00},
  {0x00,0x03,0x04,0x78,0x04,0x03,0x00,0x00},
  {0x00,0x61,0x51,0x49,0x45,0x43,0x00,0x00},
  {0x00,0x7F,0x41,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x02,0x04,0x08,0x10,0x20,0x00,0x00},
  {0x00,0x41,0x41,0x7F,0x00,0x00,0x00,0x00},
  {0x00,0x04,0x02,0x01,0x02,0x04,0x00,0x00},
  {0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00},
  {0x00,0x01,0x02,0x04,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x54,0x54,0x54,0x78,0x00,0x00},
  {0x00,0x7F,0x48,0x44,0x44,0x38,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x28,0x00,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x48,0x7F,0x00,0x00},
  {0x00,0x38,0x54,0x54,0x54,0x18,0x00,0x00},
  {0x00,0x08,0x7E,0x09,0x02,0x00,0x00,0x00},
  {0x00,0x18,0xA4,0xA4,0xA4,0x7C,0x00,0x00},
  {0x00,0x7F,0x08,0x04,0x04,0x78,0x00,0x00},
  {0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x80,0x84,0x7D,0x00,0x00,0x00,0x00},
  {0x00,0x7F,0x10,0x28,0x44,0x00,0x00,0x00},
  {0x00,0x41,0x7F,0x40,0x00,0x00,0x00,0x00},
  {0x00,0x7C,0x04,0x18,0x04,0x78,0x00,0x00},
  {0x00,0x7C,0x08,0x04,0x7C,0x00,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x38,0x00,0x00,0x00},
  {0x00,0xFC,0x24,0x24,0x18,0x00,0x00,0x00},
  {0x00,0x18,0x24,0x24,0xFC,0x00,0x00,0x00},
  {0x00,0x00,0x7C,0x08,0x04,0x00,0x00,0x00},
  {0x00,0x48,0x54,0x54,0x24,0x00,0x00,0x00},
  {0x00,0x04,0x7F,0x44,0x00,0x00,0x00,0x00},
  {0x00,0x3C,0x40,0x40,0x7C,0x00,0x00,0x00},
  {0x00,0x1C,0x20,0x40,0x20,0x1C,0x00,0x00},
  {0x00,0x3C,0x40,0x30,0x40,0x3C,0x00,0x00},
  {0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00},
  {0x00,0x1C,0xA0,0xA0,0x7C,0x00,0x00,0x00},
  {0x00,0x44,0x64,0x54,0x4C,0x44,0x00,0x00},
  {0x00,0x08,0x36,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x41,0x36,0x08,0x00,0x00,0x00,0x00},
  {0x00,0x02,0x01,0x01,0x02,0x01,0x00,0x00},
  {0x00,0x02,0x05,0x05,0x02,0x00,0x00,0x00} 
};
#endif
// Nx24 Font ASCII 32 - 126 Implemented

const uint8_t TickerFont_widths[] PROGMEM = {
    5, 4, 7, 14, 12, 14, 11, 7, 6, 6, 9, 8, 5, 7, 4, 9, 10, 8, 10, 10, 10, 10, 10, 10, 10, 10, 5, 5, 6, 9, 6, 9, 16, 12, 11, 10, 12, 11, 10, 12, 13, 9, 11, 10, 9, 15, 14, 14, 9, 15, 11, 12, 12, 13, 11, 18, 12, 11, 12, 6, 9, 6, 10, 11, 9, 9, 10, 9, 10, 9, 9, 9, 10, 5, 7, 9, 5, 13, 9, 9, 9, 9, 8, 8, 8, 9, 8, 12, 10, 9, 9, 6, 7, 6, 10};
const uint16_t TickerFont_offsets[] PROGMEM = {
    0, 15, 27, 48, 90, 126, 168, 201, 222, 240, 258, 285, 309, 324, 345, 357, 384, 414, 438, 468, 498, 528, 558, 588, 618, 648, 678, 693, 708, 726, 753, 771, 798, 846, 882, 915, 945, 981, 1014, 1044, 1080, 1119, 1146, 1179, 1209, 1236, 1281, 1323, 1365, 1392, 1437, 1470, 1506, 1542, 1581, 1614, 1668, 1704, 1737, 1773, 1791, 1818, 1836, 1866, 1899, 1926, 1953, 1983, 2010, 2040, 2067, 2094, 2121, 2151, 2166, 2187, 2214, 2229, 2268, 2295, 2322, 2349, 2376, 2400, 2424, 2448, 2475, 2499, 2535, 2565, 2592, 2619, 2637, 2658, 2676};
const uint8_t TickerFont_data[] PROGMEM = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    224, 224, 0, 0, 255, 255, 0, 0, 13, 13, 0, 0, 192, 192, 0, 0,
    192, 192, 0, 7, 7, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 192, 192, 0, 0, 0, 192, 192, 0, 0, 192, 198,
    198, 230, 254, 223, 199, 198, 230, 254, 223, 199, 198, 6, 0, 0, 6, 7,
    1, 0, 0, 6, 7, 1, 0, 0, 0, 0, 0, 0, 128, 128, 192, 240,
    240, 192, 192, 128, 0, 0, 0, 15, 15, 25, 24, 255, 255, 24, 56, 240,
    224, 0, 0, 3, 7, 6, 6, 63, 63, 6, 7, 3, 1, 0, 128, 192,
    192, 192, 192, 128, 0, 128, 224, 64, 0, 0, 0, 0, 3, 7, 6, 6,
    199, 243, 126, 143, 195, 192, 192, 192, 128, 0, 0, 0, 0, 6, 7, 1,
    0, 3, 7, 6, 6, 7, 3, 0, 0, 0, 0, 128, 192, 192, 192, 128,
    0, 0, 0, 192, 240, 48, 31, 15, 60, 119, 195, 192, 248, 0, 1, 3,
    7, 6, 6, 6, 6, 3, 3, 3, 4, 0, 0, 192, 192, 0, 0, 0,
    0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 192, 224, 96, 0, 248, 255, 3, 0, 0, 0, 3, 31, 60, 112, 96,
    96, 224, 192, 0, 0, 0, 0, 0, 3, 255, 248, 0, 96, 112, 60, 31,
    3, 0, 0, 0, 0, 192, 0, 0, 0, 0, 0, 1, 27, 15, 7, 15,
    15, 19, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 48, 48, 48, 254, 254, 48, 48, 48, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    48, 60, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 96, 96, 96, 96,
    96, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 128, 192, 0, 0, 0,
    0, 192, 240, 62, 15, 1, 0, 0, 12, 15, 3, 0, 0, 0, 0, 0,
    0, 0, 128, 192, 192, 192, 192, 128, 0, 0, 254, 255, 3, 0, 0, 0,
    129, 255, 254, 0, 0, 3, 7, 6, 6, 6, 7, 3, 0, 0, 0, 0,
    128, 192, 192, 0, 0, 0, 0, 3, 1, 255, 255, 0, 0, 0, 0, 6,
    6, 7, 7, 6, 6, 0, 0, 0, 128, 192, 192, 192, 192, 128, 128, 0,
    0, 131, 195, 97, 32, 48, 25, 15, 7, 0, 0, 7, 7, 6, 6, 6,
    6, 6, 6, 0, 0, 128, 128, 192, 192, 192, 192, 128, 0, 0, 0, 129,
    1, 24, 24, 24, 60, 239, 231, 0, 0, 1, 3, 6, 6, 6, 7, 3,
    1, 0, 0, 0, 0, 0, 0, 128, 192, 192, 0, 0, 96, 112, 120, 108,
    99, 97, 255, 255, 96, 96, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0,
    0, 192, 192, 192, 192, 192, 192, 192, 192, 0, 0, 31, 31, 6, 6, 6,
    14, 252, 248, 0, 0, 3, 7, 6, 6, 6, 7, 3, 0, 0, 0, 0,
    0, 0, 128, 192, 192, 0, 0, 0, 0, 248, 254, 31, 27, 25, 24, 240,
    224, 0, 0, 1, 3, 7, 6, 6, 7, 3, 1, 0, 192, 192, 192, 192,
    192, 192, 192, 192, 192, 192, 0, 0, 0, 192, 240, 60, 14, 3, 1, 0,
    0, 0, 6, 7, 0, 0, 0, 0, 0, 0, 0, 128, 192, 192, 192, 192,
    192, 128, 128, 0, 239, 255, 61, 24, 24, 24, 60, 255, 231, 0, 1, 3,
    7, 6, 6, 6, 7, 3, 1, 0, 0, 128, 192, 192, 192, 192, 192, 128,
    0, 0, 31, 63, 113, 96, 96, 96, 241, 255, 63, 0, 0, 12, 12, 4,
    6, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 0,
    0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 0, 0,
    12, 14, 2, 0, 0, 0, 0, 0, 0, 0, 32, 112, 216, 156, 12, 0,
    0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    140, 140, 140, 140, 140, 140, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 12, 140, 152, 240, 96, 0, 3, 1,
    1, 0, 0, 0, 0, 0, 128, 128, 128, 0, 0, 0, 0, 3, 3, 129,
    193, 65, 99, 63, 28, 0, 0, 12, 13, 0, 0, 0, 0, 0, 0, 0,
    0, 128, 192, 224, 96, 96, 96, 96, 224, 192, 192, 128, 0, 0, 0, 124,
    255, 131, 56, 124, 102, 102, 58, 120, 96, 96, 97, 63, 30, 0, 0, 0,
    1, 3, 6, 14, 12, 12, 12, 12, 6, 6, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 128, 192, 0, 0, 0, 0, 0, 192, 224, 120, 126,
    103, 49, 255, 254, 128, 0, 0, 6, 7, 1, 0, 0, 0, 0, 0, 7,
    7, 0, 0, 192, 192, 192, 192, 192, 128, 128, 0, 0, 0, 0, 255, 255,
    48, 48, 48, 57, 127, 239, 192, 0, 0, 7, 7, 6, 6, 6, 7, 3,
    3, 1, 0, 0, 0, 0, 128, 192, 192, 192, 192, 192, 0, 240, 252, 15,
    3, 1, 0, 0, 129, 129, 0, 1, 3, 7, 6, 6, 6, 3, 3, 1,
    0, 0, 192, 192, 128, 128, 128, 0, 0, 0, 0, 0, 0, 0, 255, 255,
    0, 1, 1, 3, 3, 142, 252, 248, 0, 0, 3, 7, 6, 6, 6, 6,
    7, 3, 1, 0, 0, 0, 192, 192, 192, 192, 192, 192, 192, 192, 192, 0,
    0, 255, 255, 24, 24, 24, 24, 24, 24, 0, 0, 0, 3, 7, 6, 6,
    6, 6, 6, 6, 0, 0, 0, 192, 192, 192, 192, 192, 192, 192, 192, 128,
    0, 255, 255, 24, 24, 24, 24, 24, 24, 0, 0, 7, 7, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 128, 192, 192, 192, 192, 128, 128, 0, 0,
    240, 252, 15, 51, 49, 48, 48, 48, 177, 241, 112, 0, 1, 3, 7, 6,
    6, 6, 6, 3, 3, 1, 0, 0, 0, 192, 192, 0, 0, 0, 0, 0,
    0, 0, 192, 192, 0, 0, 255, 255, 96, 96, 96, 48, 48, 48, 48, 255,
    255, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 192,
    192, 192, 192, 192, 192, 192, 192, 0, 0, 0, 0, 255, 255, 0, 0, 0,
    0, 6, 6, 6, 7, 7, 6, 6, 6, 0, 0, 0, 0, 192, 192, 192,
    192, 192, 192, 192, 192, 192, 192, 0, 0, 0, 0, 255, 255, 0, 0, 0,
    1, 3, 3, 6, 6, 7, 7, 3, 0, 0, 0, 0, 192, 192, 0, 0,
    0, 0, 0, 128, 192, 0, 255, 255, 56, 252, 204, 134, 3, 1, 0, 0,
    7, 7, 0, 0, 1, 3, 3, 6, 6, 0, 192, 192, 0, 0, 0, 0,
    0, 0, 0, 255, 255, 0, 0, 0, 0, 0, 0, 0, 7, 7, 6, 6,
    6, 6, 6, 0, 0, 0, 0, 128, 192, 192, 0, 0, 0, 128, 192, 128,
    0, 0, 0, 0, 128, 248, 127, 7, 255, 248, 0, 240, 255, 3, 255, 248,
    0, 0, 0, 7, 7, 0, 0, 0, 7, 7, 7, 0, 0, 1, 7, 6,
    0, 0, 192, 192, 128, 0, 0, 0, 0, 0, 0, 0, 192, 128, 0, 0,
    255, 255, 3, 6, 12, 56, 112, 192, 128, 0, 255, 255, 0, 0, 7, 7,
    0, 0, 0, 0, 0, 0, 1, 3, 7, 7, 0, 0, 0, 0, 0, 128,
    192, 192, 192, 192, 192, 128, 128, 0, 0, 0, 248, 254, 135, 3, 1, 0,
    0, 0, 0, 193, 255, 63, 0, 0, 0, 1, 3, 7, 6, 6, 6, 6,
    3, 1, 0, 0, 0, 0, 192, 192, 192, 192, 192, 128, 0, 0, 0, 255,
    255, 48, 48, 57, 31, 15, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 128, 192, 192, 192, 192, 192, 192, 128, 0, 0, 0, 0,
    124, 254, 135, 1, 1, 0, 192, 128, 0, 1, 7, 255, 252, 0, 0, 0,
    1, 3, 3, 6, 6, 6, 7, 3, 6, 31, 59, 112, 0, 0, 192, 192,
    192, 192, 128, 128, 0, 0, 0, 0, 0, 255, 255, 96, 224, 225, 179, 63,
    30, 0, 0, 0, 7, 7, 0, 0, 0, 1, 3, 7, 6, 0, 0, 0,
    0, 128, 128, 192, 192, 192, 192, 128, 0, 0, 0, 128, 142, 31, 25, 24,
    24, 24, 56, 240, 224, 0, 0, 1, 3, 7, 6, 6, 6, 6, 3, 3,
    1, 0, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 0, 0, 0,
    0, 0, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7,
    0, 0, 0, 0, 0, 0, 0, 192, 192, 0, 0, 0, 0, 0, 0, 0,
    192, 192, 0, 0, 255, 255, 128, 0, 0, 0, 0, 0, 192, 255, 63, 0,
    0, 0, 3, 3, 7, 6, 6, 6, 7, 3, 1, 0, 0, 0, 192, 192,
    0, 0, 0, 0, 0, 0, 192, 192, 0, 1, 31, 254, 224, 0, 192, 248,
    63, 7, 0, 0, 0, 0, 1, 7, 6, 7, 1, 0, 0, 0, 0, 192,
    192, 0, 0, 0, 0, 0, 128, 192, 192, 0, 0, 0, 0, 128, 192, 0,
    0, 1, 15, 127, 240, 0, 224, 252, 31, 3, 255, 252, 0, 192, 248, 63,
    7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 3, 7, 6, 7,
    1, 0, 0, 0, 0, 192, 192, 128, 0, 0, 0, 0, 0, 0, 192, 192,
    0, 0, 1, 131, 199, 110, 56, 124, 238, 135, 1, 0, 0, 6, 7, 3,
    0, 0, 0, 0, 0, 3, 7, 6, 192, 192, 0, 0, 0, 0, 0, 0,
    192, 192, 0, 0, 1, 7, 30, 184, 224, 124, 31, 3, 0, 0, 0, 0,
    0, 6, 7, 1, 0, 0, 0, 0, 0, 0, 192, 192, 192, 192, 192, 192,
    192, 192, 192, 192, 192, 0, 0, 128, 192, 96, 56, 28, 14, 3, 1, 1,
    0, 0, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 224,
    224, 96, 96, 0, 0, 255, 255, 0, 0, 0, 0, 127, 127, 96, 96, 0,
    0, 192, 192, 0, 0, 0, 0, 0, 0, 0, 0, 3, 15, 124, 224, 128,
    0, 0, 0, 0, 0, 0, 0, 3, 15, 12, 96, 96, 96, 224, 224, 0,
    0, 0, 0, 255, 255, 0, 96, 96, 96, 127, 127, 0, 0, 0, 0, 128,
    192, 96, 64, 128, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 0, 32, 96, 192, 128,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    224, 248, 24, 12, 12, 252, 252, 0, 0, 1, 3, 6, 6, 6, 3, 3,
    6, 0, 224, 224, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 24, 12,
    12, 28, 248, 240, 0, 0, 7, 7, 6, 6, 6, 7, 3, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 224, 248, 28, 12, 12, 28, 24,
    0, 0, 1, 3, 7, 6, 6, 7, 3, 0, 0, 0, 0, 0, 0, 0,
    0, 224, 224, 0, 0, 240, 248, 28, 12, 12, 12, 255, 255, 0, 0, 1,
    3, 7, 6, 6, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 224, 248, 220, 204, 108, 108, 60, 56, 0, 1, 3, 7, 6, 6,
    6, 2, 3, 0, 0, 0, 128, 192, 224, 96, 96, 0, 0, 12, 12, 255,
    255, 12, 12, 12, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 224, 248, 28, 12, 12, 252, 248, 0,
    0, 193, 199, 198, 198, 227, 127, 31, 0, 0, 224, 224, 0, 0, 0, 0,
    0, 0, 0, 0, 255, 255, 24, 12, 12, 252, 248, 0, 0, 0, 7, 7,
    0, 0, 0, 7, 7, 0, 0, 0, 192, 192, 0, 0, 0, 252, 252, 0,
    0, 0, 7, 7, 0, 0, 0, 0, 0, 192, 192, 0, 0, 0, 0, 0,
    252, 252, 0, 0, 112, 192, 192, 255, 127, 0, 0, 0, 224, 224, 0, 0,
    0, 0, 0, 0, 0, 255, 255, 192, 96, 240, 152, 12, 4, 0, 7, 7,
    0, 0, 0, 1, 7, 6, 0, 0, 224, 224, 0, 0, 0, 255, 255, 0,
    0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 252, 252, 48, 24, 12, 252, 248, 28, 12, 252, 248, 0, 0,
    7, 7, 0, 0, 0, 7, 7, 0, 0, 7, 7, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 252, 252, 24, 12, 12, 252, 248, 0, 0, 7,
    7, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 224, 248, 28, 12, 12, 28, 248, 240, 0, 1, 3, 7, 6, 6, 7,
    3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 254, 254, 24, 12,
    12, 252, 240, 0, 0, 255, 255, 6, 6, 7, 3, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 224, 248, 24, 12, 12, 252, 252, 0, 0,
    1, 3, 6, 6, 6, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 252, 252, 24, 12, 60, 60, 0, 0, 7, 7, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 112, 120, 216, 204, 220, 156, 0,
    0, 6, 6, 6, 6, 7, 3, 0, 0, 0, 0, 128, 128, 0, 0, 0,
    0, 12, 12, 255, 255, 12, 12, 0, 0, 0, 0, 7, 7, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 252, 252, 0, 0, 0, 252,
    252, 0, 0, 3, 7, 6, 6, 6, 7, 7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 12, 124, 240, 128, 128, 224, 124, 12, 0, 0, 3, 7, 7,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    60, 252, 192, 0, 240, 124, 252, 0, 224, 252, 28, 0, 0, 3, 7, 7,
    1, 0, 7, 6, 7, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 12, 28, 184, 240, 224, 240, 184, 28, 12, 0, 6, 7, 3, 1, 0,
    1, 3, 7, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 60,
    240, 192, 0, 192, 240, 60, 12, 0, 0, 224, 127, 15, 3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 140, 204, 252, 124,
    28, 12, 0, 6, 7, 7, 7, 6, 6, 6, 6, 0, 128, 192, 96, 96,
    96, 224, 255, 191, 0, 0, 0, 0, 31, 63, 112, 96, 96, 0, 0, 0,
    224, 224, 0, 0, 0, 0, 0, 255, 255, 0, 0, 0, 0, 0, 63, 63,
    0, 0, 96, 96, 224, 192, 0, 0, 0, 0, 191, 255, 224, 64, 96, 96,
    127, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 96,
    56, 12, 12, 56, 112, 96, 112, 56, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0};


void TickerOLED::init()
{
    sendCommand(TickerOLED_Display_Off_Cmd);     //display off
    delay(5); 
    sendCommand(TickerOLED_Display_On_Cmd);  //display on
    delay(5); 
    sendCommand(TickerOLED_Normal_Display_Cmd);  //Set Normal Display (default)

    // Set Horizontal addressing
    sendCommand(0x20);          //set addressing mode
    sendCommand(0x00);          //set horizontal addressing mode
}

void TickerOLED::sendCommand(unsigned char command)
{
  Wire.beginTransmission(TickerOLED_Address); // begin I2C communication
#if defined(ARDUINO) && ARDUINO >= 100
  Wire.write(TickerOLED_Command_Mode);        // Set OLED Command mode
  Wire.write(command);
#else
  Wire.send(TickerOLED_Command_Mode);         // Set OLED Command mode
  Wire.send(command);
#endif
  Wire.endTransmission();                // End I2C communication
}

void TickerOLED::setBrightness(unsigned char Brightness)
{
   sendCommand(TickerOLED_Set_Brightness_Cmd);
   sendCommand(Brightness);
}

void TickerOLED::setTextXY(unsigned char Row, unsigned char Column)
{
    sendCommand(0xB0 + Row);            //set page address
    sendCommand(0x00 + (8*Column & 0x0F));  //set column lower address
    sendCommand(0x10 + ((8*Column>>4)&0x0F));   //set column higher address
}

void TickerOLED::clearDisplay()
{
  setTextXY(0, 0);

  uint16_t bytes = 128 * 64 / 8;
  do
  {
      Wire.beginTransmission(TickerOLED_Address); // begin I2C transmission
      Wire.write(TickerOLED_Data_Mode);            // data mode
      int chunkSize = min(bytes, MAX_DATA_CHUNK);
      for (int j = 0; j < chunkSize; j++)
          Wire.write(0);
      Wire.endTransmission();                    // stop I2C transmission
      bytes -= chunkSize;
  } while (bytes > 0);
  setTextXY(0,0); // Should be unnecessary
}

void TickerOLED::sendData(unsigned char Data)
{
     Wire.beginTransmission(TickerOLED_Address); // begin I2C transmission
#if defined(ARDUINO) && ARDUINO >= 100
     Wire.write(TickerOLED_Data_Mode);            // data mode
     Wire.write(Data);
#else
     Wire.send(TickerOLED_Data_Mode);            // data mode
     Wire.send(Data);
#endif
     Wire.endTransmission();                    // stop I2C transmission
}

void TickerOLED::drawBitmap(const unsigned char *bitmaparray, int bytes)
{
  do
  {
      Wire.beginTransmission(TickerOLED_Address); // begin I2C transmission
      Wire.write(TickerOLED_Data_Mode);            // data mode
      int chunkSize = min(bytes, MAX_DATA_CHUNK);
      for (int j = 0; j < chunkSize; j++)
      {
          Wire.write(pgm_read_byte(bitmaparray));
          bitmaparray++;
      }
      Wire.endTransmission();                    // stop I2C transmission
      bytes -= chunkSize;
  } while (bytes > 0);
}

#define TICKER_MAX_LENGTH  20 // Tweet length
#define TICKER_CHAR_WIDTH  16 // Max
#define TICKER_CHAR_HEIGHT 24
#define TICKER_PAGES     (TICKER_CHAR_HEIGHT / 8)
#define TICKER_COLUMNS   (TICKER_CHAR_WIDTH * TICKER_MAX_LENGTH)
#define TICKER_RESTART_GAP 16  // columns

static uint8_t tickerBuffer[TICKER_PAGES][TICKER_COLUMNS];
static uint16_t tickerColumn = 0;
static uint16_t tickerMaxColumn = 0;

void TickerOLED::setTicker(const char *message)
{
    memset(tickerBuffer, 0, sizeof(tickerBuffer));
    uint16_t column = 0;
    for (;;) {
        char c = pgm_read_byte(message);
        if (c == 0)
            break;
        if (c < 32 || c >= 127)
            c = 32;
        c -= 32;

        uint8_t width = pgm_read_byte(&TickerFont_widths[c]);
        if (column + width > TICKER_COLUMNS)
            break;

        uint16_t offset = pgm_read_word(&TickerFont_offsets[c]);

        for (uint8_t page = 0; page < TICKER_PAGES; page++) {
            for (uint8_t x = 0; x < width; x++) {
                tickerBuffer[page][column + x] = pgm_read_byte(&TickerFont_data[offset + x]);
            }
            offset += width;
        }
        column += width;
        message++;
    }

    tickerColumn = 0;
    tickerMaxColumn = column + TICKER_RESTART_GAP;
    if (tickerMaxColumn < 128)
        tickerMaxColumn = 128;
    else if (tickerMaxColumn > TICKER_COLUMNS)
        tickerMaxColumn = TICKER_COLUMNS;
}

void TickerOLED::updateTicker()
{
    // Go back to the origin
    sendCommand(0xB0);
    sendCommand(0x00);
    sendCommand(0x10);

    uint16_t bytes = 128 * TICKER_PAGES;
    uint16_t offset = tickerColumn;
    uint16_t offsetStart = offset;
    uint8_t page = 0;
    uint8_t widthWritten = 0;
    do
    {
        Wire.beginTransmission(TickerOLED_Address); // begin I2C transmission
        Wire.write(TickerOLED_Data_Mode);            // data mode
        int chunkSize = min(bytes, MAX_DATA_CHUNK);
        for (uint16_t j = 0; j < chunkSize; j++)
        {
            Wire.write(tickerBuffer[page][offset]);
            offset++;
            if (offset >= tickerMaxColumn)
                offset = 0;

            widthWritten++;
            if (widthWritten == 128) {
                widthWritten = 0;
                page++;
                offset = offsetStart;
            }
        }
        Wire.endTransmission();                    // stop I2C transmission
        bytes -= chunkSize;
    } while (bytes > 0);
    tickerColumn++;
    if (tickerColumn >= tickerMaxColumn)
        tickerColumn = 0;
}

void TickerOLED::setNormalDisplay()
{
    sendCommand(TickerOLED_Normal_Display_Cmd);
}

void TickerOLED::setInverseDisplay()
{
    sendCommand(TickerOLED_Inverse_Display_Cmd);
}
