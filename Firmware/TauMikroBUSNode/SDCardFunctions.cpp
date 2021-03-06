/**
 *  SDCardFunctions.cpp
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


#include "SDCardFunctions.h"
#include "main.h"


Sd2Card card;
SdVolume volume;
SdFile root;


/***********************************************************************************/
/* 
/***********************************************************************************/
bool SDCardDetected (void)
{
  return card.init(SPI_HALF_SPEED, SD_CS_PIN);
}

/***********************************************************************************/
/* 
/***********************************************************************************/
bool SDFileExists (char filename[])
{
  return SD.exists(filename);
}

/***********************************************************************************/
/* 
/***********************************************************************************/
bool SDFileIsValid (char filename[])
{
  
}

/***********************************************************************************/
/* 
/***********************************************************************************/
void SDReadFile (char filename[], flashConfig_t * data)
{
  
}

/***********************************************************************************/
/* 
/***********************************************************************************/
void SDCreateLogFile (char filename[])
{
  
}

/***********************************************************************************/
/* 
/***********************************************************************************/
void SDAppendLogFile (char filename[])
{
  
}

