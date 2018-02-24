

#include <SerialFlash.h>

#define mySerial            Serial
#define SPI_FLASH_CS_PIN    4

uint8_t flashID[5];
uint8_t flashSerialNum[8];
bool returnStatus = false;

void setup() 
{
  // put your setup code here, to run once:
  mySerial.begin(115200);
  while(!mySerial);
  mySerial.print("Press any key to begin");
  while(0 >= mySerial.available());
  mySerial.println(mySerial.read());
  
  SerialFlash.begin(SPI_FLASH_CS_PIN);
  mySerial.println();
  
  mySerial.println("Ready.");
  mySerial.println();
  delay(100);

  mySerial.println("Reading Flash ID");
  SerialFlash.wakeup();
  delay(10);
  SerialFlash.readID(flashID);
  mySerial.print("ID, Memory Type, Capacity: ");
  for(int idx = 0; idx < sizeof(flashID); idx++)
  {
    mySerial.print(flashID[idx], HEX);
    mySerial.print(",");
  }
  mySerial.println();
  mySerial.println();

  SerialFlash.readSerialNumber(flashSerialNum);
  mySerial.print("S/N: ");
  for(int idx = 0; idx < sizeof(flashSerialNum); idx++)
  {
    mySerial.print(flashSerialNum[idx], HEX);
  }
  mySerial.println();
}

void loop() 
{
  // put your main code here, to run repeatedly:
}


