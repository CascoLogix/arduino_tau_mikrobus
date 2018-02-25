/**
 *  Menus.h
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

#ifndef __MENUS_H__
#define __MENUS_H__


#include <stdint.h>


#define MENU_MAX_MENU_ITEM_TITLE_LEN    64
#define MENU_MAX_MENU_TITLE_LEN         64


/**************************************************************************************
 * Holds the structure of the contents within an individual menu
 *************************************************************************************/
typedef struct menu_t menu_t;


/**************************************************************************************
 * Holds the overall structure of an individual menu
 *************************************************************************************/
typedef struct menuContents_t menuContents_t;


/**************************************************************************************
 * Holds the structure of the contents within an individual menu
 *************************************************************************************/
struct menuContents_t {
  uint32_t menuSel;                   // Menu selection item (1 thru 'n')
  char strMenuItem[MENU_MAX_MENU_ITEM_TITLE_LEN];               // Menu item descriptive title (text string)
  menu_t* pSubMenu;                   // Pointer to sub menu (null for no sub menu)
  void (*pFunction)(void);            // Function pointer to menu command (null for 'no command')
};


/**************************************************************************************
 * Holds the overall structure of an individual menu
 *************************************************************************************/
struct menu_t {
  char menuTitle[MENU_MAX_MENU_TITLE_LEN];                 // Menu descriptive title (text string)
  menuContents_t* pContents;          // Menu contents structure
};


void displayConfigurationMenu (void);


#endif  // __MENUS_H__

