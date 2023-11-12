/*
pin 37-39 for CE,OE,WE
pin 40-47 for IO(1-8)
pin 22-36 for addr(1-15)
can't be changed.
2023.11, Jimmy Zhang
*/

#include "UT62256.h"

#define DATA_SIZE 5000
#define TEST_LEN 32768

uint8_t dataReg = 0;
uint8_t data[DATA_SIZE];
uint8_t count = 0;
UT62256 ut = UT62256();

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(A0));
}

void loop() {
  randTest();
  // while(1) {}//if needs to run only one cycle
}

void randTest() {
  ++count;//Test cycle counter
  Serial.println("Test cycle "+String(count)+" Start.");
  
  Serial.println("Init data.");
  initRandData();//Use random data to initialization data array
  // initSeqData();//Use sequence data to initializaion data array
  Serial.println("Init data done.");

  Serial.println("Start writing.");
  for(uint16_t addr = 0; addr < TEST_LEN; ++addr) {
    ut.write(addr, data[addr%DATA_SIZE]);
  }
  Serial.println("Writing done.");

  Serial.println("Start reading");
  for(uint16_t addr = 0; addr < TEST_LEN; ++addr) {
    dataReg = ut.read(addr);
    if(dataReg == data[addr%DATA_SIZE]) {
      // Serial.println("Equal.");
    } else {
      Serial.println("ERROR: Address:"+String(addr)+"\t Value:"+dataReg);
      while(1){}
    }
  }
  Serial.println("Reading done.");

  Serial.println("Test cycle "+String(count)+" done.\n");
}

void initRandData() {
  for(uint16_t i = 0; i < DATA_SIZE; ++i) {
    data[i] = random(0,256);
  }
}

void initSeqData() {
  for(uint16_t i = 0; i < DATA_SIZE; ++i) {
    data[i] = i;
  }
}

