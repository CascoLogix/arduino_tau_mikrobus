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


struct {
  char leading[128];    uint8_t numLeading;
  char global[128];     uint8_t numGlobal;
  char trailing[128];   uint8_t numTrailing;
} disallowedSSIDchars = {
  {'!', '#', ';'}, 3,
  {'+', '[', ']', '/', '\\', '"', '\t'}, 7,
  {' '}, 1
};


char getMenuSelection (uint8_t* errStatus)
{
  uint8_t serialRxBytes = 0;
  uint8_t retVal = 0;
  char rxBuffer[16];

  memset(rxBuffer, 0, sizeof(rxBuffer));    // Initialize buffer to 0's

  while((serialRxBytes < 2) && (rxBuffer[1] != '\r') && (rxBuffer[1] != 0x03))
  {
    rxBuffer[serialRxBytes++] = getCharFromSerial();
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

char getCharFromSerial (void)
{
  while(!SerialUSB.available());      // Wait for char
  return SerialUSB.read();            // Read and return received char
}

void promptWiFiSSID (char WiFiSSIDname[])
{
  SerialUSB.print("Enter WiFi SSID Name and press 'Enter'");
  getWiFiSSIDfromSerial(WiFiSSIDname);
  SerialUSB.println();
  SerialUSB.println();
}

void getWiFiSSIDfromSerial(char WiFiSSIDname[])
{
  uint8_t charIndex = 0;

  SerialUSB.println();
  
  do {
    while(!SerialUSB.available());
    WiFiSSIDname[charIndex] = SerialUSB.read();
    if(WiFiSSIDname[charIndex] == '\r' || WiFiSSIDname[charIndex] == 0x03)
      break;
    else
      SerialUSB.print(WiFiSSIDname[charIndex]);
    charIndex++;
  } while(charIndex < SSID_MAX_LEN);

  WiFiSSIDname[charIndex] = 0;

  while(SerialUSB.available());
  {
    SerialUSB.read();
  }

  if(!validateWiFiSSIDname(WiFiSSIDname))
  {
    SerialUSB.println();
    SerialUSB.println("Invalid WiFi SSID name");

    SerialUSB.print("Cannot start with ");
    for(int i = 0; i < disallowedSSIDchars.numLeading; i++)
    {
      SerialUSB.print("'");
      SerialUSB.print(disallowedSSIDchars.leading[i]);
      SerialUSB.print("' ");
    }
    SerialUSB.println();

    SerialUSB.print("Cannot contain ");
    for(int i = 0; i < disallowedSSIDchars.numGlobal; i++)
    {
      SerialUSB.print("'");
      SerialUSB.print(disallowedSSIDchars.global[i]);
      SerialUSB.print("' ");
    }
    SerialUSB.println();

    SerialUSB.print("Cannot contain trailing ");
    for(int i = 0; i < disallowedSSIDchars.numTrailing; i++)
    {
      SerialUSB.print("'");
      SerialUSB.print(disallowedSSIDchars.trailing[i]);
      SerialUSB.print("' ");
    }
    SerialUSB.println();

    memset(WiFiSSIDname, 0, SSID_MAX_LEN);
  }
}

void promptWiFiPassword (char WiFiPassword[])
{
  SerialUSB.print("Enter WiFi password and press 'Enter'");
  getWiFiPasswordfromSerial(WiFiPassword);
  SerialUSB.println();
  SerialUSB.println();
}

void getWiFiPasswordfromSerial(char WiFiPassword[])
{
  uint8_t charIndex = 0;

  SerialUSB.println();
  
  do {
    while(!SerialUSB.available());
    WiFiPassword[charIndex] = SerialUSB.read();
    if(WiFiPassword[charIndex] == '\r' || WiFiPassword[charIndex] == 0x03)
      break;
    else
      SerialUSB.print("*");
    charIndex++;
  } while(charIndex < SSID_MAX_LEN);

  WiFiPassword[charIndex] = 0;

  while(SerialUSB.available());
  {
    SerialUSB.read();
  }
}

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

void printConfigParams (flashConfig_t * data)
{
  printConfigParamSSIDname(data);
  printConfigParamWiFiPwd(data);
  printConfigParamNetAddr(data);
  printConfigParamNetKey(data);
  printConfigParamNodeID(data);
  printConfigParamTXPer(data);
  printConfigParamRXInt(data);
  printConfigParamRXWait(data);
  SerialUSB.println();
}

void printConfigParamSSIDname (flashConfig_t * data)
{
  SerialUSB.print("WiFi SSID: ");
  SerialUSB.println(data->SSID_Name);
}

void printConfigParamWiFiPwd (flashConfig_t * data)
{
  SerialUSB.print("WiFi password: ");
  if(data->WiFi_Password[0] != 0)
  {
    SerialUSB.println("********");
  }
  else
  {
    SerialUSB.println("not set");
  }
}

void printConfigParamNetAddr (flashConfig_t * data)
{
  SerialUSB.print("Network ID: ");
  SerialUSB.println(data->networkID);
}

void printConfigParamNetKey (flashConfig_t * data)
{
  SerialUSB.print("Network key: ");
  if(data->encryptionKey[0] != 0)
  {
    SerialUSB.println("********");
  }
  else
  {
    SerialUSB.println("not set");
  }
}

void printConfigParamNodeID (flashConfig_t * data)
{
  SerialUSB.print("Node ID: ");
  SerialUSB.println(data->nodeID);
}

void printConfigParamTXPer (flashConfig_t * data)
{
  SerialUSB.print("TX period (mS): ");
  SerialUSB.println(data->TXPer);
}

void printConfigParamRXInt (flashConfig_t * data)
{
  SerialUSB.print("RX interval: ");
  SerialUSB.println(data->RXInt);
}

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
  getEncryptionKeyfromSerial(encryptionKey);
  SerialUSB.println();
  SerialUSB.println();
}

/***********************************************************************************/
/* Get Encryption Key Chars from Serial
/***********************************************************************************/
void getEncryptionKeyfromSerial(char encryptionKey[])
{
  uint8_t charIndex = 0;

  SerialUSB.println();
  
  do {
    while(!SerialUSB.available());
    encryptionKey[charIndex] = SerialUSB.read();
    if(encryptionKey[charIndex] == '\r' || encryptionKey[charIndex] == 0x03)
      break;
    else
      SerialUSB.print("*");
    charIndex++;
  } while(charIndex < SSID_MAX_LEN);

  encryptionKey[charIndex] = 0;

  while(SerialUSB.available());
  {
    SerialUSB.read();
  }
}

/***********************************************************************************/
/* Prompt Gateway ID
/***********************************************************************************/
uint16_t promptGatewayID (void)
{
  /*
  SerialUSB.print("Enter network encryption key and press 'Enter'");
  getEncryptionKeyfromSerial(encryptionKey);
  SerialUSB.println();
  SerialUSB.println();
  */
  return 0;
}

/***********************************************************************************/
/* Get Gateway ID Chars from Serial
/***********************************************************************************/
uint16_t getGatewayIDfromSerial(void)
{
  /*
  uint8_t charIndex = 0;

  SerialUSB.println();
  
  do {
    while(!SerialUSB.available());
    encryptionKey[charIndex] = SerialUSB.read();
    if(encryptionKey[charIndex] == '\r' || encryptionKey[charIndex] == 0x03)
      break;
    else
      SerialUSB.print("*");
    charIndex++;
  } while(charIndex < SSID_MAX_LEN);

  encryptionKey[charIndex] = 0;

  while(SerialUSB.available());
  {
    SerialUSB.read();
  }
  */
  return 0;
}

/***********************************************************************************/
/* Prompt Network ID
/***********************************************************************************/
uint16_t promptNetworkID (void)
{
  /*
  SerialUSB.print("Enter network encryption key and press 'Enter'");
  getEncryptionKeyfromSerial(encryptionKey);
  SerialUSB.println();
  SerialUSB.println();
  */
  return 0;
}

/***********************************************************************************/
/* Get Network ID Chars from Serial
/***********************************************************************************/
uint16_t getNetworkIDfromSerial(void)
{
  /*
  uint8_t charIndex = 0;

  SerialUSB.println();
  
  do {
    while(!SerialUSB.available());
    encryptionKey[charIndex] = SerialUSB.read();
    if(encryptionKey[charIndex] == '\r' || encryptionKey[charIndex] == 0x03)
      break;
    else
      SerialUSB.print("*");
    charIndex++;
  } while(charIndex < SSID_MAX_LEN);

  encryptionKey[charIndex] = 0;

  while(SerialUSB.available());
  {
    SerialUSB.read();
  }
  */
  return 0;
}

/***********************************************************************************/
/* Prompt Node ID
/***********************************************************************************/
uint16_t promptNodeID (void)
{
  /*
  SerialUSB.print("Enter network encryption key and press 'Enter'");
  getEncryptionKeyfromSerial(encryptionKey);
  SerialUSB.println();
  SerialUSB.println();
  */
  return 0;
}

/***********************************************************************************/
/* Get Node ID Chars from Serial
/***********************************************************************************/
uint16_t getNodeIDfromSerial(void)
{
  /*
  uint8_t charIndex = 0;

  SerialUSB.println();
  
  do {
    while(!SerialUSB.available());
    encryptionKey[charIndex] = SerialUSB.read();
    if(encryptionKey[charIndex] == '\r' || encryptionKey[charIndex] == 0x03)
      break;
    else
      SerialUSB.print("*");
    charIndex++;
  } while(charIndex < SSID_MAX_LEN);

  encryptionKey[charIndex] = 0;

  while(SerialUSB.available());
  {
    SerialUSB.read();
  }
  */
  return 0;
}

/***********************************************************************************/
/* Prompt Transmit Period
/***********************************************************************************/
uint32_t promptTXPer (void)
{
  /*
  SerialUSB.print("Enter network encryption key and press 'Enter'");
  getEncryptionKeyfromSerial(encryptionKey);
  SerialUSB.println();
  SerialUSB.println();
  */
  return 0;
}

/***********************************************************************************/
/* Get TX Period from Serial
/***********************************************************************************/
uint32_t getTXPerfromSerial(void)
{
  /*
  uint8_t charIndex = 0;

  SerialUSB.println();
  
  do {
    while(!SerialUSB.available());
    encryptionKey[charIndex] = SerialUSB.read();
    if(encryptionKey[charIndex] == '\r' || encryptionKey[charIndex] == 0x03)
      break;
    else
      SerialUSB.print("*");
    charIndex++;
  } while(charIndex < SSID_MAX_LEN);

  encryptionKey[charIndex] = 0;

  while(SerialUSB.available());
  {
    SerialUSB.read();
  }
  */
  return 0;
}

/***********************************************************************************/
/* Prompt RX Interval
/***********************************************************************************/
uint32_t promptRXInt (void)
{
  /*
  SerialUSB.print("Enter network encryption key and press 'Enter'");
  getEncryptionKeyfromSerial(encryptionKey);
  SerialUSB.println();
  SerialUSB.println();
  */
  return 0;
}

/***********************************************************************************/
/* Get RX Interval from Serial
/***********************************************************************************/
uint32_t getRXIntfromSerial (void)
{
  /*
  uint8_t charIndex = 0;

  SerialUSB.println();
  
  do {
    while(!SerialUSB.available());
    encryptionKey[charIndex] = SerialUSB.read();
    if(encryptionKey[charIndex] == '\r' || encryptionKey[charIndex] == 0x03)
      break;
    else
      SerialUSB.print("*");
    charIndex++;
  } while(charIndex < SSID_MAX_LEN);

  encryptionKey[charIndex] = 0;

  while(SerialUSB.available());
  {
    SerialUSB.read();
  }
  */
  return 0;
}

/***********************************************************************************/
/* Prompt RX Wait Time
/***********************************************************************************/
uint32_t promptRXWait (void)
{
  /*
  SerialUSB.print("Enter network encryption key and press 'Enter'");
  getEncryptionKeyfromSerial(encryptionKey);
  SerialUSB.println();
  SerialUSB.println();
  */
  return 0;
}

/***********************************************************************************/
/* Get RX Wait from Serial
/***********************************************************************************/
uint32_t getRXWaitfromSerial(void)
{
  /*
  uint8_t charIndex = 0;

  SerialUSB.println();
  
  do {
    while(!SerialUSB.available());
    encryptionKey[charIndex] = SerialUSB.read();
    if(encryptionKey[charIndex] == '\r' || encryptionKey[charIndex] == 0x03)
      break;
    else
      SerialUSB.print("*");
    charIndex++;
  } while(charIndex < SSID_MAX_LEN);

  encryptionKey[charIndex] = 0;

  while(SerialUSB.available());
  {
    SerialUSB.read();
  }
  */
  return 0;
}


