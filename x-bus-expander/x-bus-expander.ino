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

#include <arduino.h>
#include <Wire.h>

#include "MCP23017.h"
#include "xbushost.h"

#define LED_PIN  13
#define CTS_PIN  9
//number of inputs/outputs total, 8 is maximum number of MCP23017 on one I2C bus.
#define NBR_OF_CHANNELS  8 

uint8_t output[NBR_OF_CHANNELS];
xBusHost xbus = xBusHost();
MCP23017 mcp[NBR_OF_CHANNELS];


void setup(){
  Wire.begin(); //start i2c lib
  
  //init MCP23017 io-expanders
  for(byte i=0;i<NBR_OF_CHANNELS;i++){
    mcp[i].begin(i);
    mcp[i].configure(0xff00); //portA input, portB output
    mcp[i].writePortB(0x00); //all off
  }
  
  xbus.begin(Serial, CTS_PIN);
}

void loop(){
  xbus.update();
  
  for(byte i=0;i<NBR_OF_CHANNELS;i++){
    //invert bits, this will make a non existing MCP to give inactive feedback.
    xbus.set_feedback(i, ~(mcp[i].readPortA())); 
  }
}

void notify_xbus_accessory_decoder_activated( uint8_t board, uint8_t board_output, uint8_t dir){
  if(board < NBR_OF_CHANNELS){ 
    if(dir){
      output[board] |= (1<<board_output);
    }else{
      output[board] &= ~(1<<board_output);
    }
    
    //update the specific MCP23017
    mcp[board].writePortB( output[board] );
  }
}

