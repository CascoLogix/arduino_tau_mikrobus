/**
 *  TerminalInterface.h
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

#ifndef __TERMINAL_INTERFACE_H__
#define __TERMINAL_INTERFACE_H__


#include <stdint.h>
#include "ApplicationFlashFunctions.h"


void configureSettings (void);
char terminalInterface (void);
void displayConfigurationMenu (void);
char getMenuSelection (uint8_t* errStatus);
void promptWiFiSSID (char WiFiSSIDname[]);
void getTextFromSerial(char strBuf[], uint8_t numBytes);
void getTextFromSerial(char strBuf[], uint8_t numBytes, char echoChar);
void promptWiFiPassword (char WiFiPassword[]);
void promptEncryptionKey (char encryptionKey[]);
bool validateWiFiSSIDname (char WiFiSSIDname[]);
void saveWiFiSSID (char * WiFiSSIDname);
void printConfigParams (flashConfig_t * data);
void printConfigParamSSIDname (flashConfig_t * data);
void printConfigParamWiFiPwd (flashConfig_t * data);
void printConfigParamGateID (flashConfig_t * data);
void printConfigParamNetID (flashConfig_t * data);
void printConfigParamNodeID (flashConfig_t * data);
void printConfigParamEncryptionKey (flashConfig_t * data);
void printConfigParamTXPer (flashConfig_t * data);
void printConfigParamRXInt (flashConfig_t * data);
void printConfigParamRXWait (flashConfig_t * data);
uint16_t promptGatewayID (void);
uint16_t promptNetworkID (void);
uint16_t promptNodeID (void);
uint32_t promptTXPer (void);
uint32_t promptRXInt (void);
uint32_t promptRXWait (void);


#endif  // __TERMINAL_INTERFACE_H__

