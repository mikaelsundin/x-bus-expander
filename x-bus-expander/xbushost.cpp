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

#include "xbushost.h"

#define XBUS_HOST_BAUDRATE_DEFAULT  9600
#define XBUS_WAIT_HEADER 0xff

#define low_nibble(x)    (x>>0 & 0x0f);
#define high_nibble(x)   (x>>4 & 0x0f);


xBusHost::xBusHost(){
  int x=0;
  fsm_cnt = XBUS_WAIT_HEADER;
}

//get xbus interface information
void xBusHost::interface_version_request(){
    serial->write(0x02); //header
    serial->write(0x30); //Hardware Version 3.0
    serial->write(0x01); //Software Version 01
    serial->write(0x33); //xor
}

void xBusHost::command_station_request(){
   switch(fsm_data[0]){
   case 0x21: //version
     serial->write(0x63);  //Command station response
     serial->write(0x21);  //type: version
     serial->write(0x30);  //command station version: 3.0
     serial->write(0x00);  //command station type
     serial->write(0x72);  //xor
     break;
   case 0x24: //status request
     serial->write(0x62);  //Command station response
     serial->write(0x22);  //type: status
     serial->write(0x00);  //Power on - no error
     serial->write(0x40);  //xor
     break;  
   case 0x80: //Stop operation request 
     serial->write(0x61);  //Track power off
     serial->write(0x00);  //
     serial->write(0x61);  //xor
     break;
   case 0x81: //Normal operation request
     serial->write(0x61);  //Normal operation resumed
     serial->write(0x01);  //
     serial->write(0x60);  //xor
     break;
   }
}

//host ask for decoder information
//always return that all is a feedback module
void xBusHost::decoder_information_request(){
  uint8_t data[2];
 
  data[0] = fsm_data[0];
  data[1] = fsm_data[1] & 0x01 ? 0x10 : 0x00; 
    
  send(0x40, data, 2);
}

void xBusHost::decoder_operation_request(){
  uint8_t board=0;
  uint16_t output=0; //0-7
  uint8_t direction;
  
  board = (fsm_data[0]>>1);
  output |= fsm_data[0] & 0x01 ? 0x04 : 0x00;
  output |= fsm_data[1] & 0x04 ? 0x02 : 0x00;
  output |= fsm_data[1] & 0x02 ? 0x01 : 0x00;
  direction = fsm_data[1] & 0x01;
  
  //route information to correct function
  if( fsm_data[1] & 0x08){
    if(notify_xbus_accessory_decoder_activated){
      notify_xbus_accessory_decoder_activated(board, output, direction);
    }
  }else{
    if(notify_xbus_accessory_decoder_deactivated){
      notify_xbus_accessory_decoder_deactivated(board, output, direction);
    }
  }

  //send something back for rocrail and other program to be happy. using highest 
  byte msg[2] = {0xff, 0x40};
  send(0x40, msg, 2);
}

void xBusHost::process(void){
  
  //check header and length at same time
  if(fsm_header == 0xF0){  		 //version request
    interface_version_request();
  }else if(fsm_header == 0x21){  //command station request
    command_station_request();
  }else if(fsm_header == 0x42){  //Accessory Decoder information request 
    decoder_information_request();
  }else if(fsm_header == 0x52){  //Accessory Decoder operation request
    decoder_operation_request();
  }else{ 
     //command not supported
     serial->write(0x61);
     serial->write(0x82);
     serial->write(0xE3); 
  }
}
    
void xBusHost::send(uint8_t header, uint8_t *data, uint8_t len){
  uint8_t checksum = header + len;
  
  serial->write(header+len);
  while(len--){
      checksum ^= *data;
      serial->write(*data++);
  }
  serial->write(checksum);
}

void xBusHost::parse(uint8_t data){
  static uint8_t index;
  if(fsm_cnt == XBUS_WAIT_HEADER){
    fsm_cnt = data & 0x0f;
    fsm_header = data;
    fsm_checksum = data;
    index = 0;
  }else if(fsm_cnt == 0){
    if(data == fsm_checksum){ 
        setCTS(false);
        process();
        setCTS(true);
    }else{
      serial->write(0x61);
      serial->write(0x80);
      serial->write(0xE1);
    }
    
    fsm_cnt = XBUS_WAIT_HEADER;
  }else{
      fsm_data[index++] = data;
      fsm_checksum ^= data;
      fsm_cnt--;
  }
}

inline void xBusHost::setCTS(bool ready){
  digitalWrite(cts_pin, ready ? LOW : HIGH);
}

void xBusHost::begin(HardwareSerial &serial, uint8_t cts_pin){
  this->cts_pin = cts_pin;
  pinMode(this->cts_pin, OUTPUT);
  
  
  this->serial = &serial;
  this->serial->begin(XBUS_HOST_BAUDRATE_DEFAULT);
  
  setCTS(true);
}

void xBusHost::update(){
  uint8_t c;
  if( serial->available() ){
    parse( serial->read() );
  }
}

//xbus feedback response:
// 0x4NNNN, AAAAAAAA, ITTNZZZZ, (AAAAAAAA, ITTNZZZZ ....)
// NNNN = number of bytes in packet
// AAAA AAAA = Address
// ITTNZZZZ
//   I: always 0 for feedback module
//   TT : type of feedback 10 = feedback module
//   N  : Nibble
//   ZZZZ : actual data
void xBusHost::set_feedback(uint8_t board, uint8_t status){
  uint8_t data[4];
  uint8_t cnt;
  
  if(board < XBUS_HOST_MAX_FEEDBACK){
     if(feedback_status[board] != status){
         data[0] = board;
         data[1] = 0x40 | low_nibble(status);
         
         data[2] = board;
         data[3] = 0x50 | high_nibble(status);

         send(0x40, data, 4);
         feedback_status[board] = status;
     }
   
  } 
  
  
}


