/*
pin 37-39 for CE,OE,WE
pin 40-47 for IO(1-8)
pin 22-36 for addr(1-15)
can't be changed.
2023.11, Jimmy Zhang
*/

#ifndef _UT62256_H_
#define _UT62256_H_

#include "Arduino.h"
#include "stdint.h"

#define NOP do { __asm__ __volatile__ ("nop"); } while (0)

#define WRITE_ENABLE {PORTC &= B11111110, PORTG &= B11111011;}
#define WRITE_DISABLE {PORTG |= B00000100, PORTC |= B00000001;}
#define WRITE_PULSE {WRITE_ENABLE; WRITE_DISABLE;}
#define READ_ENABLE {PORTC &= B11111110, PORTD &= B01111111;}
#define READ_DISABLE {PORTC |= B00000001, PORTD |= B10000000;}

class UT62256 {
public:
    UT62256();
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr);

private:
    void setIOPin(bool isOutput);
    void setData(uint8_t data);
    void setAddr(uint16_t addr);
};

#endif