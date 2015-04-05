#include <arduino.h>
#include "xbushost.h"

#define LED_PIN  13
#define CTS_PIN  9

xBusHost xbus = xBusHost();

#define NBR_OF_OUTPUTS  8
uint8_t output[NBR_OF_OUTPUTS];

uint8_t read_button(){
  uint8_t tmp=0;
  
  
  tmp |= digitalRead(3) ? 0x00 : 0x01;
  tmp |= digitalRead(4) ? 0x00 : 0x02;
  tmp |= digitalRead(5) ? 0x00 : 0x04;
  tmp |= digitalRead(6) ? 0x00 : 0x08;
  
  return tmp;
}


void setup(){
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  
  pinMode(LED_PIN, OUTPUT);
  xbus.begin(Serial, CTS_PIN);
}

void loop(){
  xbus.update();
  
  //todo: add reading from i2c bus
  xbus.set_feedback(0, output[0]);
  xbus.set_feedback(1, output[1]);
  xbus.set_feedback(2, read_button());
}

void notify_xbus_accessory_decoder_activated( uint8_t board, uint8_t board_output, uint8_t dir){
  if(board < NBR_OF_OUTPUTS){ 
    if(dir){
      output[board] |= (1<<board_output);
    }else{
      output[board] &= ~(1<<board_output);
    }
  }
}

