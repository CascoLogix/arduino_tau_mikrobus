/**
 *  TerminalInterface.cpp
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

#include <Arduino.h>
#include "TerminalInterface.h"


#define TERM_CHAR                   '\r'      // Carriage return character
//#define TERM_CHAR                 '\n'      // Newline character
#define MENU_EXIT                   '0'


struct {
  char leading[128];    uint8_t numLeading;
  char global[128];     uint8_t numGlobal;
  char trailing[128];   uint8_t numTrailing;
} disallowedSSIDchars = {
  {'!', '#', ';'}, 3,
  {'+', '[', ']', '/', '\\', '"', '\t'}, 7,
  {' '}, 1
};


uint32_t asciiToInt (char strNum[], uint8_t numChars);


/***********************************************************************************/
/* Terminal Interface
/***********************************************************************************/
char terminalInterface (void)
{
  SerialUSB.println("Press 'Enter' to continue");
  SerialUSB.println();

  SerialUSB.println("Onboard flash chip info:");
  displayFlashInfo();
  configureSettings();
}

/***********************************************************************************/
/* Configure Settings
/***********************************************************************************/
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

/***********************************************************************************/
/* Get Menu Selection
/***********************************************************************************/
char getMenuSelection (uint8_t* errStatus)
{
  uint8_t serialRxBytes = 0;
  uint8_t retVal = 0;
  char rxBuffer[16];

  memset(rxBuffer, 0, sizeof(rxBuffer));    // Initialize buffer to 0's

  while((serialRxBytes < 2) && (rxBuffer[1] != '\r') && (rxBuffer[1] != 0x03))
  {
    while(!SerialUSB.available());      // Wait for char
    rxBuffer[serialRxBytes++] = SerialUSB.read();
  }

  while(SerialUSB.available())        // Empty rx buffer
  {
    SerialUSB.read();                 // Throw away received char
  }

  if((rxBuffer[0] >= '0') || (rxBuffer[0] <= '9'))
  {
    retVal = rxBuffer[0];
    *errStatus = 0;
  }
  else if((rxBuffer[0] == 'L') || (rxBuffer[0] == 'E') || (rxBuffer[0] == 'S') || (rxBuffer[0] == 'l') || (rxBuffer[0] == 'e') || (rxBuffer[0] == 's'))
  {
    SerialUSB.println("Letter");
    retVal = rxBuffer[0];
    *errStatus = 0;
  }
  else
  {
    retVal = rxBuffer[0];
    *errStatus = 1;
  }

  return retVal;
}

/***********************************************************************************/
/* Prompt for WiFi SSID Name
/***********************************************************************************/
void promptWiFiSSID (char WiFiSSIDname[])
{
  SerialUSB.print("Enter WiFi SSID Name and press 'Enter'");
  //getWiFiSSIDfromSerial(WiFiSSIDname);
  getTextFromSerial(WiFiSSIDname, SSID_MAX_LEN);
  
  if(!validateWiFiSSIDname(WiFiSSIDname))
  {
    SerialUSB.println();
    SerialUSB.println("Invalid WiFi SSID name");

    SerialUSB.print("Cannot start with ");
    for(int idx = 0; idx < disallowedSSIDchars.numLeading; idx++)
    {
      SerialUSB.print("'");
      SerialUSB.print(disallowedSSIDchars.leading[idx]);
      SerialUSB.print("' ");
    }
    SerialUSB.println();

    SerialUSB.print("Cannot contain ");
    for(int idx = 0; idx < disallowedSSIDchars.numGlobal; idx++)
    {
      SerialUSB.print("'");
      SerialUSB.print(disallowedSSIDchars.global[idx]);
      SerialUSB.print("' ");
    }
    SerialUSB.println();

    SerialUSB.print("Cannot contain trailing ");
    for(int idx = 0; idx < disallowedSSIDchars.numTrailing; idx++)
    {
      SerialUSB.print("'");
      SerialUSB.print(disallowedSSIDchars.trailing[idx]);
      SerialUSB.print("' ");
    }
    SerialUSB.println();

    memset(WiFiSSIDname, 0, SSID_MAX_LEN);
  }
  
  SerialUSB.println();
  SerialUSB.println();
}

/***********************************************************************************/
/* Prompt for WiFi Password
/***********************************************************************************/
void promptWiFiPassword (char WiFiPassword[])
{
  SerialUSB.print("Enter WiFi password and press 'Enter'");
  //getWiFiPasswordfromSerial(WiFiPassword);
  getTextFromSerial(WiFiPassword, WIFI_PWD_MAX_LEN, '*');
  SerialUSB.println();
  SerialUSB.println();
}

/***********************************************************************************/
/* Validate WiFi SSID Name
/***********************************************************************************/
bool validateWiFiSSIDname (char WiFiSSIDname[])
{
  uint8_t charIndex;
  uint8_t charIndex2;

  // Check for leading !, #, or ; characters (not allowed)
  for(charIndex = 0; charIndex < disallowedSSIDchars.numLeading; charIndex++)
  {
    if(WiFiSSIDname[0] == disallowedSSIDchars.leading[charIndex])
    {
      SerialUSB.print("Leading '");
      SerialUSB.print(WiFiSSIDname[0]);
      SerialUSB.println(" detected");
      return false;
    }
  }

  // Check for any characters not allowed at all
  for(charIndex = 0; charIndex < strlen(WiFiSSIDname); charIndex++)
  {
    for(charIndex2 = 0; charIndex2 < disallowedSSIDchars.numGlobal; charIndex2++)
    {
      if(WiFiSSIDname[charIndex] == disallowedSSIDchars.global[charIndex2])
      {
        SerialUSB.print("Disallowed character detected: '");
        SerialUSB.print(WiFiSSIDname[charIndex]);
        SerialUSB.println("'");
        return false;
      }
    }
  }

  // Check for trailing spaces
  for(charIndex2 = 0; charIndex2 < disallowedSSIDchars.numTrailing; charIndex2++)
  {
    if(WiFiSSIDname[strlen(WiFiSSIDname) - 1] == disallowedSSIDchars.trailing[charIndex2])
    {
      SerialUSB.print("Trailing '");
      SerialUSB.print(WiFiSSIDname[strlen(WiFiSSIDname) - 1]);
      SerialUSB.println(" detected");
      return false;
    }
  }

  // If everything passed, then return true
  return true;
}

/***********************************************************************************/
/* Print All Config Parameters
/***********************************************************************************/
void printConfigParams (flashConfig_t * data)
{
  printConfigParamSSIDname(data);
  printConfigParamWiFiPwd(data);
  printConfigParamGateID(data);
  printConfigParamNetID(data);
  printConfigParamNodeID(data);
  printConfigParamEncryptionKey(data);
  printConfigParamTXPer(data);
  printConfigParamRXInt(data);
  printConfigParamRXWait(data);
  SerialUSB.println();
}

/***********************************************************************************/
/* Print SSID Name From Config Parameters
/***********************************************************************************/
void printConfigParamSSIDname (flashConfig_t * data)
{
  SerialUSB.print("WiFi SSID: ");
  
  if(data->SSID_Name[0] == 0xFF)
  {
    SerialUSB.println("not set");
  }
  else
  {
    SerialUSB.println(data->SSID_Name);
  }
}

/***********************************************************************************/
/* Print WiFi Password From Config Parameters
/***********************************************************************************/
void printConfigParamWiFiPwd (flashConfig_t * data)
{
  SerialUSB.print("WiFi password: ");
  if(data->WiFi_Password[0] == 0xFF)
  {
    SerialUSB.println("not set");
  }
  else
  {
    SerialUSB.println("********");
  }
}

/***********************************************************************************/
/* Print Gateway ID From Config Parameters
/***********************************************************************************/
void printConfigParamGateID (flashConfig_t * data)
{
  SerialUSB.print("Gateway ID: ");
  SerialUSB.println(data->gatewayID);
}

/***********************************************************************************/
/* Print Network ID From Config Parameters
/***********************************************************************************/
void printConfigParamNetID (flashConfig_t * data)
{
  SerialUSB.print("Network ID: ");
  SerialUSB.println(data->networkID);
}

/***********************************************************************************/
/* Print Node ID From Config Parameters
/***********************************************************************************/
void printConfigParamNodeID (flashConfig_t * data)
{
  SerialUSB.print("Node ID: ");
  SerialUSB.println(data->nodeID);
}

/***********************************************************************************/
/* Print Encryption From Config Parameters
/***********************************************************************************/
void printConfigParamEncryptionKey (flashConfig_t * data)
{
  SerialUSB.print("Encryption key: ");
  if(data->encryptionKey[0] == 0xFF)
  {
    SerialUSB.println("not set");
  }
  else
  {
    SerialUSB.println("********");
  }
}

/***********************************************************************************/
/* Print TX Period From Config Parameters
/***********************************************************************************/
void printConfigParamTXPer (flashConfig_t * data)
{
  SerialUSB.print("TX period (mS): ");
  SerialUSB.println(data->TXPer);
}

/***********************************************************************************/
/* Print RX Interval From Config Parameters
/***********************************************************************************/
void printConfigParamRXInt (flashConfig_t * data)
{
  SerialUSB.print("RX interval: ");
  SerialUSB.println(data->RXInt);
}

/***********************************************************************************/
/* Print RX Wait Time From Config Parameters
/***********************************************************************************/
void printConfigParamRXWait (flashConfig_t * data)
{
  SerialUSB.print("RX wait: ");
  SerialUSB.println(data->RXWait);
}

/***********************************************************************************/
/* Prompt Encryption Key
/***********************************************************************************/
void promptEncryptionKey (char encryptionKey[])
{
  SerialUSB.print("Enter network encryption key and press 'Enter'");
  getTextFromSerial(encryptionKey, ENCRYPTION_KEY_MAX_LEN, '*');
  SerialUSB.println();
  SerialUSB.println();
}

/***********************************************************************************/
/* Get Text from Serial
/***********************************************************************************/
void getTextFromSerial(char strBuf[], uint8_t numBytes)
{
  uint8_t charIndex = 0;

  SerialUSB.println();
  
  do {
    while(!SerialUSB.available());
    strBuf[charIndex] = SerialUSB.read();
    if(strBuf[charIndex] == '\r' || strBuf[charIndex] == 0x03)
      break;
    else
      SerialUSB.print(strBuf[charIndex]);
    charIndex++;
  } while(charIndex < SSID_MAX_LEN);

  strBuf[charIndex] = 0;

  while(SerialUSB.available());
  {
    SerialUSB.read();
  }
}

/***********************************************************************************/
/* Get Text from Serial (with masked echo chars)
/***********************************************************************************/
void getTextFromSerial(char strBuf[], uint8_t numBytes, char echoChar)
{
  uint8_t charIndex = 0;

  SerialUSB.println();
  
  do {
    while(!SerialUSB.available());
    strBuf[charIndex] = SerialUSB.read();
    if(strBuf[charIndex] == '\r' || strBuf[charIndex] == 0x03)
      break;
    else
      SerialUSB.print(echoChar);
    charIndex++;
  } while(charIndex < SSID_MAX_LEN);

  strBuf[charIndex] = 0;

  while(SerialUSB.available());
  {
    SerialUSB.read();
  }
}

/***********************************************************************************/
/* Convert ASCII Input Into Integer
/***********************************************************************************/
uint32_t asciiToInt (char strNum[], uint8_t numChars)
{
  uint32_t retVal = 0;

  // Check for max digits entered
  if(strlen(strNum) > numChars)
  {
    SerialUSB.println("Number contains too many digits");
  }
  
  // Convert ascii digits to 16-bit value
  else
  {
    retVal = atoi(strNum);
  }
}

/***********************************************************************************/
/* Prompt Gateway ID
/***********************************************************************************/
uint16_t promptGatewayID (void)
{
  uint16_t retVal = 0;
  const uint8_t NUM_DIGITS = 5; // 5 digits for 16-bit decimal-fomatted number
  char strBuf[NUM_DIGITS + 1];
  
  SerialUSB.print("Enter gateway ID (max 65535) and press 'Enter'");
  getTextFromSerial(strBuf, NUM_DIGITS + 1);

  retVal = asciiToInt(strBuf, NUM_DIGITS + 1);

  SerialUSB.println();
  SerialUSB.println();
  
  return retVal;
}

/***********************************************************************************/
/* Prompt Network ID
/***********************************************************************************/
uint16_t promptNetworkID (void)
{
  uint16_t retVal = 0;
  const uint8_t NUM_DIGITS = 5; // 5 digits for 16-bit decimal-fomatted number
  char strBuf[NUM_DIGITS + 1];
  
  SerialUSB.print("Enter network ID (max 65535) and press 'Enter'");
  getTextFromSerial(strBuf, NUM_DIGITS + 1);

  retVal = asciiToInt(strBuf, NUM_DIGITS + 1);

  SerialUSB.println();
  SerialUSB.println();
  
  return retVal;
}

/***********************************************************************************/
/* Prompt Node ID
/***********************************************************************************/
uint16_t promptNodeID (void)
{
  uint16_t retVal = 0;
  const uint8_t NUM_DIGITS = 5; // 5 digits for 16-bit decimal-fomatted number
  char strBuf[NUM_DIGITS + 1];
  
  SerialUSB.print("Enter node ID (max 65535) and press 'Enter'");
  getTextFromSerial(strBuf, NUM_DIGITS + 1);

  retVal = asciiToInt(strBuf, NUM_DIGITS + 1);

  SerialUSB.println();
  SerialUSB.println();
  
  return retVal;
}

/***********************************************************************************/
/* Prompt Transmit Period
/***********************************************************************************/
uint32_t promptTXPer (void)
{
  uint32_t retVal = 0;
  const uint8_t NUM_DIGITS = 10; // 10 digits for 32-bit decimal-fomatted number
  char strBuf[NUM_DIGITS + 1];
  
  SerialUSB.print("Enter transmit period in milliseconds (32-bit max) and press 'Enter'");
  getTextFromSerial(strBuf, NUM_DIGITS + 1);

  retVal = asciiToInt(strBuf, NUM_DIGITS + 1);

  SerialUSB.println();
  SerialUSB.println();
  
  return retVal;
}

/***********************************************************************************/
/* Prompt RX Interval
/***********************************************************************************/
uint32_t promptRXInt (void)
{
  uint32_t retVal = 0;
  const uint8_t NUM_DIGITS = 10; // 10 digits for 8-bit decimal-fomatted number
  char strBuf[NUM_DIGITS + 1];
  
  SerialUSB.print("Enter receive interval (32-bit max) and press 'Enter'");
  getTextFromSerial(strBuf, NUM_DIGITS + 1);

  retVal = asciiToInt(strBuf, NUM_DIGITS + 1);

  SerialUSB.println();
  SerialUSB.println();
  
  return retVal;
}

/***********************************************************************************/
/* Prompt RX Wait Time
/***********************************************************************************/
uint32_t promptRXWait (void)
{
  uint32_t retVal = 0;
  const uint8_t NUM_DIGITS = 10; // 10 digits for 32-bit decimal-fomatted number
  char strBuf[NUM_DIGITS + 1];
  
  SerialUSB.print("Enter transmit period in microseconds (32-bit max) and press 'Enter'");
  getTextFromSerial(strBuf, NUM_DIGITS + 1);

  retVal = asciiToInt(strBuf, NUM_DIGITS + 1);

  SerialUSB.println();
  SerialUSB.println();
  
  return retVal;
}

