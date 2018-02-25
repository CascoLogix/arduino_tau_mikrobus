/**
 *  ApplicationFlashFunctions.h
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

#ifndef __APPLICATION_FLASH_FUNCTIONS_H__
#define __APPLICATION_FLASH_FUNCTIONS_H__


#include <stdint.h>


#define TERM_CHAR     '\r'      // Carriage return character
//#define TERM_CHAR     '\n'      // Newline character

#define MENU_EXIT     0

#define SSID_MAX_LEN                32
#define WIFI_PWD_MAX_LEN            64
#define ENCRYPTION_KEY_MAX_LEN      32
#define FLASH_CONFIG_PARAMS_ADDR    0


typedef struct {
  char SSID_Name[SSID_MAX_LEN];           // 32
  char WiFi_Password[WIFI_PWD_MAX_LEN];   // 64
  uint16_t gatewayID;                     // 2
  uint16_t networkID;                     // 2
  uint16_t nodeID;                        // 2
  char encryptionKey[ENCRYPTION_KEY_MAX_LEN];    // 32
  uint32_t TXPer; // milliseconds         // 4
  uint32_t RXInt;  // Turn on receiver after every 'N' transmits   // 4
  uint32_t RXWait; // How long to leave the receiver on after every 'N' transmits   // 4
  char Reserved[110];                      // 110
} flashConfig_t;


extern flashConfig_t runtimeParams;


void loadConfigParams (flashConfig_t * data);
void eraseConfigParams (flashConfig_t * data);
void displayFlashInfo (void);
void saveConfigParams (flashConfig_t * data);
const char * id2chip(const unsigned char *id);
void loadConfigFromFlash (const flashConfig_t * const flashConfig, flashConfig_t * runtimeConfig);
void saveConfigToFlash (const flashConfig_t * const flashConfig, const flashConfig_t * runtimeConfig);
void saveWiFiSSID(char * WiFiSSIDname);


#endif  // __APPLICATION_FLASH_FUNCTIONS_H__

