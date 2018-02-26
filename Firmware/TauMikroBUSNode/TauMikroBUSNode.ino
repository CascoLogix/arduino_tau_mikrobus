/**
 *  TauMikroBUSNode.ino
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

#include "main.h"
#include <SerialFlash.h>
#include "SDCardFunctions.h"
#include "TerminalInterface.h"
#include "Menus.h"
#include "ApplicationFlashFunctions.h"
#include <RTCZero.h>


uint16_t i = 0;

void setup() 
{
  uint8_t timeout;
  
  SerialUSB.begin(115200);
  SerialFlash.begin(FLASH_CS_PIN);
  loadConfigFromFlash(FLASH_CONFIG_PARAMS_ADDR, &runtimeParams);

  // Look for SD card first to avoid waiting for serial terminal delay
  if(SDCardDetected())
  {
    if(SDFileExists(CONFIG_FILENAME))
    {
      if(/*file_is_valid*/1)
      {
        // Load configuration from SD card into flash
        SDReadFile(CONFIG_FILENAME, &runtimeParams);
        
        // Then save config to flash chip
        saveConfigToFlash(FLASH_CONFIG_PARAMS_ADDR, &runtimeParams);
      }
    }
  }
  
  // If serial connection, then launch terminal interface
  else if(Serial)
  {
    timeout = 6000;           // Allows 1-minute for serial connection before continuing on
    while(!Serial && timeout)
    {
      delay(100);
      timeout--;
    }

    terminalInterface();
    Serial.end();     // Disconnect SerialUSB
  }
}

void loop() 
{
  SerialUSB.println("Application main loop started");
  while(1);
}


