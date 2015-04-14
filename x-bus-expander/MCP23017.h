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

#ifndef _MCP23017_H_
#define _MCP23017_H_

#include <arduino.h>
#include <Wire.h>
#include <stdint.h>

class MCP23017
{
public:
  MCP23017();
  void begin(){ begin(0); };
  void begin(uint8_t sub_addr);

  //configure the io-expander
  void configure(uint16_t iodir);
  void configure(uint16_t iodir, uint16_t iopol);
  
  uint8_t readPortA();
  uint8_t readPortB();
  
  void writePortA(uint8_t data);
  void writePortB(uint8_t data);
private:
  uint8_t _address;

  void writeRegister8(uint8_t reg, uint8_t val);
  void writeRegister16(uint8_t reg, uint16_t val);
  
  uint8_t readRegister8(uint8_t reg);
  uint8_t readRegister16(uint8_t reg);
};

#endif
