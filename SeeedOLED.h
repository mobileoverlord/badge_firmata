/*
 * SeeedOLED.h
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

#ifndef SeeedOLED_data_H
#define SeeedOLED_data_H

// SeeedOLED Instruction set addresses

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif

#define SeeedOLED_Max_X                 127 //128 Pixels
#define SeeedOLED_Max_Y                 63  //64  Pixels

#define PAGE_MODE                       01
#define HORIZONTAL_MODE                 02


#define SeeedOLED_Address               0x3c
#define SeeedOLED_Command_Mode          0x80
#define SeeedOLED_Data_Mode             0x40
#define SeeedOLED_Display_Off_Cmd       0xAE
#define SeeedOLED_Display_On_Cmd        0xAF
#define SeeedOLED_Normal_Display_Cmd    0xA6
#define SeeedOLED_Inverse_Display_Cmd   0xA7
#define SeeedOLED_Activate_Scroll_Cmd   0x2F
#define SeeedOLED_Dectivate_Scroll_Cmd  0x2E
#define SeeedOLED_Set_Brightness_Cmd    0x81

class SeeedOLED {

public:

void init(void);

void setNormalDisplay();
void setInverseDisplay();

void sendCommand(unsigned char command);
void sendData(unsigned char Data);
void sendData(unsigned char *Data, int count);

void setTextXY(unsigned char Row, unsigned char Column);
void clearDisplay();
void setBrightness(unsigned char Brightness);
void drawBitmapFast(unsigned char *bitmaparray,int bytes);

void setTicker(const char *message);
void updateTicker();

};

extern SeeedOLED SeeedOled;  // SeeedOLED object 

#endif


