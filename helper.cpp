#include "main.h"
#include "edog.h"
#include "signal.h"

#define EEPROM_ADDR_MAIN_DATA  (EEPROM_ADDR_APP_DATA + 0x0000)

extern main_eeprom_data_st main_eeprom_data;

uint8_t main_eeprom_data_arr[8];

void helper_save_main_eeprom(void)
{   
    main_eeprom_data.main_state = signal_get_state();
    edog_put_tx_buff_uint16( 0, main_eeprom_data.main_state);
    edog_put_tx_buff_uint16( 2, main_eeprom_data.restart_cntr);
    edog_put_tx_buff_uint32( 4, 0x89ABCDEF);
    edog_write_eeprom_buff(EEPROM_ADDR_MAIN_DATA);
}

void helper_load_main_eeprom(void)
{
    edog_read_eeprom(EEPROM_ADDR_MAIN_DATA);
    main_eeprom_data.main_state = edog_get_rx_buff_uint16(0);
    main_eeprom_data.restart_cntr = edog_get_rx_buff_uint16(2);
    signal_set_state(main_eeprom_data.main_state);
}


void helper_initialize_data(void)
{
  bool incorrect_data = false;
  edog_set_wd_timeout(4000);
  delay(5);

  helper_load_main_eeprom();
  Serial.printf("State = %02X Restarts = %d\n\r",main_eeprom_data.main_state, main_eeprom_data.restart_cntr);
  if (main_eeprom_data.main_state > SIGNAL_STATE_SENDING) 
  {
    main_eeprom_data.main_state = SIGNAL_STATE_START;
    incorrect_data = true;
  }
  if (main_eeprom_data.restart_cntr > 10000) 
  {
    main_eeprom_data.restart_cntr = 0;
    incorrect_data = true;
  }  
  main_eeprom_data.restart_cntr++;
  signal_set_state(main_eeprom_data.main_state);
  if (incorrect_data)
  {
    Serial.printf("Fixed data: State = %02X Restarts = %d\n\r",main_eeprom_data.main_state, main_eeprom_data.restart_cntr);
  }
  delay(10);
  helper_save_main_eeprom();
}
