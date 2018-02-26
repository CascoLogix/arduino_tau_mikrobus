/**
 *  main.h
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

#ifndef __MAIN_H__
#define __MAIN_H__


#define TERM_CHAR                   '\r'      // Carriage return character
//#define TERM_CHAR                 '\n'      // Newline character

#define FLASH_CS_PIN                4
#define SD_CS_PIN                   10
#define MIKROBUS_PWM_PIN            8
#define MIKROBUS_INT_PIN            2
#define MIKROBUS_AN_PIN             A0
#define MIKROBUS_SERIAL             Serial
#define MIKROBUS_SPI_CS_PIN         5

#define CONFIG_FILENAME             "config.bin"


#endif  // __MAIN_H__
