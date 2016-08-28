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
#include "arial.h"

#define MAX_DATA_CHUNK 31  // The Arduino has a 32 byte limit and the OLED wants a "Data" indicator byte

byte TickerOLED::init()
{
    byte rc;

    // If the first command succeeds, assume that the rest will...
    rc = sendCommand(TickerOLED_Display_Off_Cmd);     //display off
    if (rc != 0) return rc;

    delay(5);
    sendCommand(TickerOLED_Display_On_Cmd);  //display on
    delay(5);
    sendCommand(TickerOLED_Normal_Display_Cmd);  //Set Normal Display (default)

    // Set Horizontal addressing
    sendCommand(0x20);          //set addressing mode
    sendCommand(0x00);          //set horizontal addressing mode

    return rc;
}

byte TickerOLED::sendCommand(unsigned char command)
{
  Wire.beginTransmission(TickerOLED_Address); // begin I2C communication
  Wire.write(TickerOLED_Command_Mode);        // Set OLED Command mode
  Wire.write(command);
  return Wire.endTransmission();                // End I2C communication
}

byte TickerOLED::setBrightness(unsigned char Brightness)
{
    Wire.beginTransmission(TickerOLED_Address); // begin I2C communication
    Wire.write(TickerOLED_Command_Mode);        // Set OLED Command mode
   Wire.write(TickerOLED_Set_Brightness_Cmd);
   Wire.write(Brightness);
   return Wire.endTransmission();                // End I2C communication
}

byte TickerOLED::setTextXY(unsigned char Row, unsigned char Column)
{
    Wire.beginTransmission(TickerOLED_Address); // begin I2C communication
    Wire.write(TickerOLED_Command_Mode);        // Set OLED Command mode
    Wire.write(0xB0 + Row);            //set page address
    Wire.write(TickerOLED_Command_Mode);        // Set OLED Command mode
    Wire.write(0x00 + (8*Column & 0x0F));  //set column lower address
    Wire.write(TickerOLED_Command_Mode);        // Set OLED Command mode
    Wire.write(0x10 + ((8*Column>>4)&0x0F));   //set column higher address
    return Wire.endTransmission();                // End I2C communication
}

byte TickerOLED::clearDisplay()
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
      bytes -= chunkSize;
      Wire.endTransmission(bytes != 0); // stop I2C transmission if last one
  } while (bytes > 0);
  return setTextXY(0,0); // Should be unnecessary
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
      bytes -= chunkSize;
      Wire.endTransmission(bytes != 0); // stop I2C transmission if last one
  } while (bytes > 0);
}

#define TICKER_MAX_LENGTH  40 // Kind of the max length
#define TICKER_CHAR_WIDTH  8 // Rough average width
#define TICKER_CHAR_HEIGHT 24
#define TICKER_PAGES     (TICKER_CHAR_HEIGHT / 8)
#define TICKER_COLUMNS   (TICKER_CHAR_WIDTH * TICKER_MAX_LENGTH)
#define TICKER_RESTART_GAP 16  // columns

static uint8_t tickerBuffer[TICKER_PAGES][TICKER_COLUMNS];
static uint16_t tickerColumn = 0;
static uint16_t tickerMaxColumn = 0;

void TickerOLED::setTicker(const char *message, uint16_t len)
{
    memset(tickerBuffer, 0, sizeof(tickerBuffer));

    // Handle off case
    if (len == 0) {
        tickerColumn = 0;
        tickerMaxColumn = 0;
        return;
    }

    uint16_t column = 0;
    for (uint16_t i = 0; i < len; i++) {
        char c = message[i]; // pgm_read_byte(&message[i]);
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
    if (tickerMaxColumn == 0)
        return;

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
        bytes -= chunkSize;
        Wire.endTransmission(bytes != 0); // stop I2C transmission if last one
    } while (bytes > 0);
    tickerColumn++;
    if (tickerColumn >= tickerMaxColumn)
        tickerColumn = 0;
}
