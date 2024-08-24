#include "main.h"
#include "edog.h"
#include "signal.h"
#include "eep.h"

#define EEPROM_ADDR_MAIN_DATA  (EEPROM_ADDR_APP_DATA + 0x0000)

//extern main_eeprom_data_st main_eeprom_data;
extern main_ctrl_st main_ctrl;
uint8_t main_eeprom_data_arr[8];


void helper_print_hex_arr(uint8_t *arr, uint8_t n)
{
    for (uint8_t i = 0; i < n;i++)
    {
      Serial.printf("%02X ", arr[i]);
    }
    Serial.println();
}

void helper_save_main_eeprom(void)
{ 
    eep_set_addr(EEP_ADDR_MAIN_STATE);
    eep_write_u16(main_ctrl.state);
    eep_set_addr(EEP_ADDR_RESTART_CNTR);
    eep_write_u16(main_ctrl.restart_cntr);
    eep_commit();
}

void helper_load_main_eeprom(void)
{
    eep_set_addr(EEP_ADDR_MAIN_STATE);
    main_ctrl.state = eep_read_u16();
    eep_set_addr(EEP_ADDR_RESTART_CNTR);
    main_ctrl.restart_cntr = eep_read_u16();
    signal_set_state(main_ctrl.state);
}


void helper_initialize_data(void)
{
  bool incorrect_data = false;
  //edog_set_wd_timeout(4000);
  delay(5);

  helper_load_main_eeprom();
  Serial.printf("State = %02X Restarts = %d\n\r",main_ctrl.state, main_ctrl.restart_cntr);
  if (main_ctrl.state > SIGNAL_STATE_SENDING) 
  {
    main_ctrl.state = SIGNAL_STATE_START;
    incorrect_data = true;
  }
  if (main_ctrl.restart_cntr > 10000) 
  {
    main_ctrl.restart_cntr = 0;
    incorrect_data = true;
  }  
  main_ctrl.restart_cntr++;
  signal_set_state(main_ctrl.state);
  if (incorrect_data)
  {
    Serial.printf("Fixed data: State = %02X Restarts = %d\n\r",main_ctrl.state, main_ctrl.restart_cntr);
  }
  delay(10);
  helper_save_main_eeprom();
}
