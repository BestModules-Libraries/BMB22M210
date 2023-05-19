/*****************************************************************
File:         write_read.ino
Description:  1.Application circuit:module connection to BMCOM1 on BMduino UNO
              2.explanation:uart port1 send 1 byte data to uart port 2,then port 2 read
              data and display on the Serial Monitor;uart port1 send 16 bytes data to uart
              port 2,then port 2 read data and display on the Serial Monitor.
******************************************************************/
#include "BMB22M210.h"

BMB22M210 mySerial(22,&Serial1);   //Please uncomment out this line of code if you use HW Serial1 on BMduino
//BMB22M210 mySerial(25,&Serial2); //Please uncomment out this line of code if you use HW Serial2 on BMduino
//BMB22M210 mySerial(3,&Serial3);  //Please uncomment out this line of code if you use HW Serial3 on BMduino
//BMB22M210 mySerial(3,&Serial4);  //Please uncomment out this line of code if you use HW Serial4 on BMduino

uint8_t sendbuff[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
int receivebuff[16]={0};
uint8_t i=0;
int data = 0;

void setup() 
{ 
   Serial.begin(9600);       //Serial Monitor
   delay(1000);
   mySerial.begin(9600);         //initial module
   mySerial.beginPort(1,9600);   //set uart port1 baud：9600
   mySerial.beginPort(2,9600);   //set uart port2 baud：9600
}

void loop() 
{
   Serial.println("port1 write 1 byte:0x75");
   mySerial.write(1,0x75);                //uart port1 write 1 byte data
   delay(2);
   Serial.print("num of port2 can be read:");
   Serial.println(mySerial.available(2));//num of uart port2 can be read
   Serial.print("port2 read:");
   if(mySerial.available(2)>=1)
   {
     Serial.print(mySerial.read(2));     //uart port2 read 1 byte data
     Serial.print(" ");
   }
   Serial.println("");

   Serial.println("port1 write 16 bytes:1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16");
   mySerial.writeBytes(1,sendbuff,16);    //uart port1 write 16 bytes data
   delay(20);
   Serial.print("num of port2 can be read:");
   Serial.println(mySerial.available(2)); //num of uart port2 can be read
   Serial.print("port2 read:");
   if(mySerial.available(2)>=16)
   {
     mySerial.readBytes(2,receivebuff,16); //uart port2 read 16 bytes data
     for(i=0;i<16;i++)
     {
       Serial.print(receivebuff[i]);
       Serial.print(" ");
     }
   }
   Serial.println("");
   Serial.println("");
   delay(1000);
}




  
  
