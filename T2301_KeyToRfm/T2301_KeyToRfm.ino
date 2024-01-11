#define PIN_SERIAL1_TX (0u)
#define PIN_SERIAL1_RX (1u)

#undef  PIN_SERIAL2_TX 
#undef  PIN_SERIAL2_RX

#define PIN_SERIAL_2_TX (4u)
#define PIN_SERIAL_2_RX (5u)

#include "Arduino.h"
#include "main.h"
#include "kbd_uart.h"



/**
  <KPx:y=z>\n
      x = keypad module index 
      y = keypad keyindex
      z = key value  '1', '0'
  <KP2:1=1>

https://arduino-pico.readthedocs.io/en/latest/serial.html

**/
  


void setup() {
  // put your setup code here, to run once:
  
  Serial1.setTX(PIN_SERIAL1_TX);
  Serial1.setRX(PIN_SERIAL1_RX);
  Serial2.setTX(PIN_SERIAL_2_TX);
  Serial2.setRX(PIN_SERIAL_2_RX);

  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);

  delay(3000);
  Serial.println(APP_NAME);
  Serial.println(__DATE__); Serial.println(__TIME__);
  
  kbd_uart_initialize();

}

void loop() {
  // put your main code here, to run repeatedly:

  while(false) 
  {
    Serial1.write("A5A5A5");
    Serial2.write("A5A5A5");
    delay(2000);
  }  
  // Serial1.write("A5");
  // Serial2.write("5A");
    
  if (kbd_uart_read())
  {
      kbd_uart_parse_rx();
  }

  if (Serial1.available()) {
    Serial.write(Serial1.read());   
  }
  // delay(50);
  // if (Serial2.available()) {     
  //   Serial1.write(Serial2.read() & 0xAA);   
  // }
  delay(50);
}
