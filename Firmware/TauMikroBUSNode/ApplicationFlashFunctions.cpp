/**
 *  ApplicationFlashFunctions.cpp
 *
 *  Created: 2018 Feb 24
 *  Author: Clint Stevenson
 *
 *  Description:
 *
 *  Copyright (C) 2018  CascoLogix (http://cascologix.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ApplicationFlashFunctions.h"
#include <SerialFlash.h>


//char chipID[];
uint8_t buf[256];
uint32_t chipsize, blocksize;
flashConfig_t runtimeParams;

void loadConfigParams (flashConfig_t * data)
{
  SerialFlash.read(FLASH_CONFIG_PARAMS_ADDR, data, 256);
}

void eraseConfigParams (flashConfig_t * data)
{
  SerialFlash.eraseBlock(FLASH_CONFIG_PARAMS_ADDR);
  memset(data, 0, 256);
  while(!SerialFlash.ready());
  SerialUSB.println("Config parameters have been erased.");
}

void displayFlashInfo (void)
{
  Serial.println("Read Chip Identification:");
  SerialFlash.readID(buf);
  Serial.print("  JEDEC ID:     ");
  Serial.print(buf[0], HEX);
  Serial.print(" ");
  Serial.print(buf[1], HEX);
  Serial.print(" ");
  Serial.println(buf[2], HEX);
  Serial.print("  Part Nummber: ");
  Serial.println(id2chip(buf));
  Serial.print("  Memory Size:  ");
  chipsize = SerialFlash.capacity(buf);
  Serial.print(chipsize);
  Serial.println(" bytes");
  //if (chipsize == 0) return false;
  Serial.print("  Block Size:   ");
  blocksize = SerialFlash.blockSize();
  Serial.print(blocksize);
  Serial.println(" bytes");
  Serial.println();
  SerialUSB.print("Flash chip serial number: ");
  SerialFlash.readSerialNumber(buf);
  for(int i = 0; i < 8; i++)
  {
    Serial.print(buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}


void saveConfigParams (flashConfig_t * data)
{
  SerialFlash.eraseBlock(FLASH_CONFIG_PARAMS_ADDR);
  while(!SerialFlash.ready());
  saveConfigToFlash(FLASH_CONFIG_PARAMS_ADDR, data);
  while(!SerialFlash.ready());
}

void loadConfigFromFlash (const flashConfig_t * const flashConfig, flashConfig_t * runtimeConfig)
{
  SerialFlash.read((uint32_t)flashConfig, (void*)runtimeConfig, sizeof(flashConfig_t));
}

void saveConfigToFlash(const flashConfig_t * const flashConfig, const flashConfig_t * runtimeConfig)
{
  SerialFlash.write((uint32_t)flashConfig, (const void*)runtimeConfig, sizeof(flashConfig_t));
}

const char * id2chip(const unsigned char *id)
{
  if (id[0] == 0xEF) {
    // Winbond
    if (id[1] == 0x40) {
      if (id[2] == 0x14) return "W25Q80BV";
      if (id[2] == 0x15) return "W25Q16DV";
      if (id[2] == 0x17) return "W25Q64FV";
      if (id[2] == 0x18) return "W25Q128FV";
      if (id[2] == 0x19) return "W25Q256FV";
    }
  }
  if (id[0] == 0x01) {
    // Spansion
    if (id[1] == 0x02) {
      if (id[2] == 0x16) return "S25FL064A";
      if (id[2] == 0x19) return "S25FL256S";
      if (id[2] == 0x20) return "S25FL512S";
    }
    if (id[1] == 0x20) {
      if (id[2] == 0x18) return "S25FL127S";
    }
    if (id[1] == 0x40) {
      if (id[2] == 0x15) return "S25FL116K";
    }
  }
  if (id[0] == 0xC2) {
    // Macronix
    if (id[1] == 0x20) {
      if (id[2] == 0x18) return "MX25L12805D";
    }
  }
  if (id[0] == 0x20) {
    // Micron
    if (id[1] == 0xBA) {
      if (id[2] == 0x20) return "N25Q512A";
      if (id[2] == 0x21) return "N25Q00AA";
    }
    if (id[1] == 0xBB) {
      if (id[2] == 0x22) return "MT25QL02GC";
    }
  }
  if (id[0] == 0xBF) {
    // SST
    if (id[1] == 0x25) {
      if (id[2] == 0x02) return "SST25WF010";
      if (id[2] == 0x03) return "SST25WF020";
      if (id[2] == 0x04) return "SST25WF040";
      if (id[2] == 0x41) return "SST25VF016B";
      if (id[2] == 0x4A) return "SST25VF032";
    }
    if (id[1] == 0x25) {
      if (id[2] == 0x01) return "SST26VF016";
      if (id[2] == 0x02) return "SST26VF032";
      if (id[2] == 0x43) return "SST26VF064";
    }
  }
  return "(unknown chip)";
}

