/*********************************************
File:   BMB22M210.CPP
Author: BESTMODULES
Description:  Function realization
History:
V1.0.0   -- initial version；2022-09-01；Arduino IDE : v1.8.16
**********************************************/

#include"BMB22M210.h"

/**********************************************************
Description: constructed function,HW Serial
Parameters: intPin: interrupt pin
            theSerial：HW serial port
            Note:BMduino UNO can choice &Serial、&Serial1~4
Return:  
Others: 
**********************************************************/
BMB22M210::BMB22M210(uint8_t intPin,HardwareSerial *theSerial)
{
  _intPin = intPin;
  _serial = theSerial;
}

/**********************************************************
Description: reset sub serial port
Parameters:  port:1/2/3/4
Return:  
Others: 
**********************************************************/
void BMB22M210::resetPort(uint8_t port)
{
  if(port == 1)
  {
    writeGReg(0x01,0x01);
  }
  else if(port == 2)
  {
    writeGReg(0x01,0x02);
  }
  else if(port == 3)
  {
    writeGReg(0x01,0x04);
  }
  else if(port == 4)
  {
    writeGReg(0x01,0x08);
  }
}

/**********************************************************
Description: initial module
Parameters:baud：Communication baud rate of development board and module
Return:
Others: 
**********************************************************/
void BMB22M210::begin(uint32_t baud)
{
    _serial->begin(baud);
    _serial->write(0x55);
    delay(100);
    _serial->read();
  
   writeGReg(0,0xff);
   writeGReg(1,0x0f);
   writeGReg(2,0x0); 
   writeGReg(0x10,0x00);
}

/**********************************************************
Description: initial module，only use on BMB22T101
Parameters: baud：Communication baud rate of development board and module
            resetPin：reset Pin,BMB22T101——reset Pin = D2
Return:
Others: 
**********************************************************/
void BMB22M210::begin(uint32_t baud,uint8_t rstPin)
{
    _rstPin = rstPin;

    pinMode(_rstPin,OUTPUT);
    digitalWrite(_rstPin,LOW);
    delay(10);
    digitalWrite(_rstPin,HIGH);
    delay(10);
    
    _serial->begin(baud);
    _serial->write(0x55);
    delay(100);
    _serial->read();
  
   writeGReg(0,0xff);
   writeGReg(1,0x0f);
   writeGReg(2,0x0); 
   writeGReg(0x10,0x00);
}

/**********************************************************
Description: Initialize sub serial port
Parameters: port： sub serial port(1~4)  
            baud：Baud rate of sub serial port
            config:UART config
              0——MYSERIAL_8N1
              1——MYSERIAL_8N2
              8——MYSERIAL_801
              9——MYSERIAL_802
              10——MYSERIAL_8O1
              11——MYSERIAL_8O2
              12——MYSERIAL_8E1
              13——MYSERIAL_8E2
              14——MYSERIAL_811
              15——MYSERIAL_812
Return:  
Others: 
**********************************************************/
void BMB22M210::beginPort(uint8_t port,uint32_t baud,uint8_t config)
{
   writeSReg(port,0x03,0);
   writeSReg(port,0x04,0x03);   
   writeSReg(port,0x05,config);   
   writeSReg(port,0x06,0x0F);  
  
   writeSReg(port,0x07,0x00);      
   writeSReg(port,0x08,0);     
   writeSReg(port,0x0B,0);       
   setPortBaud(port,baud); 
   writeSReg(port,0x04,0x03);
   delay(70);
}

/**********************************************************
Description: set sub serial port TX/RX enable
Parameters: port： sub serial port(1~4)
            status：tx/rx status
                   0 all disable
                   1 tx disable,rx enable
                   2 rx disable,tx enable
                   3 all enable
Return:  
Others: 
**********************************************************/
void BMB22M210::setPortStatus(uint8_t port,PORTSTATUS status)
{
    switch(status)
    {
    case 0:
         writeSReg(port,0x04,0x00);
         break;
    case 1:
         writeSReg(port,0x04,0x01);
         break;
    case 2:
         writeSReg(port,0x04,0x02);
         break;
    case 3:
         writeSReg(port,0x04,0x03);
         break;
    }
}

/**********************************************************
Description: get INT
Parameters:  none
Return:      INT status  
Others:      1:INT status is HIGH
             0:INT status is LOW
**********************************************************/
uint8_t BMB22M210::getINT()
{
   uint8_t statusValue = 0;
   statusValue = digitalRead(_intPin);
   return statusValue;
}




/**********************************************************
Description: send one byte data
Parameters: port： sub serial port(1~4)  
            data:  Eight bit data
Return:  
Others: 
**********************************************************/
void BMB22M210::write(uint8_t port,uint8_t data)
{
    switch (port){
                case 1:
                writeSReg(1,0x0d,data);
                break;
                case 2:
                writeSReg(2,0x0d,data);
                break;
                case 3:
                writeSReg(3,0x0d,data);
                break;
                case 4:
                writeSReg(4,0x0d,data);
                break;
    }


}

 
/**********************************************************
Description: receivr one byte data
Parameters: port： sub serial port(1~4)  
Return:  0~255:data
            -1:rx FIFO is empty
Others: 
**********************************************************/ 
int BMB22M210::read(uint8_t port) 
{ 
    int va = 0;
    int fifo = 0;
    if(port == 1)
    {
        if(readSReg(1,0x0a) >0)
        {
             va =   readSReg(1,0x0d);
        }
        else 
          {
            fifo = readSReg(1,0x0b);
            fifo >>=3;
            if(fifo & 0x01)
            {
                va =   readSReg(1,0x0d);
            }
            else{
              return -1;
            }
          }  
    }
    else if(port == 2)
    {
      
        if(readSReg(2,0x0a) > 0)
        {
                va =   readSReg(2,0x0d); 
        }
        else 
          {
            fifo = readSReg(2,0x0b);
            fifo >>=3;
            if(fifo & 0x01)
            {
                va =   readSReg(2,0x0d);
            }
            else{
              
              return -1;
            }
          }
    }
    else if(port == 3)
    {
        if(readSReg(3,0x0a) > 0)
        {
                va =   readSReg(3,0x0d);
        }
        else 
        {
            fifo = readSReg(3,0x0b);
            fifo >>=3;
            if(fifo & 0x01)
            {
                va =   readSReg(3,0x0d);
            }
            else
            {
              return -1;
            }
         }
    }
    else if(port == 4)
    {
        if(readSReg(4,0x0a) > 0)
         {
                va =   readSReg(4,0x0d);
         }
        else 
          {
            fifo = readSReg(4,0x0b);
            fifo >>=3;
            if(fifo & 0x01)
            {
                va =   readSReg(4,0x0d);
            }
            else{
                return -1;
            }
          }
    }
    return va;
 }

/**********************************************************
Description: query fifo quantity(receive)
Parameters: port： sub serial port(1~4)  
Return:  data quantity(0~255)
Others: 
**********************************************************/ 
uint8_t BMB22M210::available(uint8_t port) 
{
    uint8_t va = 0;
    unsigned char fifo = 0;
    
    if((port>=1)&&(port<=4))
    {
        va = availableReadSReg(port,0x0a);
        if(va > 0)
         {
             return va;
         }
        else 
        {
            fifo = availableReadSReg(port,0x0b);
            fifo >>=3;
            if(fifo & 0x01)
            {
                return 255;
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
      return 0;
    }
}


/**********************************************************
Description: query fifo quantity(transmit)
Parameters: port： sub serial port(1`4)  
Return:  data quantity(0~255)
        
Others: 
**********************************************************/
uint8_t BMB22M210::getTxFifoNum(uint8_t port)
{
    uint8_t va = 0;
    int fifo = 0;
    if((port>=1)&&(port<=4))
    {
        va = readSReg(port,0x09);
        if(va > 0)
         {
             return va;
         }
        else 
        {
            fifo = readSReg(port,0x0b);
            fifo >>=2;
            if(fifo & 0x01)
            {
                return 255;
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
      return 0;
    }
}



/**********************************************************
Description: read main serial register value
Parameters: greg: main serial register address 
Return:  main register value 
Others: 
**********************************************************/
int BMB22M210::readGReg(uint8_t greg)
{
  uint8_t recg;
  cmd=0x40|greg;
  _serial->write(cmd); 
  delayMicroseconds(100);
  if(_serial->available())
  {
    delayMicroseconds(1500);
    recg = _serial->read(); 
    return recg;
  }
  else
  {
    delay(5);
    recg = _serial->read();
    return recg;
  }

}


/**********************************************************
Description: write main serial register value
Parameters: greg:main serial register address  
            datg:main serial register value to  be written 
Return:   
Others: 
**********************************************************/
void BMB22M210::writeGReg(uint8_t greg,uint8_t datg)
{ 
  uint8_t cmd1 = 0;
  cmd1=0|greg;
  _serial->write(cmd1); 
  _serial->write(datg);
  delayMicroseconds(1500);

}


/**********************************************************
Description: write sub serial register value
Parameters: port:sub serial port(1~4)
            sreg:sub serial register address
            dats: sub serial register value to be written 
Return:   
Others: 
**********************************************************/
void BMB22M210::writeSReg(uint8_t port,uint8_t sreg,uint8_t dats)
{ 
  unsigned char cmd2 = 0;
  cmd2=0x0|((port-1)<<4)|sreg;
  _serial->write(cmd2);
  _serial->write(dats);
  delayMicroseconds(1500);

}


/**********************************************************
Description: read sub serial register value
Parameters: port:sub serial port(1~4) 
            sreg:sub serial register address  
Return:   sub serial register value
Others: 
**********************************************************/
int BMB22M210::readSReg(uint8_t port,uint8_t sreg)
{  
  int recs = 0;
  cmd = 0;
  cmd=0x40|((port-1)<<4)|sreg;
  _serial->write(cmd);
  if(_serial->available() > 0)
  {
    recs=_serial->read();
    return recs;
  }
  else
  {
    delay(5);
    recs=_serial->read(); 
    return recs;
  }

}

/**********************************************************
Description: read sub serial register value,only available() use
Parameters: port:sub serial port(1~4) 
            sreg:sub serial register address  
Return:   sub serial register value
Others: 
**********************************************************/
int BMB22M210::availableReadSReg(uint8_t port,uint8_t sreg)
{  
  int recs = 0;
  cmd = 0;
  cmd=0x40|((port-1)<<4)|sreg;
  _serial->write(cmd);
  delay(5);
  recs=_serial->read();
  return recs;

}
/**********************************************************
Description: write fifo data
Parameters: port： sub serial port(1~4)
            buff[]：Eight bit FIFO array
            num：Number of arrays written（1~16）
Return:   
Others: 
**********************************************************/
void BMB22M210::writeBytes(uint8_t port,uint8_t buff[],uint8_t num)
{
  uint8_t i= 0;
  if(num > 16)
  {
    num = 16;
  }
  cmd=0x80|((port-1)<<4)|(num-1);
  _serial->write(cmd); 
  for(i=0;i<num;i++)
  { 
    _serial->write(buff[i]);
    delay(2);
  }

}


/**********************************************************
Description: read fifo data
Parameters: port： sub serial port(1~4) 
            buff[]：Eight bit FIFO array
            num：Number of arrays written（1~16）
Return:
Others: 
**********************************************************/
void BMB22M210::readBytes(uint8_t port,int buff[],uint8_t num)
{
  uint8_t n ;
  if(num > 16)
  {
    num = 16;
  }
 cmd=0xc0|((port-1)<<4)|(num-1);
  _serial->write(cmd);
 for(n=0;n<num;n++)
 {
    if(_serial->available() > 0)
    {
       buff[n]  = _serial->read();
    }
    else
    {
       delay(5);
       buff[n]  = _serial->read();
    }
 }

}



/**********************************************************
Description: set sub serial ports baud
Parameters: port:sub serial port(1~4)
            baud:sub serial baud(4800/9600/14400/19200/38400/115200/230400)
Return:  
Others: 
**********************************************************/
void BMB22M210::setPortBaud(uint8_t port,uint32_t  baud)
{ 
  uint8_t baud1,baud0,pres;
  switch (baud) 
  {
  case 4800:
    baud1=0x00;
    baud0=0x8f;
    pres=0;
    break;
  case 9600:
    baud1=0x00;
    baud0=0x47;
    pres=0;
    break;
  case 14400:
    baud1=0x00;
    baud0=0x2f;
    pres=0;
    break;
  case 19200:
    baud1=0x00;
    baud0=0x23;
    pres=0;
    break;
  case 38400:
    baud1=0x00;
    baud0=0x11;
    pres=0;
    break;
  case 57600:
    baud1=0x00;
    baud0=0x0b;
    pres=0;
    break;
  case 115200:
    baud1=0x00;
    baud0=0x05;
    pres=0;
    break;
  case 230400:
    baud1=0x00;
    baud0=0x02;
    pres=0;
    break;
  default:
    baud1=0x00;
    baud0=0x47;
    pres=0;
  }
  writeSReg(port,3,0);
  writeSReg(port,0x04,0);

  writeSReg(port,3,1);
  writeSReg(port,4,baud1);
  writeSReg(port,5,baud0);
  writeSReg(port,6,pres);
  writeSReg(port,3,0);

  writeSReg(port,0x04,3);
}
