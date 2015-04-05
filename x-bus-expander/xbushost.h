#ifndef _X_BUS_H_
#define _X_BUS_H_

#include "arduino.h"
#include <stdint.h>

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

