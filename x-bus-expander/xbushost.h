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

#ifndef _X_BUS_H_
#define _X_BUS_H_

#include "arduino.h"
#include <stdint.h>
#include <HardwareSerial.h>

//number of 8bit value to report back
#define XBUS_HOST_MAX_FEEDBACK  8

class xBusHost{
  private:
    HardwareSerial* serial;
    uint8_t cts_pin;
    
    void process(void);
    void parse(uint8_t data);
    
    uint8_t feedback_status[XBUS_HOST_MAX_FEEDBACK];
    
    //helper functions
    void interface_version_request();
    void command_station_request();
    void decoder_information_request();
    void decoder_operation_request();
    
    void send(uint8_t header, uint8_t *data, uint8_t len);
    void setCTS(bool ready);
    
    uint8_t fsm_header;
    uint8_t fsm_data[15];
    uint8_t fsm_checksum;
    
    uint8_t fsm_cnt;
    
    
    
    
  public:
    xBusHost();
    void begin(HardwareSerial &serial, uint8_t cts_pin);
    void update(void);
    
    //board : 0-255, 
    void set_feedback(uint8_t board, uint8_t status);
};


//This expect every board has 8 outputs = 1024 outputs
//board 0-127, output 0-7
extern void notify_xbus_accessory_decoder_activated( uint8_t board, uint8_t board_output, uint8_t direction) __attribute__ ((weak));
extern void notify_xbus_accessory_decoder_deactivated( uint8_t board, uint8_t board_output, uint8_t direction) __attribute__ ((weak));

#endif

