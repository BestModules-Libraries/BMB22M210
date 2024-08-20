/*********************************************
File:           BMB22M210.h
Author:         BEST MODULES CORP.
Description:    Define classes and required variables
Version:        V1.0.3   -- 2024-08-20
**********************************************/
#ifndef BMB22M210_H_
#define BMB22M210_H_
#include <Arduino.h>

/*  config  */
#define  MYSERIAL_8N1  0   //data:8bit、No check、  Stop bit:1bit
#define  MYSERIAL_8N2  1   //data:8bit、No check、  Stop bit:2bit
#define  MYSERIAL_801  8   //data:8bit、0 check、   Stop bit:1bit
#define  MYSERIAL_802  9   //data:8bit、0 check、   Stop bit:2bit
#define  MYSERIAL_8O1  10  //data:8bit、Odd check、 Stop bit:1bit
#define  MYSERIAL_8O2  11  //data:8bit、Odd check、 Stop bit:2bit
#define  MYSERIAL_8E1  12  //data:8bit、Even check、Stop bit:1bit
#define  MYSERIAL_8E2  13  //data:8bit、Even check、Stop bit:2bit
#define  MYSERIAL_811  14  //data:8bit、1 check、   Stop bit:1bit
#define  MYSERIAL_812  15  //data:8bit、1 check、   Stop bit:2bit

enum PORTSTATUS
{ 
    TXRX_DISABLE = 0,
    TX_DISABLE = 1,
    RX_DISABLE = 2,
    TXRX_ENABLE = 3,
};


class BMB22M210{
  public:
  BMB22M210(uint8_t intPin,HardwareSerial *theSerial = &Serial);
  void begin(uint32_t baud);
  void begin(uint32_t baud,uint8_t rstPin);   //only use on BMB22T101 Module
  void beginPort(uint8_t port,uint32_t baud,uint8_t config=MYSERIAL_8N1);
  uint8_t getINT();
  void setPortStatus(uint8_t port,PORTSTATUS status);
  void write(uint8_t port,uint8_t data);
  int read(uint8_t port);
  void writeBytes(uint8_t port,uint8_t buff[],uint8_t num);
  void readBytes(uint8_t port,int buff[],uint8_t num);
  uint8_t available(uint8_t port);
  void resetPort(uint8_t port);
  
  uint8_t getTxFifoNum(uint8_t port);
  uint8_t getFifoStatus(uint8_t port);
  uint8_t getInterruptFlag(uint8_t port);
  void setTxFifoInterrupt(uint8_t port,uint8_t data);
  void setRxFifoInterrupt(uint8_t port,uint8_t data);
  
  private:
  int readGReg(uint8_t greg);
  void writeGReg(uint8_t greg,uint8_t datg);
  void writeSReg(uint8_t port,uint8_t sreg,uint8_t dats);
  int readSReg(uint8_t port,uint8_t sreg);
  int availableReadSReg(uint8_t port,uint8_t sreg);
  void setPortBaud(uint8_t port,uint32_t  baud);
  
  HardwareSerial *_serial = NULL;
  uint8_t cmd = 0;
  uint8_t _intPin = 0;
  uint8_t _rstPin = 0;
};
#endif                                                       
