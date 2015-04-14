/******************************************************************************
Copyright (c) 2015, Mikael Sundin
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


******************************************************************************/
/*
 * A simple implementation of MCP23017 to hande the expander as two 8-bits ports.
 */
#include "MCP23017.h"


/* Defines */
#define MCP23017_BASE_ADDRESS  0x40
#define MCP23017_REG_IODIR     0x00
#define MCP23017_REG_IOPOL     0x02

#define MCP23017_REG_GPIOA     0x12
#define MCP23017_REG_GPIOB     0x13

MCP23017::MCP23017(){
	
}

/**********************************************/
/* Port functions                             */
uint8_t MCP23017::readPortA(){ 
  return readRegister8(MCP23017_REG_GPIOA); 
};

uint8_t MCP23017::readPortB(){ 
  return readRegister8(MCP23017_REG_GPIOB);
};
  
void MCP23017::writePortA(uint8_t data){
  writeRegister8(MCP23017_REG_GPIOA, data);
} 
void MCP23017::writePortB(uint8_t data){
  writeRegister8(MCP23017_REG_GPIOB, data);
} 
/**********************************************/


/**********************************************/
/* Configure functions                        */
void MCP23017::begin(uint8_t sub_adr) {
  _address  = (MCP23017_BASE_ADDRESS>>1) | (sub_adr & 0x07);
}

void MCP23017::configure(uint16_t iodir){
  configure(iodir, 0x0000);
}
 
void MCP23017::configure(uint16_t iodir, uint16_t iopol){
  writeRegister16(MCP23017_REG_IODIR, iodir);
  writeRegister16(MCP23017_REG_IOPOL, iopol);
}
/**********************************************/


/**********************************************/
/* MCP23017 register functions                */
void MCP23017::writeRegister8(uint8_t reg, uint8_t val){
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}
 
void MCP23017::writeRegister16(uint8_t reg, uint16_t val){
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(highByte(val));
  Wire.write(lowByte(val));
  Wire.endTransmission();
}

uint8_t MCP23017::readRegister8(uint8_t reg){
  uint8_t ret=0;
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom((int)_address, 1);
  ret = Wire.read();
  
  return ret;
}
/**********************************************/



