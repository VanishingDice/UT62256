#include "UT62256.h"

UT62256::UT62256() {
    setIOPin(OUTPUT);
    DDRA = B11111111;//Set 22-36 to OUTPUT
    DDRC |= B11111110;
    DDRC |= B00000001;//Set EN(37) to OUTPUT
    DDRD |= B10000000;//Set OE(38) to OUTPUT
    DDRG |= B00000100;//Set WE(39) to OUTPUT
    WRITE_DISABLE;
    READ_DISABLE;
}

void UT62256::setIOPin(bool isOutput) {
    if(isOutput) {
        DDRG |= B00000011;//40,41
        DDRL |= B11111100;//42-47
    } else {
        DDRG &= B11111100;//40,41
        DDRL &= B00000011;//42-47
    }
}

void UT62256::setAddr(uint16_t addr) {
    PORTA = (addr & B11111111);
    uint8_t addrH = (addr >> 8) & B01111111;
    //Reverse addr
    addrH = (((addrH & B10101010) >> 1) | ((addrH & B01010101) << 1));
    addrH = (((addrH & B11001100) >> 2) | ((addrH & B00110011) << 2));
    addrH = (((addrH & B11110000) >> 4) | ((addrH & B00001111) << 4));
    PORTC = addrH | (PORTC & B00000001);
}

void UT62256::setData(uint8_t data) {
    PORTG = (PORTG & B11111100) | ((data & B00000001) << 1 | (data & B00000010) >> 1 );
    PORTL = (PORTL & B00000011) | ((data & B10000000) >> 5 | (data & B01000000) >> 3 | (data & B00100000) >> 1 
                                | (data & B00010000) << 1 | (data & B00001000) << 3 | (data & B00000100) << 5 );
}

void UT62256::write(uint16_t addr, uint8_t data) {
    setIOPin(OUTPUT);
    setAddr(addr);
    setData(data);
    WRITE_PULSE;
}

uint8_t UT62256::read(uint16_t addr) {
    uint8_t reg = 0;
    setIOPin(INPUT);
    setAddr(addr);
    READ_ENABLE;
    NOP;//UT62256 needs time to give output, delete this will cause terrible problem :)
    //Read from pin register
    reg = (PING & B00000001) << 1 | (PING & B00000010) >> 1 
        | (PINL & B10000000) >> 5 | (PINL & B01000000) >> 3 | (PINL & B00100000) >> 1 
        | (PINL & B00010000) << 1 | (PINL & B00001000) << 3 | (PINL & B00000100) << 5;
    READ_DISABLE;
    return reg;
}