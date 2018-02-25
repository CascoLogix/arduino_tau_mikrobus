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
#include "TerminalInterface.h"
#include "Menus.h"
#include "ApplicationFlashFunctions.h"


#define FLASH_CS_PIN                4


uint8_t i = 0;

void setup() 
{
  SerialFlash.begin(FLASH_CS_PIN);
  // put your setup code here, to run once:
  SerialUSB.begin(115200);
  while(!Serial);
  SerialUSB.println("Press 'Enter' to continue");
  SerialUSB.println();

  SerialUSB.println("Onboard flash chip info:");
  displayFlashInfo();

  loadConfigFromFlash(FLASH_CONFIG_PARAMS_ADDR, &runtimeParams);
  configureSettings();
}

void loop() 
{
  SerialUSB.println("Application main loop started");
  while(1);
}

void configureSettings (void)
{
  uint8_t menuSelection = 1;            // Initialize to something other than zero to enter the loop below
  uint8_t errStatus = 0;
  
  while(MENU_EXIT != menuSelection)
  {
    displayConfigurationMenu();
    
    menuSelection = getMenuSelection(&errStatus);

    if(errStatus)
    {
      // Display error status
    }

    else
    {
      switch(menuSelection)
      {
        case '1':
          promptWiFiSSID(runtimeParams.SSID_Name);
          break;

        case '2':
          //SerialUSB.println("Case 2");
          promptWiFiPassword(runtimeParams.WiFi_Password);
          break;

        case '3':
          //SerialUSB.println("Case 3");
          runtimeParams.gatewayID = promptGatewayID();
          break;

        case '4':
          //SerialUSB.println("Case 4");
          runtimeParams.networkID = promptNetworkID();
          break;

        case '5':
          //SerialUSB.println("Case 5");
          runtimeParams.nodeID = promptNodeID();
          break;

        case '6':
          //SerialUSB.println("Case 6");
          promptEncryptionKey(runtimeParams.encryptionKey);
          break;

        case '7':
          //SerialUSB.println("Case 7");
          runtimeParams.TXPer = promptTXPer();
          break;

        case '8':
          //SerialUSB.println("Case 8");
          runtimeParams.RXInt = promptRXInt();
          break;

        case '9':
          //SerialUSB.println("Case 9");
          runtimeParams.RXWait = promptRXWait();
          break;

        case 'L': case 'l':
          //SerialUSB.println("Case L");
          printConfigParams(&runtimeParams);
          break;

        case 'S': case 's':
          //SerialUSB.println("Case S");
          saveConfigParams(&runtimeParams);
          break;

        case 'E': case 'e':
          //SerialUSB.println("Case E");
          eraseConfigParams(&runtimeParams);
          break;

        case MENU_EXIT:
          SerialUSB.println("Exiting configuration menu");
          break;

        default:
          SerialUSB.println("Default case");
          break;
      }
    }
  }
}

