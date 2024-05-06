#ifndef __HELPER_H__
#define __HELPER_H__

void helper_save_main_eeprom(void);

void helper_load_main_eeprom(void);

void helper_initialize_data(void);

void helper_print_hex_arr(uint8_t *arr, uint8_t n);

#endif