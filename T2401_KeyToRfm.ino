



/**
Github repositories:
    https://github.com/infrapale/T2401_KeyToRfm   (this sketch)
    https://github.com/infrapale/T2312_Keypad_2x4
    https://github.com/infrapale/T2311_RFM69_Modem


---------------------       ------------------           ---------------------     ------------------  
| T2312_Keypad_2x4  |       | T2401_KeyToRfm |           | T2311_RFM69_Modem |     | RFM69 Receiver |
---------------------       ------------------           ---------------------     ------------------
   |                            |                              |                          | 
   |   <KP1:4=0> \n             |                              |                          |
   |--------------------------->|                              |                          |
   |                            |                              |                          |
   |                            |    <#X1N:RMH1;RKOK1;T;->\n   |                          |
   |                            |----------------------------->|                          |
   |                            |                              |                          | 
   |                            |                              |  {"Z":"MH1","S":"RKOK1", |
   |                            |                              |  "V":"T","R":"-"}        |
   |                            |                              |- - - - - - - - - - - - ->|
   |                            |                              |                          | 
   |     UART 1                 |       UART 2                 |    . . . 433MHz . . .    | 


  <KPx:y=z>\n
      x = keypad module index 
      y = keypad keyindex
      z = key value  '1', '0'
  <KP2:1=1>

https://arduino-pico.readthedocs.io/en/latest/serial.html

**/
#define PIN_SERIAL1_TX (0u)
#define PIN_SERIAL1_RX (1u)

#undef  PIN_SERIAL2_TX 
#undef  PIN_SERIAL2_RX

#define PIN_SERIAL_2_TX (4u)
#define PIN_SERIAL_2_RX (5u)

#define PIN_I2C_SCL     (9u)  
#define PIN_I2C_SDA     (8u)

#define LCD_I2C_ADDR    (0x27)
#define EDOG_I2C_ADDR   (13)

#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <Adafruit_NeoPixel.h>
#include "main.h"
#include "kbd_uart.h"
#include "task.h"
#include "func.h"
#include "relay.h"
#include "signal.h"
// #include "clock24.h"
#include "menu4x2.h"
#include "autom.h"
#include "edog.h"
#include "helper.h"
#include "supervisor.h"

  

//extern task_st *task[TASK_NBR_OF];
main_ctrl_st main_ctrl = {0x00};

main_eeprom_data_st main_eeprom_data;

void debug_print_task(void);

//                              name             ms next state prevcb
task_st read_key_task_handle      = {"Read Key       ", 100,0, 0, 255, run_read_key_commands };
task_st send_key_task_handle      = {"Send Key       ", 10, 0, 0, 255, run_send_key_commands };
task_st menu_timeout_task_handle  = {"Menu Timeout   ", 1000, 0, 0, 255, menu4x2_timeout_task };
task_st signal_task_handle        = {"Signal fast    ", 100, 0, 0, 255, signal_update};
task_st signal_state_task_handle  = {"Signal state   ", 1000, 0, 0, 255, signal_state_machine};
task_st autom_task_handle         = {"Automation     ", 1000,0, 0, 255, autom_task};
task_st supervisor_task_handle    = {"Supervisor     ", 100,0, 0, 255, supervisor_task};
task_st debug_print_handle        = {"Debug Print    ", 2000,0, 0, 255, debug_print_task};

int show = -1;
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

extern task_st task[TASK_NBR_OF];

void setup() {
  // put your setup code here, to run once:
  int error;
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
  
  Wire.setSCL(PIN_I2C_SCL);
  Wire.setSDA(PIN_I2C_SDA);
  Wire.begin();
  Wire.beginTransmission(LCD_I2C_ADDR);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
    show = 0;
    lcd.begin(20, 4);  // initialize the lcd
  } 
  else 
  {
    Serial.println(": LCD not found.");
  }  // if

  analogReadResolution(12);
  pinMode(PIN_PIR, INPUT);

  task_initialize(TASK_NBR_OF);
  task_set_task(TASK_READ_KEY, &read_key_task_handle); 
  task_set_task(TASK_SEND_RFM, &send_key_task_handle); 
  task_set_task(TASK_MENU_TIMEOUT, &menu_timeout_task_handle); 
  task_set_task(TASK_SIGNAL, &signal_task_handle);
  task_set_task(TASK_SIGNAL_STATE,&signal_state_task_handle);
  task_set_task(TASK_AUTOM, &autom_task_handle);
  task_set_task(TASK_SUPERVISOR, &supervisor_task_handle);
  task_set_task(TASK_DEBUG, &debug_print_handle);
  kbd_uart_initialize();
  //clock24_initialize();
  menu4x2_initialize();
  signal_initialize();
  supervisor_initialize();

  autom_initialize(6, 37);
  autom_randomize();

  edog_initialize(EDOG_I2C_ADDR);

  helper_initialize_data();
  // edog_test_eeprom_write_read();
  delay(5);
    
}

void loop() {
  //edog_clear_watchdog();
  
  task_run();    
}


void debug_print_task(void)
{
  // uint16_t a_ldr = analogRead(PIN_LDR);
  // Serial.println(a_ldr);
  // Serial.print("PIR="); Serial.println(digitalRead(PIN_PIR));
  
  // uint16_t clock_state = clock24_get_state());
  // if (clock_state & CLOCK_STATE_AT_HOME) Serial.print("At home -");
  // else  Serial.print("Away -");
  // if (clock_state & CLOCK_STATE_SENDING) Serial.print("Sending -");
  // if (clock_state & CLOCK_STATE_OPTION) Serial.print("Option -");
  // if (clock_state & CLOCK_STATE_CNTDWN) Serial.print("Countdown -");
  // Serial.println("");

}
 