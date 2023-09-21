/*****************************************************************
File:         servosRun.ino
Description:
             1.Set the EID and initial Angle of the steering gear module.
             2.Set the rotation of the steering gear module according to the touch status of the 4 key capacitive Touch module,
               And the current Angle of the steering gear module is displayed on the serial port monitor
               
connection method： 
                  BMK52M134：intPin:STA(D3)  SCLPin:SCL/TX(D19)   SDLPin:SDL/RX(D18)
                  BM22O2221_A： TRX:TRX1(D6 and D7)
******************************************************************/
#include "BMK52M134.h"
#include <SoftwareSerial.h>

/***One-Wire UART Port(UART1)****/
#define TX1   6
#define RX1   7
/***One-Wire UART Port(UART2)****/
//#define TX2   11
//#define RX2   12

BMK52M134     BMK52(3,&Wire);  // Create the object
SoftwareSerial mySerial(RX1,TX1); //  Create the object，Select the UART1
//SoftwareSerial mySerial(RX2,TX2); //  Create the object，Select the UART2

uint8_t angle = 45; //Steering gear Angle
uint8_t KEYValue = 0; //Save key value

void setup() 
{
  Serial.begin(9600); //Example Initialize the serial port monitor
  BMK52.begin();  //Example Initialize the 4 key capacitive Touch  module
  mySerial.begin(115200, SERIAL_9N1); //Initialize the small actuator module
  setEID(1);  //Set the EID of the steering gear module to 1
  setPosTime(1,angle,0);//Set the steering gear Angle to the default 45°
  Serial.print("Angle:"); 
  Serial.print(angle); 
  Serial.println("°");
}

void loop() 
{
    if(!BMK52.getINT()) //Check whether the key is pressed
   {
    KEYValue = BMK52.getKeyValue(); //get Key Value
    switch (KEYValue)
    {
    case 1: angle = 0; break;   //When KEY1 is pressed, the steering gear Angle parameter is set to 0° 
    case 2: angle = 45; break;  //When KEY2 is pressed, the steering gear Angle parameter is set to 45°
    case 3: angle = 90; break;  //When KEY3 is pressed, the steering gear Angle parameter is set to 90°
    case 4: angle = 140; break; //When KEY4 is pressed, the steering gear Angle parameter is set to 140°
    }
    setPosTime(1,angle,0); //According to the key situation, the drive motor rotates to the corresponding Angle
    Serial.print("Angle:"); 
    Serial.print(angle); 
    Serial.println("°");
   }
}


uint8_t setEID(uint8_t EID)
{
  uint16_t rBuf[4]={0};
  uint16_t mid = 0x100;
  uint8_t Tlen_eid = (0X02<<4)+0x00;
  uint8_t instr = 0x80 + (EID & 0x0f);
  uint8_t check_sum = (uint8_t)~(mid + Tlen_eid + instr);
  mySerial.SetRxStatus(DISABLE); //RXPin DISABLE
  mySerial.SetTxStatus(ENABLE);  //TXPin ENABLE
  /***write command****/
  mySerial.write(mid);
  mySerial.write(Tlen_eid);
  mySerial.write(instr);
  mySerial.write(check_sum);
  mySerial.flush();
  mySerial.SetRxStatus(ENABLE); //RXPin ENABLE
  mySerial.SetTxStatus(DISABLE); //TXPin DISABLE
  delay(13);
  /***read Module reply****/
  rBuf[0] = mySerial.read();
  rBuf[1] = mySerial.read();
  rBuf[2] = mySerial.read();
  rBuf[3] = mySerial.read();
  delay(20);
  return rBuf[2];
}

uint8_t setPosTime(uint8_t EID,int16_t Position, uint16_t Moving_Time)
{
  uint8_t rBuf[4]={0};
  uint16_t mid = 0x125;
  uint8_t Tlen_eid = (0x06<<4)+EID;
  uint8_t instr = 0x09;
  uint8_t DATA1 = Position;
  uint8_t DATA2 = Position >> 8;
  uint8_t DATA3 = Moving_Time;
  uint8_t DATA4 = Moving_Time >> 8;
  uint8_t check_sum = (uint8_t)~(mid + Tlen_eid + instr + DATA1 + DATA2 + DATA3 + DATA4);
  mySerial.SetRxStatus(DISABLE); //RXPin DISABLE
  mySerial.SetTxStatus(ENABLE);  //TXPin ENABLE
  /***write command****/
  mySerial.write(mid);
  mySerial.write(Tlen_eid);
  mySerial.write(instr);
  mySerial.write(DATA1);
  mySerial.write(DATA2);
  mySerial.write(DATA3);
  mySerial.write(DATA4);
  mySerial.write(check_sum);
  mySerial.flush();
  mySerial.SetRxStatus(ENABLE); //RXPin ENABLE
  mySerial.SetTxStatus(DISABLE); //TXPin DISABLE
  delay(13);
  /***read Module reply****/
  rBuf[0] = mySerial.read();
  rBuf[1] = mySerial.read();
  rBuf[2] = mySerial.read();
  rBuf[3] = mySerial.read();
  delay(20);
  return rBuf[2];
}
