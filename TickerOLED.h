/*
 * TickerOLED.h
 * SSD130x OLED Driver Library
 *
 * Based off the Seeed OLED driver, but now very similar any more.
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

#ifndef TickerOLED_data_H
#define TickerOLED_data_H

#include "Arduino.h"

#define TickerOLED_Max_X                 127 //128 Pixels
#define TickerOLED_Max_Y                 63  //64  Pixels

#define TickerOLED_Address               0x3c
#define TickerOLED_Command_Mode          0x80
#define TickerOLED_Data_Mode             0x40
#define TickerOLED_Display_Off_Cmd       0xAE
#define TickerOLED_Display_On_Cmd        0xAF
#define TickerOLED_Normal_Display_Cmd    0xA6
#define TickerOLED_Inverse_Display_Cmd   0xA7
#define TickerOLED_Activate_Scroll_Cmd   0x2F
#define TickerOLED_Dectivate_Scroll_Cmd  0x2E
#define TickerOLED_Set_Brightness_Cmd    0x81

class TickerOLED {

public:

static byte init();
static byte sendCommand(unsigned char command);

static byte setTextXY(unsigned char Row, unsigned char Column);
static byte clearDisplay();
static byte setBrightness(unsigned char Brightness);
static void drawBitmap(const unsigned char *bitmaparray,int bytes);

static void setTicker(const char *message, uint16_t len);
static void updateTicker();

};

#endif


