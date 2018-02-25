/**
 *  Menu.cpp
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
#include "Menus.h"


const menuContents_t mainMenuContents[] = {
  {1,   "Set WiFi SSID name",                   0/*(menu_t*)submenu_ptr1*/,     0},
  {2,   "Set WiFi password",                    0/*(menu_t*)submenu_ptr2*/,     0},
  {3,   "Set gateway ID",                       0/*(menu_t*)submenu_ptr3*/,     0},
  {4,   "Set network ID3",                      0/*(menu_t*)submenu_ptr3*/,     0},
  {5,   "Set node ID",                          0/*(menu_t*)submenu_ptr3*/,     0},
  {6,   "Set encryption key",                   0/*(menu_t*)submenu_ptr3*/,     0},
  {7,   "Set transmit period",                  0/*(menu_t*)submenu_ptr3*/,     0},
  {8,   "Set receive interval",                 0/*(menu_t*)submenu_ptr3*/,     0},
  {9,   "Set receiver on time (mS)",            0/*(menu_t*)submenu_ptr3*/,     0},
  {10,  "List configuration parameters",        0/*(menu_t*)submenu_ptr3*/,     0},
  {11,  "Save configuration parameters",        0/*(menu_t*)submenu_ptr3*/,     0},
  {12,  "Erase all configuration parameters",   0/*(menu_t*)submenu_ptr3*/,     0},
  {0,   0,                                      0,                              0}
};


const menu_t mainMenu = {
  "Configuration menu", 
  (menuContents_t*)&mainMenuContents
};


void displayConfigurationMenu (void)
{
  SerialUSB.println("Configuration menu");
  SerialUSB.println("    1) Set WiFi SSID name");
  SerialUSB.println("    2) Set WiFi password");
  SerialUSB.println("    3) Set gateway ID");
  SerialUSB.println("    4) Set network ID");
  SerialUSB.println("    5) Set node ID");
  SerialUSB.println("    6) Set encryption key");
  SerialUSB.println("    7) Set transmit period");
  SerialUSB.println("    8) Set receive interval");
  SerialUSB.println("    9) Set receiver on time (mS)");
  SerialUSB.println("    L) List configuration parameters");
  SerialUSB.println("    S) Save configuration parameters");
  SerialUSB.println("    E) Erase all configuration parameters");
  SerialUSB.println();
  SerialUSB.println("Enter the menu item below and press 'Enter'");
}



