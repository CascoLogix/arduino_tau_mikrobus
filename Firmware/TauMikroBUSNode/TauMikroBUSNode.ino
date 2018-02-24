

#include <SerialFlash.h>


#define TERM_CHAR     '\r'      // Carriage return character
//#define TERM_CHAR     '\n'      // Newline character

#define MENU_EXIT     0

#define SSID_MAX_LEN                32
#define WIFI_PWD_MAX_LEN            64
#define NET_KEY_MAX_LEN             32
#define FLASH_CONFIG_PARAMS_ADDR    0
#define FLASH_CS_PIN                4

typedef struct {
  uint8_t first;
  uint8_t second;
  uint8_t third;
  uint8_t fourth;
} ipAddr_t;

typedef struct {
  char SSID_Name[SSID_MAX_LEN];
  char WiFi_Password[WIFI_PWD_MAX_LEN];
  ipAddr_t NetAddr;
  char NetKey[NET_KEY_MAX_LEN];
  uint8_t NodeID;
  uint32_t TXPer; // milliseconds
  uint8_t RXInt;  // Turn on receiver after every 'N' transmits
  uint8_t RXWait; // How long to leave the receiver on after every 'N' transmits
  char Reserved[40];
} flashConfig_t;

struct {
  char leading[128];
  uint8_t numLeading;
  char global[128];
  uint8_t numGlobal;
  char trailing[128];
  uint8_t numTrailing;
} disallowedSSIDchars = {
  {'!', '#', ';'}, 3,
  {'+', '[', ']', '/', '\\', '"', '\t'}, 7,
  {' '}, 1
};

flashConfig_t runtimeParams;
//char chipID[];
uint8_t buf[256];
uint32_t chipsize, blocksize;

uint8_t i = 0;


void setup() 
{
  SerialFlash.begin(FLASH_CS_PIN);
  // put your setup code here, to run once:
  SerialUSB.begin(115200);
  while(!Serial);
  SerialUSB.println("Press 'Enter' to continue");
  SerialUSB.println();

  //displayFlashInfo();

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
          SerialUSB.println("Case 3");
          break;

        case '4':
          //SerialUSB.println("Case 4");
          promptNetKey(runtimeParams.NetKey);
          break;

        case '5':
          SerialUSB.println("Case 5");
          break;

        case '6':
          SerialUSB.println("Case 6");
          break;

        case '7':
          SerialUSB.println("Case 7");
          break;

        case '8':
          SerialUSB.println("Case 8");
          break;

        case '9':
          //SerialUSB.println("Case 9");
          loadConfigFromFlash(FLASH_CONFIG_PARAMS_ADDR, &runtimeParams);
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

void displayConfigurationMenu (void)
{
  SerialUSB.println("Configuration menu");
  SerialUSB.println("    1) Set WiFi SSID name");
  SerialUSB.println("    2) Set WiFi password");
  SerialUSB.println("    3) Set network address");
  SerialUSB.println("    4) Set network key");
  SerialUSB.println("    5) Set node ID");
  SerialUSB.println("    6) Set transmit period");
  SerialUSB.println("    7) Set receive interval");
  SerialUSB.println("    8) Set receiver on time (mS)");
  SerialUSB.println("    9) Load configuration parameters from flash");
  SerialUSB.println("    L) List configuration parameters");
  SerialUSB.println("    S) Save configuration parameters");
  SerialUSB.println("    E) Erase all configuration parameters");
  SerialUSB.println();
  SerialUSB.println();
  SerialUSB.println("Enter the menu item below and press 'Enter'");
}

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

void loadConfigFromFlash (const flashConfig_t * const flashConfig, flashConfig_t * runtimeConfig)
{
  uint16_t index;
  uint8_t * ptrRuntimeConfig = (uint8_t*)runtimeConfig;
  
  SerialFlash.read((uint32_t)flashConfig, ptrRuntimeConfig, sizeof(flashConfig_t));
}

const char * id2chip(const unsigned char *id)
{
  if (id[0] == 0xEF) {
    // Winbond
    if (id[1] == 0x40) {
      if (id[2] == 0x14) return "W25Q80BV";
      if (id[2] == 0x15) return "W25Q16DV";
      if (id[2] == 0x17) return "W25Q64FV";
      if (id[2] == 0x18) return "W25Q128FV";
      if (id[2] == 0x19) return "W25Q256FV";
    }
  }
  if (id[0] == 0x01) {
    // Spansion
    if (id[1] == 0x02) {
      if (id[2] == 0x16) return "S25FL064A";
      if (id[2] == 0x19) return "S25FL256S";
      if (id[2] == 0x20) return "S25FL512S";
    }
    if (id[1] == 0x20) {
      if (id[2] == 0x18) return "S25FL127S";
    }
    if (id[1] == 0x40) {
      if (id[2] == 0x15) return "S25FL116K";
    }
  }
  if (id[0] == 0xC2) {
    // Macronix
    if (id[1] == 0x20) {
      if (id[2] == 0x18) return "MX25L12805D";
    }
  }
  if (id[0] == 0x20) {
    // Micron
    if (id[1] == 0xBA) {
      if (id[2] == 0x20) return "N25Q512A";
      if (id[2] == 0x21) return "N25Q00AA";
    }
    if (id[1] == 0xBB) {
      if (id[2] == 0x22) return "MT25QL02GC";
    }
  }
  if (id[0] == 0xBF) {
    // SST
    if (id[1] == 0x25) {
      if (id[2] == 0x02) return "SST25WF010";
      if (id[2] == 0x03) return "SST25WF020";
      if (id[2] == 0x04) return "SST25WF040";
      if (id[2] == 0x41) return "SST25VF016B";
      if (id[2] == 0x4A) return "SST25VF032";
    }
    if (id[1] == 0x25) {
      if (id[2] == 0x01) return "SST26VF016";
      if (id[2] == 0x02) return "SST26VF032";
      if (id[2] == 0x43) return "SST26VF064";
    }
  }
  return "(unknown chip)";
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

void promptNetKey (char NetKey[])
{
  SerialUSB.print("Enter network key and press 'Enter'");
  getNetKeyfromSerial(NetKey);
  SerialUSB.println();
  SerialUSB.println();
}

void getNetKeyfromSerial(char netKey[])
{
  uint8_t charIndex = 0;

  SerialUSB.println();
  
  do {
    while(!SerialUSB.available());
    netKey[charIndex] = SerialUSB.read();
    if(netKey[charIndex] == '\r' || netKey[charIndex] == 0x03)
      break;
    else
      SerialUSB.print("*");
    charIndex++;
  } while(charIndex < SSID_MAX_LEN);

  netKey[charIndex] = 0;

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

void saveWiFiSSID(char * WiFiSSIDname)
{
  char strReadBack[32];
  
  if(validateWiFiSSIDname(WiFiSSIDname))
  {
    SerialFlash.write(FLASH_CONFIG_PARAMS_ADDR, WiFiSSIDname, strlen(WiFiSSIDname) + 1);
    SerialUSB.println("WiFi SSID name saved successfully!");
    //SerialUSB.print("WiFi SSID name, ");
    //SerialFlash.read(FLASH_CONFIG_PARAMS_ADDR, strReadBack, strlen(WiFiSSIDname));
    //SerialUSB.print(strReadBack);
    //SerialUSB.println(" saved successfully!");
  }
  else
  {
    SerialUSB.println("Invalid WiFi SSID name. Could not save.");
  }
}

void saveConfigParams (flashConfig_t * data)
{
  SerialFlash.eraseBlock(FLASH_CONFIG_PARAMS_ADDR);
  while(!SerialFlash.ready());
  saveWiFiSSID(data->SSID_Name);
//  saveWiFiPwd(data->SSID_Name);
//  saveNetAddr(data->SSID_Name);
//  saveNetKey(data->SSID_Name);
//  saveNodeID(data->SSID_Name);
//  saveTXPer(data->SSID_Name);
//  saveRXInt(data->SSID_Name);
//  saveRXWait(data->SSID_Name);
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
  SerialUSB.print("Network address: ");
  SerialUSB.print(data->NetAddr.first);
  SerialUSB.print(".");
  SerialUSB.print(data->NetAddr.second);
  SerialUSB.print(".");
  SerialUSB.print(data->NetAddr.third);
  SerialUSB.print(".");
  SerialUSB.println(data->NetAddr.fourth);
}

void printConfigParamNetKey (flashConfig_t * data)
{
  SerialUSB.print("Network key: ");
  if(data->NetKey[0] != 0)
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
  SerialUSB.println(data->NodeID);
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

void loadConfigParams (flashConfig_t * data)
{
  SerialFlash.read(FLASH_CONFIG_PARAMS_ADDR, data, 256);
}

void eraseConfigParams (flashConfig_t * data)
{
  SerialFlash.eraseBlock(FLASH_CONFIG_PARAMS_ADDR);
  memset(data, 0, 256);
  while(!SerialFlash.ready());
  SerialUSB.println("Config parameters have been erased.");
}

void displayFlashInfo (void)
{
  Serial.println("Read Chip Identification:");
  SerialFlash.readID(buf);
  Serial.print("  JEDEC ID:     ");
  Serial.print(buf[0], HEX);
  Serial.print(" ");
  Serial.print(buf[1], HEX);
  Serial.print(" ");
  Serial.println(buf[2], HEX);
  Serial.print("  Part Nummber: ");
  Serial.println(id2chip(buf));
  Serial.print("  Memory Size:  ");
  chipsize = SerialFlash.capacity(buf);
  Serial.print(chipsize);
  Serial.println(" bytes");
  //if (chipsize == 0) return false;
  Serial.print("  Block Size:   ");
  blocksize = SerialFlash.blockSize();
  Serial.print(blocksize);
  Serial.println(" bytes");
  Serial.println();
}

